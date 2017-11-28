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
  * @param  None
  * @retval None
  */
void HW_TIM_DelayInit(void);


/**
  * @brief  us级延时
  * @param  要延时的us数,取值范围为: 0xFFFF FFFF/0x18 ==> 约178S
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_TIM_DelayUs(uint32_t nus);


/**
  * @brief  ms级延时
  * @param  要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_TIM_DelayMs(uint32_t nms);


#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_DELAY_H */
