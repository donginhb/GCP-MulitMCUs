/**
  ******************************************************************************
  * @file    SPI.c
  * @author  Duhanfeng
  * @version V2.0 寄存器版本
  * @date    2017.04.18
  * @brief   SPIx实现文件
  ******************************************************************************
  * @attention
  * 
  *
  * SPIx:   NSS  --- PA4   通用推挽,10M
  *         SCK  --- PA5   复用推挽,50M
  *         MISO --- PA6   上拉输入
  *         MOSI --- PA7   复用推挽,50M
  *
  * SPI2:   NSS  --- PB12  通用推挽,10M
  *         SCK  --- PB13  复用推挽,50M
  *         MISO --- PB14  上拉输入
  *         MOSI --- PB15  复用推挽,50M
  *
  * SPI3:   NSS  --- PA15  通用推挽,10M
  *         SCK  --- PB3   复用推挽,50M
  *         MISO --- PB4   上拉输入
  *         MOSI --- PB5   复用推挽,50M
  * 
  * 数据帧: 8位数据位,高位在前,每发一位出去从机都会返回一位的数据回来
  * 
  * 本文件中将SPI的NSS片选功能配置为软件管理模式,这样,外接SPI设备时不再受限于此片选位
  * 的实际连接情况(防止片选脚复用).
  * 
  * SPI1的时钟为72M,SPI2,3的时钟为36M,故而SPI1不可以2分频(最大18M)
  * 
  * 
  * V2.0------------
  * 修改描述: 修复配置速度函数无效的错误
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPI[uSpiNode]->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
  *
  * V3.0------------
  * 修改描述: 修改底层实现机制,将本文件匹配SPI1~3
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2017.03.10
  *
  * V4.0------------
  * 修改描述: 1.增加SPI的主从机模式配置
  *           2.发现其主从模式下从机接收数据移位的原因及解决方案
  *           3.发现仿真情况下SPI卡死的原因以及解决方案(适用于其他外设类似的情况)
  *           4.开放部分接口
  *           5.将SPI读写函数输入参的数据类型从8位改成16位(兼容16位数据)
  * 主从模式: 在SPI从模式中,SLK,MISO,MOSI和主机的IO一一对应连接,但是需要根据实际的
  *           情况来配置其输入输出模式.从模式下,从机发送的数据只是存在在发送缓存寄
  *           存器中,等待主机的下一次数据时钟的到来才逐位移位.
  * 数据移位: 在SPI从模式接收中,接收到的数据有可能和实际主机发送的数据不一致,存在数
  *           据移位(不一致)的现象,这时主从模式的时钟不同步导致的.这个现象往往是主
  *           从机在初始化过程中,IO不稳定的状态导致其从机误识别为其接收到的数据,从
  *           而让两者的数据不同步.以主从机环回通信测试为例,这个问题的解决方法是先
  *           对主从机IO进行模式配置,再对SPI进行初始化(先配置主机再配置从机),让从
  *           机在启动的时候已经错过了主机导致的IO抖动的情况.
  * 仿真卡死: 在仿真时,如果在SPI读写函数中的 while (!(SPI[uSpiNode]->SR & (0X1<<0)));
  *           语句处打断点(尚未执行),再执行此条件判断语句,则发现其无法通过其条件判
  *           断,从而让系统卡死在此处,但是直接的执行就不会出现这个问题.经调试发现,
  *           只要是在判断SPI_RXNE之前进入了断点或者是停止执行,都会导致其RXNE标志
  *           清零,原因是在调试模式下(非连续执行),会对SPI_DR寄存器进行读取,而读取
  *           DR寄存器会导致其RXNE标志位清零,最终导致其判断错误从而卡死.这个问题
  *           在读寄存器清零的情况下都会有可能出现,这一点值得注意.
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.04.18
  *
  * V4.1------------
  * 修改描述: 1.修改其SPI配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  * 
  * V4.2------------
  * 修改描述: 规范接口,作为GCP(General control platform)的硬件抽象层使用
  * 修改作者: Duhanfeng
  * 当前版本: V4.2
  * 修改日期: 2017.11.20
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "HW_Spi.h"
#include "HW_Gpio.h"
#include "HW_SysCtrl.h"
#include "chip.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//SPI节点定义
SPI_TypeDef * const SPI[HW_SPI_NODE_NUM] = { SPI1, SPI2, SPI3 };

//SPI 中断向量定义
static const IRQn_Type SPI_IRQn[HW_SPI_NODE_NUM] = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};

//SPI通道引脚
#define SPI1_NSS_PORT       (0 )
#define SPI1_NSS_PIN        (4 )
#define SPI1_CLK_PORT       (0 )
#define SPI1_CLK_PIN        (5 )
#define SPI1_MISO_PORT      (0 )
#define SPI1_MISO_PIN       (6 )
#define SPI1_MOSI_PORT      (0 )
#define SPI1_MOSI_PIN       (7 )

#define SPI2_NSS_PORT       (1 )
#define SPI2_NSS_PIN        (12)
#define SPI2_CLK_PORT       (1 )
#define SPI2_CLK_PIN        (13)
#define SPI2_MISO_PORT      (1 )
#define SPI2_MISO_PIN       (14)
#define SPI2_MOSI_PORT      (1 )
#define SPI2_MOSI_PIN       (15)

#define SPI3_NSS_PORT       (0 )
#define SPI3_NSS_PIN        (15)
#define SPI3_CLK_PORT       (1 )
#define SPI3_CLK_PIN        (3 )
#define SPI3_MISO_PORT      (1 )
#define SPI3_MISO_PIN       (4 )
#define SPI3_MOSI_PORT      (1 )
#define SPI3_MOSI_PIN       (5 )


/**
  * @brief  SPIx引脚配置
  * @param  uSpiNode SPI节点号
  * @param  uWorkMode 工作模式
  *     @arg HW_SPI_MASTER_MODE 主机模式
  *     @arg HW_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void HW_SPI_IOConfig(uint8_t uSpiNode, uint8_t uWorkMode)
{
  switch (uSpiNode)
  {
  case HW_SPI_NODE0: 
      {
        //HW_GPIO_ModeConfig(SPI1_NSS_PORT,  SPI1_NSS_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI1_CLK_PORT,  SPI1_CLK_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI1_MISO_PORT, SPI1_MISO_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_PUSH_UP : HW_GPIO_AF_PP_H);
          HW_GPIO_ModeConfig(SPI1_MOSI_PORT, SPI1_MOSI_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          break;
      }
      
  case HW_SPI_NODE1: 
      {
        //HW_GPIO_ModeConfig(SPI2_NSS_PORT,  SPI2_NSS_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI2_CLK_PORT,  SPI2_CLK_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI2_MISO_PORT, SPI2_MISO_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_PUSH_UP : HW_GPIO_AF_PP_H);
          HW_GPIO_ModeConfig(SPI2_MOSI_PORT, SPI2_MOSI_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          break;
      }
      
  case HW_SPI_NODE2: 
      {
        //HW_GPIO_ModeConfig(SPI3_NSS_PORT,  SPI3_NSS_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI3_CLK_PORT,  SPI3_CLK_PIN,  (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          HW_GPIO_ModeConfig(SPI3_MISO_PORT, SPI3_MISO_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_PUSH_UP : HW_GPIO_AF_PP_H);
          HW_GPIO_ModeConfig(SPI3_MOSI_PORT, SPI3_MOSI_PIN, (uWorkMode == HW_SPI_MASTER_MODE) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
          break;
      }
      
  default: break;
  }
  
}


/**
  * @brief  SPI模式配置
  * @param  uSpiNode SPI节点号
  * @param  uWorkMode 工作模式
  *     @arg HW_SPI_MASTER_MODE 主机模式
  *     @arg HW_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
static void HW_SPI_ModeConfig(uint8_t uSpiNode, uint8_t uWorkMode)
{
    //开时钟
    switch (uSpiNode)
    {
    case HW_SPI_NODE0: RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;
    case HW_SPI_NODE1: RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; break;
    case HW_SPI_NODE2: RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; break;
    default: break;
    }
    
    //模式配置
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_BIDIMODE; //双线双向模式(全双工)
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_CRCEN;    //硬件CRC校验,考虑可移植性,不开启
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_DFF;      //数据帧格式:8位
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_RXONLY;   //全双工模式配置
    SPI[uSpiNode]->CR1 |=  SPI_CR1_SSM;      //软件管理模式
    SPI[uSpiNode]->CR1 |=  SPI_CR1_SSI;
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_LSBFIRST; //数据帧:高位先发
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_BR;
    SPI[uSpiNode]->CR1 |=  SPI_CR1_BR;       //SPIx分频: 256分频(先配置为最低,再根据实际修改)
    SPI[uSpiNode]->CR1 |=  SPI_CR1_MSTR;     //配置为主机
    
    //从模式配置
    if (uWorkMode == HW_SPI_SLAVE_MODE)
    {
        SPI[uSpiNode]->CR1 &= ~SPI_CR1_SSI;         //内部NSS下拉
        SPI[uSpiNode]->CR1 &= ~SPI_CR1_MSTR;        //配置为从机
        
        SPI[uSpiNode]->CR2 |=  SPI_CR2_RXNEIE;      //开启接收中断
        
        HW_NVIC_Enable(SPI_IRQn[uSpiNode], 2, 2);  //开内核中断
        
        SPI[uSpiNode]->DR = SPI_DR_DR;              //清空DR寄存器
    }
    
    //配置通信时序
    SPI[uSpiNode]->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    
    //开启SPI
    SPI[uSpiNode]->CR1 |=  SPI_CR1_SPE;
    
}


/*****************************************************************************
 * SPI相关控制接口
 ****************************************************************************/

