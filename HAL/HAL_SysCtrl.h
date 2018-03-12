#ifndef  __HAL_SYSCTRL_H
#define  __HAL_SYSCTRL_H

#include "DataType/DataType.h"
#include "HwDrivers/HW_SysCtrl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 系统复位相关
 ****************************************************************************/

/**
  * @brief  系统复位函数
  * @param  None
  * @retval None
  */
void HAL_SystemReset(void);


/**
  * @brief  程序跳转
  * @param  None
  * @retval None
  * @note   此函数应用于Bootloader对APP的跳转
  */
void HAL_JumToApp(void);


/**
  * @brief  总中断使能
  * @param  None
  * @retval None
  */
void HAL_EnableIRQ(void);


/**
  * @brief  总中断关闭
  * @param  None
  * @retval None
  */
void HAL_DisableIRQ(void);


/**
  * @brief  JTAG模式设置,用于设置JTAG的模式
  * @param  mode:jtag,swd模式设置; 0-全使能;1-使能SWD;2-全关闭;
  * @retval None
  */
void HAL_JTAGConfig(uBit8 uMode);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYSCTRL_H */
