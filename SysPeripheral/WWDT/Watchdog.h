#ifndef  __WATCHDOG_H
#define  __WATCHDOG_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * WatchDog 相关控制接口
 ****************************************************************************/

/**
  * @brief  看门狗启动
  * @param  ulTimeOutMs 超时时间
  * @retval None
  * @note   看门狗一旦开启,就无法停止
  */
void WWDT_Start(uBit32 ulTimeOutMs);


/**
  * @brief  喂狗
  * @param  None
  * @retval None
  */
void WWDT_Feed(void);


#ifdef __cplusplus
}
#endif

#endif /* __WATCHDOG_H */