/**
  * @brief  SPI初始化
  * @param  uSpiNode SPI节点号
  * @param  uWorkMode 工作模式
  *     @arg HW_SPI_MASTER_MODE 主机模式
  *     @arg HW_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void HW_SPI_Init(uint8_t uSpiNode, uint8_t uWorkMode)
{
    //配置工作模式
    HW_SPI_ModeConfig(uSpiNode, uWorkMode);
    
    //配置引脚
    HW_SPI_IOConfig(uSpiNode, uWorkMode);
    
    //配置工作速度
    HW_SPI_SetSpeed(uSpiNode, HW_SPI_SPEED_DIV16);
    
}


/**
  * @brief  SPI 速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedDvi 分频系数
  *     @arg HW_SPI_SPEED_DIV2   //2分频(SPI1不可以2分频)
  *     @arg HW_SPI_SPEED_DIV4   //4分频
  *     @arg HW_SPI_SPEED_DIV8   //8分频
  *     @arg HW_SPI_SPEED_DIV16  //16分频
  *     @arg HW_SPI_SPEED_DIV32  //32分频
  *     @arg HW_SPI_SPEED_DIV64  //64分频
  *     @arg HW_SPI_SPEED_DIV128 //128分频
  *     @arg HW_SPI_SPEED_DIV256 //256分频
  * @retval None
  */
