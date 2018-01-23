#ifndef  __HW_TIM_DELAY_H
#define  __HW_TIM_DELAY_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 延时功能定时器接口函数
 ****************************************************************************/

/**
  * @brief  定时器初始化
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HW_TIM_DelayInit(uint8_t uTimeNode);


/**
  * @brief  us级延时
  * @param  uTimeNode 定时器节点
  * @param  要延时的us数,取值范围为: [0-65535]
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_TIM_DelayUs(uint8_t uTimeNode, uint32_t ulUs);


/**
  * @brief  ms级延时
  * @param  uTimeNode 定时器节点
  * @param  要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_TIM_DelayMs(uint8_t uTimeNode, uint32_t ulMs);


#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_DELAY_H */
