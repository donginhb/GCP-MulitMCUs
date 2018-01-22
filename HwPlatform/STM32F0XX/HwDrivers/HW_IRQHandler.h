#ifndef  __HW_IRQ_HANDLER_H
#define  __HW_IRQ_HANDLER_H

#include "chip.h"

typedef enum
{
    //系统滴答
    STM32F0_IRQ_TRG_SYSTICK_UPDATE = 0,
    STM32F0_IRQ_TRG_SYSTICK_OS,
    
    //外部中断
    STM32F0_IRQ_TRG_EXTI0,
    STM32F0_IRQ_TRG_EXTI1,
    STM32F0_IRQ_TRG_EXTI2,
    STM32F0_IRQ_TRG_EXTI3,
    STM32F0_IRQ_TRG_EXTI4,
    STM32F0_IRQ_TRG_EXTI5,
    STM32F0_IRQ_TRG_EXTI6,
    STM32F0_IRQ_TRG_EXTI7,
    STM32F0_IRQ_TRG_EXTI8,
    STM32F0_IRQ_TRG_EXTI9,
    STM32F0_IRQ_TRG_EXTI10,
    STM32F0_IRQ_TRG_EXTI11,
    STM32F0_IRQ_TRG_EXTI12,
    STM32F0_IRQ_TRG_EXTI13,
    STM32F0_IRQ_TRG_EXTI14,
    STM32F0_IRQ_TRG_EXTI15,
    
    STM32F0_IRQ_TRG_WWDG,
    STM32F0_IRQ_TRG_PVD,
    STM32F0_IRQ_TRG_RTC,
    STM32F0_IRQ_TRG_FLASH,
    STM32F0_IRQ_TRG_RCC,
    STM32F0_IRQ_TRG_TS,
    STM32F0_IRQ_TRG_DMA1_Channel1,
    STM32F0_IRQ_TRG_DMA1_Channel2_3,
    STM32F0_IRQ_TRG_DMA1_Channel4_5,
    STM32F0_IRQ_TRG_ADC1_COMP,
    STM32F0_IRQ_TRG_TIM1_BRK_UP_TRG_COM,
    STM32F0_IRQ_TRG_TIM1_CC,
    STM32F0_IRQ_TRG_TIM2,
    STM32F0_IRQ_TRG_TIM3,
    STM32F0_IRQ_TRG_TIM6_DAC,
    STM32F0_IRQ_TRG_TIM14,
    STM32F0_IRQ_TRG_TIM15,
    STM32F0_IRQ_TRG_TIM16,
    STM32F0_IRQ_TRG_TIM17,
    STM32F0_IRQ_TRG_I2C1,
    STM32F0_IRQ_TRG_I2C2,
    STM32F0_IRQ_TRG_SPI1,
    STM32F0_IRQ_TRG_SPI2,
    STM32F0_IRQ_TRG_USART1,
    STM32F0_IRQ_TRG_USART2,
    STM32F0_IRQ_TRG_CEC,
    
}STM32_IRQ_TRIGGER_SOURCE;


#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  空指针入口
  * @param  None
  * @retval None
  */
void HW_IRQ_NullEntry(void);


/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HW_IRQ_Init(void);


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HW_IRQ_SetTrgCallback(void (*ptr)(void), uint32_t ulTrgSource);


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HW_IRQ_ReleaseTrgCallback(uint32_t ulTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __HW_IRQ_HANDLER_H */
