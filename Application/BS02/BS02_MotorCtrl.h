#ifndef  __BS02_MOTOR_CTRL_H
#define  __BS02_MOTOR_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

#define BS02_MOTOR_DIRECT_CW      (1)     //顺时针旋转
#define BS02_MOTOR_DIRECT_ACW     (0)     //逆时针旋转
    
/*****************************************************************************
 * 电机相关控制接口
 ****************************************************************************/

/**
  * @brief  电机使能
  * @param  bIsEnable 电机使能位
  * @retval None
  */
void BS02_EnableMotor(bool bIsEnable);


/**
  * @brief  电机使能状态获取
  * @param  None
  * @retval 电机使能状态
  */
bool BS02_GetMotorEnableStatue(void);


/**
  * @brief  电机方向设置
  * @param  bDirect 电机方向位
  * @retval None
  */
void BS02_SetMotorDirect(bool bDirect);


/**
  * @brief  电机速度设置
  * @param  None
  * @retval 电机方向
  */
bool BS02_GetMotorDirect(void);


/**
  * @brief  电机方向设置
  * @param  ulSpeedIndex 速度索引
  * @retval 0-成功 非0-失败
  */
uBit32 BS02_SetMotorSpeed(uBit32 ulSpeedIndex);


/**
  * @brief  电机速度索引获取
  * @param  None
  * @retval 速度索引
  */
uBit32 BS02_GetMotorSpeedIndex(void);
      

/**
  * @brief  电机最大速度索引获取
  * @param  None
  * @retval 最大速度索引
  */
uBit32 BS02_GetMaxMotorSpeedIndex(void);


/**
  * @brief  电机驱动管理
  * @param  None
  * @retval None
  */
void BS02_MotorDriverHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __BS02_MOTOR_CTRL_H */
