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
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "HAL_Spi.h"
#include "DataType/DataType.h"

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

    
}


/**
  * @brief  SPI 速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedDvi 分频系数
  * @retval None
  */
void HAL_SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedDvi)
{
    
    
}


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit16 HAL_SPI_ReadWriteByte(uBit8 uSpiNode, uBit16 nWriteData)
{
    
    
    return 0;
}



