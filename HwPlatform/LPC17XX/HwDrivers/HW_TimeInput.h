#ifndef  __HW_TIME_INPUT_H
#define  __HW_TIME_INPUT_H

#include "chip.h"


//定时器节点定义
#define HW_TIME_NODE_0      (0)
#define HW_TIME_NODE_1      (1)
#define HW_TIME_NODE_2      (2)
#define HW_TIME_NODE_3      (3)


#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * 定时器输入捕获相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式(普通/PWM输入/编码器模式)
  * @retval None
  */
void HW_TIM_InputInit(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uInputMode);


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uint16_t HW_TIM_GetInputCount(uint8_t uTimeNode);


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @retval 定时器通道捕获寄存器值
  */
uint16_t HW_TIM_GetInputCapValue(uint8_t uTimeNode, uint8_t uChannelMask);


/*****************************************************************************
 * 定时器输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableInputIRQ(uint8_t uTimeNode, uint8_t uChannelNum, bool bIsEnable);


/**
  * @brief  定时器输入中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HW_TIM_GetInputITStatus(uint8_t uTimeNode, uint8_t uChannelNum);


/**
  * @brief  定时器输入中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HW_TIM_ClearInputITStatus(uint8_t uTimeNode, uint8_t uChannelNum);


#ifdef __cplusplus
}
#endif


#endif /* __HW_TIME_INPUT_H */
