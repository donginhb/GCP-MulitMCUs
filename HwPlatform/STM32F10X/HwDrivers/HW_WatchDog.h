#ifndef  __HW_WATCHDOG_H
#define  __HW_WATCHDOG_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * WatchDog 基本功能接口
 ****************************************************************************/

/**
  * @brief  看门狗启动
  * @param  ulTimeOutMs 超时时间
  * @retval None
  * @note   看门狗一旦开启,就无法停止
  */
void HW_WWDT_Start(uint32_t ulTimeOutMs);


/**
  * @brief  喂狗
  * @param  None
  * @retval None
  */
void HW_WWDT_Feed(void);

    
#ifdef __cplusplus
}
#endif



#endif /* __HW_WATCHDOG_H */
