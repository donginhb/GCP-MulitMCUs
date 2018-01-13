#ifndef  __HAL_PWM_H
#define  __HAL_PWM_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


//PWM节点定义
#define HAL_PWM_NODE_0      ( 0)
#define HAL_PWM_NODE_1      ( 1)
#define HAL_PWM_NODE_2      ( 2)
#define HAL_PWM_NODE_3      ( 3)
#define HAL_PWM_NODE_4      ( 4)
#define HAL_PWM_NODE_5      ( 5)
#define HAL_PWM_NODE_6      ( 6)
#define HAL_PWM_NODE_7      ( 7)
#define HAL_PWM_NODE_8      ( 8)
#define HAL_PWM_NODE_9      ( 9)
#define HAL_PWM_NODE_10     (10)
#define HAL_PWM_NODE_11     (11)
#define HAL_PWM_NODE_12     (12)

    

/*****************************************************************************
 * PWM相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HAL_PWM_Init(uBit8 uPwmNode, uBit8 uChannelMask);


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HAL_PWM_EnableChannel(uBit8 uPwmNode, uBit8 uChannelMask, bool bIsEnble);


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HAL_PWM_SetOutputPwmDutyRatio(uBit8 uPwmNode, uBit8 uChannelMask, float fDutyRatio);


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HAL_PWM_SetOutputPwmFrq(uBit8 uPwmNode, uBit32 ulFrequency);


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void HAL_PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnablle);



#ifdef __cplusplus
}
#endif

#endif /* __HAL_PWM_H */