void HW_SPI_SetSpeed(uint8_t uSpiNode, uint8_t uSpeedDvi)
{
    //关闭SPIx
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_SPE;
    
    //配置SPIx速度
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_BR;
    SPI[uSpiNode]->CR1 |=  uSpeedDvi;   //SPIx分频
    
    //开启SPIx
    SPI[uSpiNode]->CR1 |=  SPI_CR1_SPE;
    
}


/**
  * @brief  SPI 状态寄存器获取
  * @param  uSpiNode SPI节点号
  * @retval SR的值
  */
uint16_t HW_SPI_GetStatus(uint8_t uSpiNode)
{
    
    return SPI[uSpiNode]->SR;
}


/**
  * @brief  SPIx数据寄存器写入
  * @param  Port SPI端口
  * @param  cWriteData 要写入的数据
  * @retval Note
  */
void HW_SPI_WriteData(uint8_t uSpiNode, uint16_t nWriteData)
{
    SPI[uSpiNode]->DR = nWriteData;
    
}


/**
  * @brief  SPIx数据寄存器读取
  * @param  Port SPI端口
  * @retval cReadData  读取到的数据
  */
uint16_t HW_SPI_ReadData(uint8_t uSpiNode)
{
    
    return SPI[uSpiNode]->DR;
}


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uint16_t HW_SPI_ReadWriteByte(uint8_t uSpiNode, uint16_t nWriteData)
{
    uint8_t cReadData = 0;
    
    //等待发送完成
    while (!(SPI[uSpiNode]->SR & SPI_SR_TXE));
    SPI[uSpiNode]->DR = nWriteData;
    
    //等待接收完成
    //值得注意的是,在仿真的时候如果在这里打断点(读RXNE状态位之前),则系统会将
    //卡死,因为仿真器会读取DR寄存器的值,从而令到RXNE状态位清零
    while (!(SPI[uSpiNode]->SR & SPI_SR_RXNE));
    cReadData = SPI[uSpiNode]->DR;
    
    return cReadData;
}



