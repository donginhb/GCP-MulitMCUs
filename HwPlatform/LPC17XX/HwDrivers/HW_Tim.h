#ifndef  __HW_TIM_H
#define  __HW_TIM_H

#include "chip.h"

//定时器节点定义
#define HW_TIME_NODE_0              (0)
#define HW_TIME_NODE_1              (1)
#define HW_TIME_NODE_2              (2)
#define HW_TIME_NODE_3              (3)


//定时器中断模式定义
#define HW_TIM_INT_MATCH            (0x01)  //匹配中断
#define HW_TIM_INT_CAPTURE          (0x02)  //捕获中断

//定时器捕获边沿定义
#define HW_TIM_CAP_EDGE_RISING      (0x01)  //上升沿捕获
#define HW_TIM_CAP_EDGE_FALLING     (0x02)  //下降沿捕获
#define HW_TIM_CAP_EDGE_BOTH        (0x03)  //双边沿捕获

#ifdef __cplusplus
extern "C"
{
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
  * @note   初始化后,默认已经打开定时器通道,但是尚未启动定时器
  */
void HW_TIM_InitOutput(uint8_t uTimeNode, uint8_t uChannelMask);


/**
  * @brief  定时器通道使能设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HW_TIM_EnableChannel(uint8_t uTimeNode, uint8_t uChannelMask, bool bIsEnble);


/**
  * @brief  定时器输出中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableOutputIRQ(uint8_t uTimeNode, uint8_t uChannelMask, bool bIsEnable);


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


/*****************************************************************************
 * 定时器输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入计数初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HW_TIM_InitInputCount(uint8_t uTimeNode, uint8_t uChannelMask);


/**
  * @brief  定时器计数使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableInputCount(uint8_t uTimeNode, bool bIsEnable);


/**
  * @brief  定时器最大输入计数设置
  * @param  uTimeNode 定时器节点
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位;若设置多个通道的"最大计数值",以最小的计数值为准;
  */
void HW_TIM_SetMaxInputCount(uint8_t uTimeNode, uint8_t uChannelNum, uint32_t ulMaxCount);


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 当前计数值
  */
uint32_t HW_TIM_GetCurInputCount(uint8_t uTimeNode);


/*****************************************************************************
 * 定时器输入捕获相关控制接口
 ****************************************************************************/

/**
  * @brief  捕获模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval 0-成功 非0-失败
  */
void HW_TIM_InitCapture(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uTrgSource);


/**
  * @brief  捕获捕获边缘设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval None
  */
void HW_TIM_SetCaptureEdge(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uTrgSource);


/**
  * @brief  定时器输入使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HW_TIM_EnableCapture(uint8_t uTimeNode, bool bIsEnable);


/**
  * @brief  计数器复位
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HW_TIM_ResetCounter(uint8_t uTimeNode);


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uint32_t HW_TIM_GetCaptureValue(uint8_t uTimeNode, uint8_t uChannelNum);


/*****************************************************************************
 * 定时器输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM计数使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableIRQ(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntModeMask, bool bIsEnable);


/**
  * @brief  MCPWM中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uIntMode 中断模式(仅能获取一种中断的标志)
  * @retval None
  */
bool HW_TIM_GetITStatus(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntMode);


/**
  * @brief  MCPWM中断状态清除
  * @param  uTimeNode 定时器节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @retval None
  */
void HW_TIM_ClearITStatus(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntModeMask);


#ifdef __cplusplus
}
#endif

#endif /* __HW_TIM_H */
