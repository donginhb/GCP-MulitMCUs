#ifndef  __HW_TIM_DELAY_H
#define  __HW_TIM_DELAY_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * RIT延时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器初始化
  * @param  None
  * @retval None
  */
void HW_RIT_InitDelayMode(void);


/**
  * @brief  us级延时
  * @param  ulUs 要延时的us数,取值范围为: 0xFFFF FFFF/0x18 ==> 约178S
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_RIT_DelayUs(uint32_t ulUs);


/**
  * @brief  ms级延时
  * @param  ulMs 要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_RIT_DelayMs(uint32_t ulMs);


/*****************************************************************************
 * RIT计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  None
  * @retval None
  */
void HW_RIT_InitReckonMode(void);


/**
  * @brief  计时开始
  * @param  None
  * @retval None
  */
void HW_RIT_StartReckon(void);


/**
  * @brief  计时停止
  * @param  None
  * @retval None
  */
void HW_RIT_StopReckon(void);


/**
  * @brief  计时值获取
  * @param  None
  * @retval 计时值(单位: us)
  */
uint32_t HW_RIT_GetReckonValue(void);


/**
  * @brief  计时值清零
  * @param  None
  * @retval None
  */
void HW_RIT_ClearRecvkonvalue(void);



#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_DELAY_H */
