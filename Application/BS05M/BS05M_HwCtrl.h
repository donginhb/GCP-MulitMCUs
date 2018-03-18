#ifndef  __BS05M_HW_CFG_H
#define  __BS05M_HW_CFG_H

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
void BS05M_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS05M_MainWorkLedShow(void);



/*****************************************************************************
 * 光照强度检测处理线程接口
 ****************************************************************************/

/**
  * @brief  光照强度检测线程接口
  * @param  None
  * @retval None
  */
void BS05M_SumLightDetectHandler(void);



/*****************************************************************************
 * 人体红外检测处理线程接口
 ****************************************************************************/

/**
  * @brief  人体红外检测线程接口
  * @param  None
  * @retval None
  */
void BS05M_HumanDetectHandler(void);


/*****************************************************************************
 * 灯输出处理线程接口
 ****************************************************************************/

/**
  * @brief  灯光输出控制接口
  * @param  None
  * @retval None
  */
void BS05M_CtrlLightStrength(uBit8 uDutyRatio);


/**
  * @brief  灯光输出控制接口
  * @param  bIsEnable false-关闭 true-打开
  * @retval None
  */
void BS05M_EnableLignt(bool bIsEnable);


/**
  * @brief  灯光输出控制接口
  * @param  None
  * @retval None
  */
void BS05M_LightOutputHandler(void);


/**
  * @brief  灯光输出使能接口(仅在非自动模式下有效)
  * @param  bIsEnable false-关闭 true-使能
  * @retval None
  */
void BS05M_EnableLightOutput(bool bIsEnable);


/**
  * @brief  灯光输出自动模式使能
  * @param  bIsEnable false-关闭 true-使能
  * @retval None
  */
void BS05M_EnableAutoOutputMode(bool bIsEnable);


#ifdef __cplusplus
}
#endif


#endif /* __BS05M_HW_CFG_H */
