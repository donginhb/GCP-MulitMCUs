#ifndef  __HW_PWM_H
#define  __HW_PWM_H

#include "chip.h"

//PWM节点定义
typedef enum
{
    HW_PWM_NODE0 = 0,
    HW_PWM_NODE_NUM,
    
}HW_PWM_NODE;

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * PWM相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HW_PWM_Init(uint8_t uPwmNode, uint8_t uChannelMask);


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HW_PWM_EnableChannel(uint8_t uPwmNode, uint8_t uChannelMask, bool bIsEnble);


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_PWM_SetOutputPwmDutyRatio(uint8_t uPwmNode, uint8_t uChannelMask, float fDutyRatio);


/**
  * @brief  PWM输出比较值设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码
  * @param  nCompareVal 比较值
  * @retval None
  */
void HW_PWM_SetOutputCmpVal(uint8_t uPwmNode, uint8_t uChannelMask, uint16_t nCompareVal);


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HW_PWM_SetOutputPwmFrq(uint8_t uPwmNode, uint32_t ulFrequency);


/**
  * @brief  PWM输出自动重加载值设置
  * @param  nReloadVal 重加载值
  * @retval None
  */
void HW_PWM_SetOutputAutoReloadValue(uint8_t uPwmNode, uint16_t nReloadVal);


/**
  * @brief  PWM输出自动重加载值获取
  * @param  uPwmNode PWM节点
  * @retval PWM中的重加载值
  */
uint16_t HW_PWM_GetOutputAutoReloadValue(uint8_t uPwmNode);


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void HW_PWM_OutputEnable(uint8_t uPwmNode, bool bIsEnablle);


#ifdef __cplusplus
}
#endif

#endif /* __HW_PWM_H */
