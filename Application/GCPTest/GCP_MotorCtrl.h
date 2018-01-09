#ifndef  __GCP_MOTOR_CTRL_H
#define  __GCP_MOTOR_CTRL_H

#include "DataType/DataType.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 升降电机相关控制接口
 ****************************************************************************/

/**
  * @brief  升降电机初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 GCP_InitLiftMotor(void);


/**
  * @brief  脉冲当量设置
  * @param  pPosCtrlMotion 位置运动参数
  * @retval 0-成功 非0-失败
  */
uBit32 GCP_SetPulseRate(float fPulseRate);


/**
  * @brief  升降电机运动控制
  * @param  pPosCtrlMotion 位置运动参数
  * @retval 0-成功 非0-失败
  */
uBit32 GCP_MoveLiftMotor(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);


/**
  * @brief  升降电机限位检测处理
  * @param  None
  * @retval None
  */
void GCP_LiftPlatformHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __GCP_MOTOR_CTRL_H */
