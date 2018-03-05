#ifndef  __BS08M_HW_CFG_H
#define  __BS08M_HW_CFG_H

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
void BS08M_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS08M_MainWorkLedShow(void);



/*****************************************************************************
 * 蜂鸣器线程接口
 ****************************************************************************/

/**
  * @brief  蜂鸣器使能
  * @param  ulKeepTime 保持时间
  * @retval None
  */
void BS08M_EnableBeep(uBit32 ulKeepTime);


/**
  * @brief  蜂鸣器线程接口
  * @param  None
  * @retval None
  */
void BS08M_BeepHanlder(void);


/*****************************************************************************
 * 温度控制接口接口
 ****************************************************************************/

/**
  * @brief  温度值设置
  * @param  ulTempValue 温度值(放大因子:100)
  * @retval 0-成功 非0-失败
  */
uBit32 BS08M_SetTempValue(uBit32 ulTempValue);


#ifdef __cplusplus
}
#endif


#endif /* __BS08M_HW_CFG_H */
