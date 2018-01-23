#ifndef  __IRB_MOTOR_CTRL_H
#define  __IRB_MOTOR_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

#define IRB_MOTOR_DIRECT_CW      (1)     //顺时针旋转
#define IRB_MOTOR_DIRECT_ACW     (0)     //逆时针旋转
    
/*****************************************************************************
 * 电机相关控制接口
 ****************************************************************************/

/**
  * @brief  电机使能
  * @param  bIsEnable 电机使能位
  * @retval None
  */
void IRB_EnableMotor(bool bIsEnable);


/**
  * @brief  电机使能状态获取
  * @param  None
  * @retval 电机使能状态
  */
bool IRB_GetMotorEnableStatue(void);


/**
  * @brief  电机方向设置
  * @param  bDirect 电机方向位
  * @retval None
  */
void IRB_SetMotorDirect(bool bDirect);


/**
  * @brief  电机速度设置
  * @param  None
  * @retval 电机方向
  */
bool IRB_GetMotorDirect(void);


/**
  * @brief  电机方向设置
  * @param  ulSpeedIndex 速度索引
  * @retval 0-成功 非0-失败
  */
uBit32 IRB_SetMotorSpeed(uBit32 ulSpeedIndex);


/**
  * @brief  电机速度索引获取
  * @param  None
  * @retval 速度索引
  */
uBit32 IRB_GetMotorSpeedIndex(void);
      

/**
  * @brief  电机最大速度索引获取
  * @param  None
  * @retval 最大速度索引
  */
uBit32 IRB_GetMaxMotorSpeedIndex(void);


/**
  * @brief  电机驱动管理
  * @param  None
  * @retval None
  */
void IRB_MotorDriverHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __IRB_MOTOR_CTRL_H */
