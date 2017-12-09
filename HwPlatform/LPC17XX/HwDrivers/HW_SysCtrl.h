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
  * @note   此函数应用于Bootloader对APP的跳转,APP地址写死为0x70000000
  */
void HW_JumToApp(void);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SYSCTRL_H */
