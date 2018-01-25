#ifndef  __SPI_H
#define  __SPI_H

#include "DataType/DataType.h"
#include "HAL/HAL_Spi.h"


//SPI工作模式
#define SPI_MASTER_MODE         (HAL_SPI_MASTER_MODE)       //主机模式
#define SPI_SLAVE_MODE          (HAL_SPI_SLAVE_MODE )       //从机模式

//SPI速度定义
#define SPI_SPEED_L             (HAL_SPI_SPEED_L)           //低速模式
#define SPI_SPEED_M             (HAL_SPI_SPEED_M)           //中速模式
#define SPI_SPEED_H             (HAL_SPI_SPEED_H)           //高速模式


typedef enum
{
    //SPI节点定义
    SPI_NODE_0 = 0,
    SPI_NODE_1,
    SPI_NODE_2,
    SPI_NODE_3,
    SPI_NODE_4,
    SPI_NODE_5,
    
}SPI_NODE;


#ifdef __cplusplus
extern "C" {
#endif
  
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
void SPI_Init(uBit8 uSpiNode, uBit8 uWorkMode);


/**
  * @brief  SPI速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedGrade 速度等级
  *   @arg  SPI_SPEED_L 低速模式
  *   @arg  SPI_SPEED_M 中速模式
  *   @arg  SPI_SPEED_H 高速模式
  * @retval None
  */
void SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedGrade);


/**
  * @brief  SPI数据读写
  * @param  uSpiNode SPI节点号
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit8 SPI_ReadWriteByte(uBit8 uSpiNode, uBit8 nWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */
