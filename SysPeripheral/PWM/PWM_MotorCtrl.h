#ifndef  __PWM_MOTOR_CTRL_H
#define  __PWM_MOTOR_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    
/*****************************************************************************
 * PWM输入相关控制接口
 ****************************************************************************/

/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @param  ulOutChNum 输出通道编号
  * @param  uPwmCountNode  PWM计数节点
  * @param  ulCountChNum 计数通道编号
  * @retval 0-成功 非0-失败
  */
uBit32 PWM_MotorPulseInit(uBit8 uPwmOutputNode, uBit32 ulOutChNum, 
                        uBit8 uPwmCountNode, uBit32 ulCountChNum);


/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @param  ulPulseCount 脉冲数量
  * @retval 0-成功 非0-失败
  */
void PWM_SendPulse(uBit8 uPwmOutputNode, uBit32 ulPulseCount);
                          

/**
  * @brief  在1MS内产生指定数量的脉冲
  * @param  uPwmOutputNode PWM节点
  * @param  ulPulseCount 脉冲数量
  * @retval 0-成功 非0-失败
  */
void PWM_SendPulseLimitMs(uBit8 uPwmOutputNode, uBit32 ulPulseCount);


/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @retval 当前记录到的脉冲数
  */
uBit32 PWM_GetCount(uBit8 uPwmOutputNode);


/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @retval true-运行中 false-停止运行
  */
bool PWM_GetRunningState(uBit8 uPwmOutputNode);


#ifdef __cplusplus
}
#endif

#endif /* __PWM_COUNT_H */
