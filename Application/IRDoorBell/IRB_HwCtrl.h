#ifndef  __IRB_HW_CFG_H
#define  __IRB_HW_CFG_H

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
void IRB_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void IRB_ShowMainWorkLed(void);



/*****************************************************************************
 * 人体红外探测线程接口
 ****************************************************************************/

/**
  * @brief  人体检测线程
  * @param  None
  * @retval None
  */
void IRB_HumanDetectHandler(void);

/*****************************************************************************
 * 红外遥控接收线程接口
 ****************************************************************************/

/**
  * @brief  红外遥控接收初始化
  * @param  None
  * @retval None
  */
void IRB_InitIR(void);


/**
  * @brief  红外遥控接收处理
  * @param  None
  * @retval None
  */
void IRB_IRHandler(void);


#ifdef __cplusplus
}
#endif


#endif /* __IRB_HW_CFG_H */
