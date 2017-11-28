#ifndef  __HAL_DELAY_H
#define  __HAL_DELAY_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 定时器延时相关控制接口
 ****************************************************************************/

/**
  * @brief  延时定时器初始化
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_DelayInit(uBit8 uTimeNode);


/**
  * @brief  us级延时
  * @param  要延时的us数
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HAL_DelayUs(uBit32 ulUs);


/**
  * @brief  us级延时
  * @param  要延时的us数
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HAL_DelayMs(uBit32 ulMs);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_DELAY_H */
