#ifndef  __HW_SYSCTRL_H
#define  __HW_SYSCTRL_H

#include "chip.h"

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



/*****************************************************************************
 * 系统NVIC相关
 ****************************************************************************/

/**
  * @brief  NVIC使能
  * @param  IRQn 中断号
  * @param  ulPreemptPrio  优先级
  * @retval None
  */
void HW_NVIC_Enable(IRQn_Type IRQn, uint32_t ulPreemptPrio);


/**
  * @brief  禁止对应的内核中断
  * @param  IRQn 中断号
  * @retval None
  */
void HW_NVIC_DisableIRQ(IRQn_Type IRQn);


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


/*****************************************************************************
 * 系统唤醒休眠相关
 ****************************************************************************/

/**
  * @brief  系统待机
  * @param  None
  * @retval None
  */
static void HW_SystemStandby(void);


/**
  * @brief  待机模式进入
  * @param  None
  * @retval None
  */
void HW_EnterSrandby(void);


/*****************************************************************************
 * 系统JTAG引脚相关
 ****************************************************************************/

/**
  * @brief  JTAG模式设置,用于设置JTAG的模式
  * @param  mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
  * @retval None
  */
void HW_SWJConfig(uint8_t uMode);


#ifdef __cplusplus
}
#endif

#endif /* __HW_SYSCTRL_H */
