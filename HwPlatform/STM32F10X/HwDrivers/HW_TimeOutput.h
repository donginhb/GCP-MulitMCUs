#ifndef  __HW_TIME_OUTPUT_H
#define  __HW_TIME_OUTPUT_H

#include "chip.h"

//定时器输入模式定义
#define TIM_OUTPUT_MODE_COMPARE             (0)         //正常捕获模式
#define TIM_OUTPUT_MODE_PWM1                (1)         //正常PWM捕获模式
#define TIM_OUTPUT_MODE_PWM2                (2)         //反相PWM捕获模式

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 定时器输出相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输出模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uOutputCfgCode 工作模式
  * @retval None
  */
void HW_TIM_InitOutput(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uOutputMode);


/**
  * @brief  定时器输出中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableOutputIRQ(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t bIsEnable);


/**
  * @brief  定时器输出PWM占空比设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 定时器通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_TIM_SetOutputPwmDutyRatio(uint8_t uTimeNode, uint8_t uChannelMask, float fDutyRatio);


/**
  * @brief  定时器输出比较值设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @param  nCompareVal 比较值
  * @retval None
  */
void HW_TIM_SetOutputCmpVal(uint8_t uTimeNode, uint8_t uChannelMask, uint16_t nCompareVal);


/**
  * @brief  定时器输出PWM频率设置
  * @param  uTimeNode 定时器节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HW_TIM_SetOutputPwmFrq(uint8_t uTimeNode, uint32_t ulFrequency);


/**
  * @brief  定时器输出自动重加载值设置
  * @param  nReloadVal 重加载值
  * @retval None
  */
void HW_TIM_SetOutputAutoReloadValue(uint8_t uTimeNode, uint16_t nReloadVal);


/**
  * @brief  定时器输出自动重加载值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器中的重加载值
  */
uint16_t HW_TIM_GetOutputAutoReloadValue(uint8_t uTimeNode);


/**
  * @brief  定时器输出使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HW_TIM_EnableOutput(uint8_t uTimeNode, bool bIsEnable);

  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIME_OUTPUT_H */
