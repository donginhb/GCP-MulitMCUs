/**
  ******************************************************************************
  * @file    SPI.c
  * @author  Duhanfeng
  * @version V1.0 
  * @date    2018.01.25
  * @brief   SPI Application
  ******************************************************************************
  * @attention
  * 
  *
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "Spi.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Spi.h"

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
  *     @arg SPI_MASTER_MODE 主机模式
  *     @arg SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void SPI_Init(uBit8 uSpiNode, uBit8 uWorkMode)
{
    HAL_SPI_Init(uSpiNode, uWorkMode);
    
}


/**
  * @brief  SPI速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedGrade 速度等级
  *   @arg  SPI_SPEED_L 低速模式
  *   @arg  SPI_SPEED_M 中速模式
  *   @arg  SPI_SPEED_H 高速模式
  * @retval None
  */
void SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedGrade)
{
    HAL_SPI_SetSpeed(uSpiNode, uSpeedGrade);
    
}


/**
  * @brief  SPI数据读写
  * @param  uSpiNode SPI节点号
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit8 SPI_ReadWriteByte(uBit8 uSpiNode, uBit8 nWriteData)
{
    
    return HAL_SPI_ReadWriteByte(uSpiNode, nWriteData);
}

