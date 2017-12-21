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

/**
  * @brief  程序跳转
  * @param  None
  * @retval None
  */
void HW_JumToApp(void);


/**
  * @brief  总中断使能
  * @param  None
  * @retval None
  */
void HW_EnableIRQ(void);


/**
  * @brief  总中断关闭
  * @param  None
  * @retval None
  */
void HW_DisableIRQ(void);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SYSCTRL_H */
