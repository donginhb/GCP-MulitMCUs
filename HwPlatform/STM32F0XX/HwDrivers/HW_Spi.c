/**
  ******************************************************************************
  * @file    SPI.c
  * @author  杜公子寒枫
  * @version V2.0 寄存器版本
  * @date    2016.08.11
  * @brief   SPI实现文件
  ******************************************************************************
  * @attention
  * 
  *
  * SPI:   NSS  --- PA4   通用推挽,10M
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
  * V2.0------------
  * 修改描述: 修复配置速度函数无效的错误
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPI->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
  *
  * V3.0------------
  * 修改描述: 修改底层实现机制,将本文件匹配SPI~3
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPI->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
  *
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_spi.h"
#include "HW_Gpio.h"
#include "chip.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//SPI数量定义
#define HW_SPI_COUNT       (1)

static SPI_TypeDef * const SPI[HW_SPI_COUNT] = {(SPI_TypeDef *)SPI1_BASE};    //串口的基地址


/**
  * @brief  SPI引脚配置
  * @param  uSpiNode SPI节点号
  * @retval None
  */
static void HW_SPI_IOConfig(uint8_t uSpiNode)
{
    switch (uSpiNode)
    {
    case HW_SPI_NODE_0:
        
        //配置引脚
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_5, HW_GPIO_AF_PP_H);
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_6, HW_GPIO_AF_PP_H);
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_7, HW_GPIO_AF_PP_H);
        
        //设置复用模式
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_5, 0);
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_6, 0);
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_7, 0);
        
        break;
        
    case HW_SPI_NODE_1:
        
        //配置引脚
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_5, HW_GPIO_AF_PP_H);
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_6, HW_GPIO_AF_PP_H);
        HW_GPIO_ModeConfig(HW_GPIO_PORT_A, HW_GPIO_PIN_7, HW_GPIO_AF_PP_H);
        
        //设置复用模式
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_5, 0);
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_6, 0);
        HW_GPIO_SetAFMode(HW_GPIO_PORT_A, HW_GPIO_PIN_7, 0);
        
        break;
    }
    
}


/**
  * @brief  SPI模式配置
  * @param  uSpiNode SPI节点号
  * @retval None
  */
static void HW_SPI_ConfigMode(uint8_t uSpiNode)
{
    //开时钟 
    RCC->APB2ENR |= (0X1<<12); //SPI[uSpiNode]
    
#if 0
    //配置工作模式 
    SPI[uSpiNode]->CR1 &= ~(0X1<<15);  //双线单向模式(全双工)
    SPI[uSpiNode]->CR1 &= ~(0X1<<13);  //硬件CRC校验,考虑可移植性,不开启
    //  SPI[uSpiNode]->CR1 &= ~(0X1<<11);  //数据帧格式:8位
    SPI[uSpiNode]->CR1 &= ~(0X1<<10);  //全双工模式配置
    SPI[uSpiNode]->CR1 |=  (0X1<<9);   //软件管理模式
    SPI[uSpiNode]->CR1 |=  (0X1<<8);   //
    SPI[uSpiNode]->CR1 &= ~(0X1<<7);   //数据帧:高位先发
    SPI[uSpiNode]->CR1 &= ~(0X7<<3);
    SPI[uSpiNode]->CR1 |=  (0X4<<3);   //SPI分频: 256分频(先配置为最低,再根据实际修改)
    SPI[uSpiNode]->CR1 |=  (0X1<<2);   //配置为主机
    
    //配置通信时序 
    SPI[uSpiNode]->CR1 |=  (0X1<<1);   //空闲时SCK为高电平
    SPI[uSpiNode]->CR1 |=  (0X1<<0);   //数据采集从第二个时钟沿开始(时序:模式3)
    
	//数据位宽 
	SPI[uSpiNode]->CR2 &= ~(0xF<<8);
	SPI[uSpiNode]->CR2 |=  (0x7<<8);  //8位
	
    //FIFO接收门限 
    //	SPI[uSpiNode]->CR2  &= ~(0X1<<12);  //8位
    SPI[uSpiNode]->CR2  |=  (0X1<<12);  //8位
	
    //开启SPI 
    SPI[uSpiNode]->CR1 |=  (0X1<<6);
    
#else 
    
    SPI[uSpiNode]->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; //(1) 
    SPI[uSpiNode]->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_FRXTH
        | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; //(2) 
    SPI[uSpiNode]->CR1 |= SPI_CR1_SPE; //(3) 
    
#endif
	
}


/*****************************************************************************
 * SPI相关控制接口
 ****************************************************************************/

/**
  * @brief  SPI初始化
  * @param  uSpiNode SPI节点号
  * @retval None
  */
void HW_SPI_Init(uint8_t uSpiNode)
{
    //配置IO
    HW_SPI_IOConfig(uSpiNode);
    
    //配置SPI工作模式
    HW_SPI_ConfigMode(uSpiNode);
    
}


/**
  * @brief  SPI速度配置
  * @param  uSpeedDvi 相关分频系数
  * @retval None
  */
void HW_SPI_SetSpeed(uint8_t uSpiNode, HW_SPI_SPEED_DIV SpeedDvi)
{
    //关闭SPI 
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_SPE;
    
    //配置SPI速度 
    SPI[uSpiNode]->CR1 &= ~SPI_CR1_BR;
    SPI[uSpiNode]->CR1 |=  (SpeedDvi<<3);   //SPI分频
    
    //开启SPI 
    SPI[uSpiNode]->CR1 |= SPI_CR1_SPE;
}


/**
  * @brief  SPI数据读写
  * @param  uSpiNode SPI节点号
  * @param  uWriteData 要写入的数据
  * @retval uRecvData  读取到的数据
  */
uint8_t HW_SPI_ReadWriteByte(uint8_t uSpiNode, uint8_t uWriteData)
{
    uint8_t uRecvData = 0;
	
    while ((SPI[uSpiNode]->SR & SPI_SR_TXE) != SPI_SR_TXE); //Test Tx empty 
    
    //Will inititiate 8-bit transmission if TXE 
    *(uint8_t *)&(SPI[uSpiNode]->DR) = uWriteData;
    
    while ((SPI[uSpiNode]->SR & SPI_SR_RXNE) != SPI_SR_RXNE); //Test Tx empty 
    
    //Will inititiate 8-bit transmission if TXE 
    uRecvData = (uint8_t)SPI[uSpiNode]->DR;
    
    return uRecvData;
}



