#ifndef  __HW_WATCHDOG_H
#define  __HW_WATCHDOG_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  看门狗初始化
  * @param  ulTimeOutMs 超时时间
  * @retval None
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
