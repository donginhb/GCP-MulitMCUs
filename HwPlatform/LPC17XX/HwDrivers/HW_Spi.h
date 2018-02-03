#ifndef  __HW_SPI_H
#define  __HW_SPI_H

#include "chip.h"

//SPI节点定义
#define HW_SPI_NODE_0      (0)
#define HW_SPI_NODE_1      (1)

//SPI分频系数定义
typedef enum 
{
    HW_SPI_SPEED_DIV2   = 0,
    HW_SPI_SPEED_DIV4  ,
    HW_SPI_SPEED_DIV8  ,
    HW_SPI_SPEED_DIV16 ,
    HW_SPI_SPEED_DIV32 ,
    HW_SPI_SPEED_DIV64 ,
    HW_SPI_SPEED_DIV128,
    HW_SPI_SPEED_DIV256,
    
}HW_SPI_SPEED_DIV;


#ifdef __cplusplus
extern "C" {
#endif
  

/*****************************************************************************
 * SPI相关控制接口
 ****************************************************************************/

/**
  * @brief  SPI初始化
  * @param  uSpiNode SPI节点号
  * @retval None
  */
void HW_SPI_Init(uint8_t uSpiNode);


/**
  * @brief  SPI速度配置
  * @param  uSpeedDvi 相关分频系数
  * @retval None
  */
void HW_SPI_SetSpeed(uint8_t uSpiNode, HW_SPI_SPEED_DIV SpeedDvi);


/**
  * @brief  SPI数据读写
  * @param  uSpiNode SPI节点号
  * @param  uWriteData 要写入的数据
  * @retval uReadData  读取到的数据
  */
uint8_t HW_SPI_ReadWriteByte(uint8_t uSpiNode, uint8_t uWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SPI_H */
