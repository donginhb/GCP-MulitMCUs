/**
  ******************************************************************************
  * @file    HW_Exti.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Exti.h"
#include "chip.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#ifdef CHIP_LPC175X_6X
#define HW_GPIOINT_IRQn  (EINT3_IRQn)
#else
#define HW_GPIOINT_IRQn  (GPIO_IRQn)
#endif



/*****************************************************************************
 * 中断配置相关控制接口
 ****************************************************************************/

/**
  * @brief  外部中断配置
  * @param  nPort 端口号
  * @param  nPin  引脚号
  * @param  uTrgSource 触发源
  * @retval 0-成功 非0-失败
  */
uint32_t HW_EXTI_Init(uint16_t nPort, uint16_t nPin, uint8_t uTrgSource)
{
    //校验端口
    if ((nPort != GPIOINT_PORT0) && (nPort != GPIOINT_PORT2))
    {
        return 1;
    }
    
    //配置GPIO中断引脚
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, nPort, nPin);
    
    //配置GPIO中断触发条件
    if (uTrgSource & HW_EXTI_TRG_RISING)
    {
        Chip_GPIOINT_SetIntRising (LPC_GPIOINT, (LPC_GPIOINT_PORT_T)nPort, 1 << nPin);
    }
    if (uTrgSource & HW_EXTI_TRG_FALLING)
    {
        Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, (LPC_GPIOINT_PORT_T)nPort, 1 << nPin);
    } 
    
    //使能内核中断
	NVIC_ClearPendingIRQ(HW_GPIOINT_IRQn);
	NVIC_EnableIRQ(HW_GPIOINT_IRQn);
    
    
    return 0;
}



