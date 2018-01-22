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

    

/*****************************************************************************
 * PWM 输出相关控制接口
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
  * @param  bIsEnable PWM使能位
  * @retval None
  */
void HAL_PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnable);


/*****************************************************************************
 * PWM 输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入计数初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void HAL_PWM_InitInputCount(uBit8 uPwmNode, uBit8 uChannelMask);


/**
  * @brief  定时器使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HAL_PWM_EnableInputCount(uBit8 uPwmNode, bool bIsEnable);


/**
  * @brief  定时器计数值获取
  * @param  uPwmNode PWM节点
  * @retval 定时器计数寄存器值
  */
uBit32 HAL_PWM_GetInputCount(uBit8 uPwmNode);


/**
  * @brief  定时器最大输入计数设置
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void HAL_PWM_SetMaxInputCount(uBit8 uPwmNode, uBit8 uChannelNum, uBit32 ulMaxCount);


/**
  * @brief  定时器输入中断使能
  * @param  uPwmNode PWM节点
  * @retval 中断触发源
  */
uBit32 HAL_PWM_GetIRQTrgSource(uBit8 uPwmNode);


/**
  * @brief  定时器输入中断使能
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HAL_PWM_EnableInputCountIRQ(uBit8 uPwmNode, uBit8 uChannelNum, bool bIsEnable);


/**
  * @brief  定时器输入中断状态获取
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HAL_PWM_GetInputCountITStatus(uBit8 uPwmNode, uBit8 uChannelNum);


/**
  * @brief  定时器输入中断标志清除
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_PWM_ClearInputCountITStatus(uBit8 uPwmNode, uBit8 uChannelNum);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_PWM_H */
