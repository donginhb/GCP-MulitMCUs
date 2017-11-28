#ifndef  __HAL_SPI_H
#define  __HAL_SPI_H

#include "DataType/DataType.h"

//SPI节点定义
typedef enum
{
    HAL_SPI_NODE0 = 0,
    HAL_SPI_NODE1,
    HAL_SPI_NODE2,
    HAL_SPI_NODE_NUM,
    
}HAL_SPI_NODE;


//SPI工作模式
#define HAL_SPI_MASTER_MODE         (0)         //主机模式
#define HAL_SPI_SLAVE_MODE          (1)         //从机模式

//SPI分频系数
#define HAL_SPI_SPEED_DIV2          (0x00)      //2分频(SPI1不可以2分频)
#define HAL_SPI_SPEED_DIV4          (0x08)      //4分频
#define HAL_SPI_SPEED_DIV8          (0x10)      //8分频
#define HAL_SPI_SPEED_DIV16         (0x18)      //16分频
#define HAL_SPI_SPEED_DIV32         (0x20)      //32分频
#define HAL_SPI_SPEED_DIV64         (0x28)      //64分频
#define HAL_SPI_SPEED_DIV128        (0x30)      //128分频
#define HAL_SPI_SPEED_DIV256        (0x38)      //256分频


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
  *     @arg HAL_SPI_MASTER_MODE 主机模式
  *     @arg HAL_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void HAL_SPI_Init(uBit8 uSpiNode, uBit8 uWorkMode);


/**
  * @brief  SPI 速度配置
  * @param  uSpiNode SPI节点号
  * @param  uSpeedDvi 分频系数
  *     @arg HAL_SPI_SPEED_DIV2   //2分频(SPI1不可以2分频)
  *     @arg HAL_SPI_SPEED_DIV4   //4分频
  *     @arg HAL_SPI_SPEED_DIV8   //8分频
  *     @arg HAL_SPI_SPEED_DIV16  //16分频
  *     @arg HAL_SPI_SPEED_DIV32  //32分频
  *     @arg HAL_SPI_SPEED_DIV64  //64分频
  *     @arg HAL_SPI_SPEED_DIV128 //128分频
  *     @arg HAL_SPI_SPEED_DIV256 //256分频
  * @retval None
  */
void HAL_SPI_SetSpeed(uBit8 uSpiNode, uBit8 uSpeedDvi);


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uBit16 HAL_SPI_ReadWriteByte(uBit8 uSpiNode, uBit16 nWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_SPI_H */
