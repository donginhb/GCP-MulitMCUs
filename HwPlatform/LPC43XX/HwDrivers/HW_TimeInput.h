#ifndef  __HW_TIME_INPUT_H
#define  __HW_TIME_INPUT_H

#include "chip.h"

//定时器节点定义
#define HW_TIME_NODE_0      (0)
#define HW_TIME_NODE_1      (1)
#define HW_TIME_NODE_2      (2)
#define HW_TIME_NODE_3      (3)

//定时器输入模式定义
#define HW_TIME_INPUT_COUNT_MODE    (0) //输入计数模式,计数模式关心的是脉冲的数量
#define HW_TIME_INPUT_CAPTURE_MODE  (1) //输入捕获模式,捕获模式关心的是单个脉冲的时序

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * 定时器输入相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void HW_TIM_InputInit(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uInputMode);


/**
  * @brief  定时器使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnablle 定时器使能位
  * @retval None
  */
void HW_TIM_InputEnable(uint8_t uTimeNode, bool bIsEnablle);


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uint16_t HW_TIM_GetInputCount(uint8_t uTimeNode);


/*****************************************************************************
 * 定时器输入捕获模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uint16_t HW_TIM_GetInputCapValue(uint8_t uTimeNode, uint8_t uChannelNum);


/*****************************************************************************
 * 定时器输入计数模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器最大输入计数设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void HW_TIM_SetMaxInputCount(uint8_t uTimeNode, uint8_t uChannelNum, uint32_t ulMaxCount);


/*****************************************************************************
 * 定时器输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
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
