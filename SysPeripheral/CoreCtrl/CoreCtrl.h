#ifndef  __CORE_CTRL_H
#define  __CORE_CTRL_H

#include "DataType/DataType.h"
#include "HAL/HAL_SysCtrl.h"


#ifdef __cplusplus
extern "C"
{
#endif

    
/*****************************************************************************
 * 系统复位相关接口函数
 ****************************************************************************/

/**
  * @brief  立即复位
  * @param  None
  * @retval None
  */
void CoreCtrl_ResetSystemNow(void);


/**
  * @brief  延时复位
  * @param  ulDelayMs 延时的时间(单位:MS)
  * @retval None
  */
void CoreCtrl_ResetSystemDelay(uBit32 ulDelayMs);


/**
  * @brief  系统复位状态获取
  * @param  None
  * @retval true-正在复位(延时执行中) false-正常
  */
bool CoreCtrl_GetResetState(void);


/**
  * @brief  系统复位处理(放在while循环中处理)
  * @param  None
  * @retval None
  */
void CoreCtrl_ResetHandler(void);


/*****************************************************************************
 * 系统程序跳转相关接口函数
 ****************************************************************************/

/**
  * @brief  程序跳转
  * @param  None
  * @retval None
  * @note   此函数应用于Bootloader对APP的跳转
  */
void CoreCtrl_JumToApp(void);


/**
  * @brief  总中断使能
  * @param  None
  * @retval None
  */
void CoreCtrl_EnableIRQ(void);


/**
  * @brief  总中断关闭
  * @param  None
  * @retval None
  */
void CoreCtrl_DisableIRQ(void);   


/*****************************************************************************
 * JTAG相关接口函数
 ****************************************************************************/

/**
  * @brief  JTAG引脚配置
  * @param  None
  * @retval None
  */
void CoreCtrl_JTAGConfig(void);


#ifdef __cplusplus
}
#endif

#endif /* __CORE_CTRL_H */
