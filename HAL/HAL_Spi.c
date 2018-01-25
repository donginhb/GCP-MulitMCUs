/**
  ******************************************************************************
  * @file    HAL_Spi.c
  * @author  Duhanfeng
  * @version V1.0
  * @date    2018.01.25
  * @brief   Hal spi drivers
  ******************************************************************************
  * @attention
  * 
  *
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "HAL_Spi.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_Spi.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * SPI相关控制接口
 ****************************************************************************/

/**
  * @brief  SPI初始化
  * @param  uSpiNode SPI节点号
  * @param  uWorkMode 工作模式
  *     @arg HAL_SPI_MASTER_MODE 主机模式
  *     @arg HAL_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void HAL_SPI_Init(uBit8 uSpiNode, uBit8 uWorkMode)
{
#if defined(LPC17XX)
    

#elif defined(LPC43XX)
    

#elif defined(STM32F10X)
    HW_SPI_Init(uSpiNode, uWorkMode);
    
#elif defined(STM32F0XX)
    HW_SPI_Init(uSpiNode);
    
#endif

}


/**
  * @brief  SPI 速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedGrade 速度等级
  *   @arg  HAL_SPI_SPEED_L 低速模式
  *   @arg  HAL_SPI_SPEED_M 中速模式
  *   @arg  HAL_SPI_SPEED_H 高速模式
  * @retval None
  */
void HAL_SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedGrade)
{
#if defined(LPC17XX)
    

#elif defined(LPC43XX)
    

#elif defined(STM32F10X)
    
    switch (uSpeedGrade)
    {
    case HAL_SPI_SPEED_L: 
        HW_SPI_SetSpeed(uSpiNode, HW_SPI_SPEED_DIV256);
        break;
    case HAL_SPI_SPEED_M: 
        HW_SPI_SetSpeed(uSpiNode, HW_SPI_SPEED_DIV16);
        break;
    case HAL_SPI_SPEED_H: 
        HW_SPI_SetSpeed(uSpiNode, (uSpiNode == 0) ? HW_SPI_SPEED_DIV4 : HW_SPI_SPEED_DIV2);
        break;
    default: break;
    }
    
#elif defined(STM32F0XX)
    
    switch (uSpeedGrade)
    {
    case HAL_SPI_SPEED_L: 
        HW_SPI_SetSpeed(uSpiNode, HW_SPI_SPEED_DIV256);
        break;
    case HAL_SPI_SPEED_M: 
        HW_SPI_SetSpeed(uSpiNode, HW_SPI_SPEED_DIV16);
        break;
    case HAL_SPI_SPEED_H: 
        HW_SPI_SetSpeed(uSpiNode, (uSpiNode == 0) ? HW_SPI_SPEED_DIV4 : HW_SPI_SPEED_DIV2);
        break;
    default: break;
    }
    
#endif
    
}


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit8 HAL_SPI_ReadWriteByte(uBit8 uSpiNode, uBit8 nWriteData)
{
    uBit8 uRecvData = 0;
    
#if defined(LPC17XX)
    

#elif defined(LPC43XX)
    

#elif defined(STM32F10X)
    uRecvData = HW_SPI_ReadWriteByte(uSpiNode, nWriteData);
    
#elif defined(STM32F0XX)
    uRecvData = HW_SPI_ReadWriteByte(uSpiNode, nWriteData);
    
#endif
    
    return uRecvData;
}
