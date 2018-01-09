#ifndef  __GCP_HW_CFG_H
#define  __GCP_HW_CFG_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void GCP_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void GCP_ShowMainWorkLed(void);


/*****************************************************************************
 * 温度采集线程接口
 ****************************************************************************/

/**
  * @brief  温度数据采集
  * @param  None
  * @retval None
  */
void GCP_SamplingTempValue(void);


/**
  * @brief  温度采集数据获取
  * @param  None
  * @retval 采集到的温度数据(原始AD值,未作转换)
  */
uBit32 GCP_GetTempSamplingValue(void);


/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/
     
/**
  * @brief  VM 启动货道电机
  * @param  ulRow 行信号 [0-9]
  * @param  ulCol 列信号 [0-9]
  * @retval 0-成功 非0-失败
  */
uBit32 GCP_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol);


/**
  * @brief  货道电机运行状态获取
  * @param  None
  * @retval 货道电机运行状态
  *   @arg  GCP_AISLE_MOTOR_STATUS_IDLE    货道电机空闲
  *         GCP_AISLE_MOTOR_STATUS_RUNNING 货道电机运行中
  *         GCP_AISLE_MOTOR_STATUS_OVER    超时
  */
uBit32 GCP_GetAisleMotorRunningState(void);


/**
  * @brief  VM 货道电机处理
  * @param  None
  * @retval None
  */
void GCP_AisleMotorHandler(void);


#ifdef __cplusplus
}
#endif


#endif /* __GCP_HW_CFG_H */
