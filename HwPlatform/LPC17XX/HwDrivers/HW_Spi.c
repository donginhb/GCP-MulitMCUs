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
        
        
        break;
        
    case HW_SPI_NODE_1:
        
        
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
    
    return uRecvData;
}



