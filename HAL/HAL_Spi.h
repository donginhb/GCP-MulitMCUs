#ifndef  __HAL_SPI_H
#define  __HAL_SPI_H

#include "DataType/DataType.h"

//SPI工作模式
#define HAL_SPI_MASTER_MODE         (0)         //主机模式
#define HAL_SPI_SLAVE_MODE          (1)         //从机模式

//SPI速度定义
#define HAL_SPI_SPEED_L             (0)         //低速模式
#define HAL_SPI_SPEED_M             (1)         //中速模式
#define HAL_SPI_SPEED_H             (2)         //高速模式

#ifdef __cplusplus
extern "C" {
#endif
  

/*****************************************************************************
 * SPI相关控制接口
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
void HAL_SPI_Init(uBit8 uSpiNode, uBit8 uWorkMode);


/**
  * @brief  SPI 速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedGrade 速度等级
  *   @arg  HAL_SPI_SPEED_L 低速模式
  *   @arg  HAL_SPI_SPEED_M 中速模式
  *   @arg  HAL_SPI_SPEED_H 高速模式
  * @retval None
  */
void HAL_SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedGrade);


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit8 HAL_SPI_ReadWriteByte(uBit8 uSpiNode, uBit8 nWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_SPI_H */
