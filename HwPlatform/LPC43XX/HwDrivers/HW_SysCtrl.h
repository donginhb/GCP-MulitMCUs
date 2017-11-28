#ifndef  __HW_SYSCTRL_H
#define  __HW_SYSCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 系统相关控制接口
 ****************************************************************************/

/**
  * @brief  系统复位函数
  * @param  None
  * @retval None
  */
void HW_SystemReset(void);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SYSCTRL_H */
