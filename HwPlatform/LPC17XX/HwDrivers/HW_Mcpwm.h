#ifndef  __HW_MCPWM_H
#define  __HW_MCPWM_H

#include "chip.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
//MCPWM 节点定义
#define HW_MCPWM_NODE_0             (0)
#define HW_MCPWM_NODE_1             (1)
#define HW_MCPWM_NODE_2             (2)
    
//MCPWM 中断模式定义
#define HW_MCPWM_INT_LIMIT          (0x01)  //限制中断
#define HW_MCPWM_INT_MATCH          (0x02)  //匹配中断
#define HW_MCPWM_INT_CAPTURE        (0x04)  //捕获中断
    
//MCPWM 输入触发边沿
#define HW_MCPWM_INPUT_TRG_R        (1)     //上升沿触发
#define HW_MCPWM_INPUT_TRG_F        (2)     //下降沿触发
#define HW_MCPWM_INPUT_TRG_R_F      (3)     //双边沿触发


/*****************************************************************************
 * MCPWM 输出相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM输出模式初始化
  * @param  uMcpwmNode MCPWM节点
  * @param  bOppositeOutput 使能反相输出通道
  * @retval 0-成功 非0-失败
  * @note   调用此初始化函数不会立即使能输出,在调用使能接口才会产生输出
  */
uint32_t HW_MCPWM_InitOutput(uint8_t uMcpwmNode, bool bOppositeOutput);


/**
  * @brief  MCPWM输出占空比设置
  * @param  uMcpwmNode MCPWM节点
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_MCPWM_SetOutputPwmDutyRatio(uint8_t uMcpwmNode, float fDutyRatio);


/**
  * @brief  MCPWM输出频率设置
  * @param  uMcpwmNode MCPWM节点
  * @param  ulFrequency  PWM频率(单位:Hz)
  * @retval None
  * @note   若没有使能更新,则在MCPWM运行期间,不会改变其频率
  */
void HW_MCPWM_SetOutputPwmFrq(uint8_t uMcpwmNode, uint32_t ulFrequency);


/**
  * @brief  MCPWM输出使能
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableOutput(uint8_t uMcpwmNode, bool bIsEnablle);


/*****************************************************************************
 * MCPWM 输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM输入计数模式初始化
  * @param  uMcpwmNode MCPWM节点
  * @param  uInputSource 输入源
  * @param  uTrgSource 触发源
  *   @arg  HW_MCPWM_INPUT_TRG_R  上升沿触发
  *   @arg  HW_MCPWM_INPUT_TRG_F  下降沿触发
  *   @arg  HW_MCPWM_INPUT_TRG_R_F 双边沿触发
  * @retval 0-成功 非0-失败
  * @note   调用此初始化函数不会立即使能计数,在调用使能接口才会使能计数
  */
uint32_t HW_MCPWM_InitInputCount(uint8_t uMcpwmNode, uint8_t uInputSource, uint8_t uTrgSource);


/**
  * @brief  MCPWM计数使能
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableInputCount(uint8_t uMcpwmNode, bool bIsEnablle);


/**
  * @brief  MCPWM最大输入计数设置
  * @param  uMcpwmNode MCPWM节点
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位;
  */
void HW_MCPWM_SetMaxInputCount(uint8_t uMcpwmNode, uint32_t ulMaxCount);


/**
  * @brief  MCPWM当前计数值获取
  * @param  uMcpwmNode MCPWM节点
  * @retval 当前计数值
  */
uint32_t HW_MCPWM_GetCurInputCount(uint8_t uMcpwmNode);


/*****************************************************************************
 * MCPWM 中断相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM计数使能
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableIRQ(uint8_t uMcpwmNode, uint8_t uIntModeMask, bool bIsEnable);


/**
  * @brief  MCPWM中断状态获取
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntMode 中断模式(仅能获取一种中断的标志)
  * @retval None
  */
bool HW_MCPWM_GetITStatus(uint8_t uMcpwmNode, uint8_t uIntMode);


/**
  * @brief  MCPWM中断状态清除
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @retval None
  */
void HW_MCPWM_ClearITStatus(uint8_t uMcpwmNode, uint8_t uIntModeMask);



#ifdef __cplusplus
}
#endif

#endif /* __HW_MCPWM_H */
