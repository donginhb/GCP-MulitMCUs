#ifndef  __HW_SPI_H
#define  __HW_SPI_H

#include "chip.h"

//SPI节点定义
typedef enum
{
    HW_SPI_NODE0 = 0,
    HW_SPI_NODE1,
    HW_SPI_NODE2,
    HW_SPI_NODE_NUM,
    
}HW_SPI_NODE;


//SPI工作模式
#define HW_SPI_MASTER_MODE         (0)         //主机模式
#define HW_SPI_SLAVE_MODE          (1)         //从机模式

//SPI分频系数
#define HW_SPI_SPEED_DIV2          (0x00)      //2分频(SPI1不可以2分频)
#define HW_SPI_SPEED_DIV4          (0x08)      //4分频
#define HW_SPI_SPEED_DIV8          (0x10)      //8分频
#define HW_SPI_SPEED_DIV16         (0x18)      //16分频
#define HW_SPI_SPEED_DIV32         (0x20)      //32分频
#define HW_SPI_SPEED_DIV64         (0x28)      //64分频
#define HW_SPI_SPEED_DIV128        (0x30)      //128分频
#define HW_SPI_SPEED_DIV256        (0x38)      //256分频


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
  *     @arg HW_SPI_MASTER_MODE 主机模式
  *     @arg HW_SPI_SLAVE_MODE  从机模式
  * @retval None
  */
void HW_SPI_Init(uint8_t uSpiNode, uint8_t uWorkMode);


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
void HW_SPI_SetSpeed(uint8_t uSpiNode, uint8_t uSpeedDvi);


/**
  * @brief  SPIx数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
uint16_t HW_SPI_ReadWriteByte(uint8_t uSpiNode, uint16_t nWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SPI_H */
