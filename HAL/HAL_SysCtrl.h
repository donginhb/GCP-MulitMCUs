#ifndef  __HAL_SYSCTRL_H
#define  __HAL_SYSCTRL_H

#include <chip.h>

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


#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYSCTRL_H */
