#ifndef  __PWM_H
#define  __PWM_H

#include "DataType/DataType.h"
#include "HAL/HAL_Pwm.h"


//PWM节点定义
#define PWM_NODE_0      (HAL_PWM_NODE_0)
#define PWM_NODE_1      (HAL_PWM_NODE_1)
#define PWM_NODE_2      (HAL_PWM_NODE_2)
#define PWM_NODE_3      (HAL_PWM_NODE_3)
#define PWM_NODE_4      (HAL_PWM_NODE_4)
#define PWM_NODE_5      (HAL_PWM_NODE_5)
#define PWM_NODE_6      (HAL_PWM_NODE_6)
#define PWM_NODE_7      (HAL_PWM_NODE_7)


#ifdef __cplusplus
extern "C"
{
#endif


/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void PWM_Init(uBit8 uPwmNode, uBit8 uChannelMask);


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void PWM_EnableChannel(uBit8 uPwmNode, uBit8 uChannelMask, bool bIsEnble);


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void PWM_SetOutputPwmDutyRatio(uBit8 uPwmNode, uBit8 uChannelMask, float fDutyRatio);


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void PWM_SetOutputPwmFrq(uBit8 uPwmNode, uBit32 ulFrequency);


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnable PWM使能位
  * @retval None
  */
void PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnable);


#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */
