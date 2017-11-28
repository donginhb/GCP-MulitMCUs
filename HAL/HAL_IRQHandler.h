#ifndef  __HAL_IRQ_HANDLER_H
#define  __HAL_IRQ_HANDLER_H

#include "DataType/DataType.h"

#if defined(LPC17XX)
typedef enum
{
    //系统滴答
    IRQ_TRG_SYSTICK_UPDATE = 0,
    IRQ_TRG_SYSTICK_OS,
    
    //外部中断
    IRQ_TRG_EXTI0,
    IRQ_TRG_EXTI1,
    IRQ_TRG_EXTI2,
    IRQ_TRG_EXTI3,
    IRQ_TRG_EXTI4,
    IRQ_TRG_EXTI5,
    IRQ_TRG_EXTI6,
    IRQ_TRG_EXTI7,
    IRQ_TRG_EXTI8,
    IRQ_TRG_EXTI9,
    IRQ_TRG_EXTI10,
    IRQ_TRG_EXTI11,
    IRQ_TRG_EXTI12,
    IRQ_TRG_EXTI13,
    IRQ_TRG_EXTI14,
    IRQ_TRG_EXTI15,
    
    //DMA中断
    IRQ_TRG_DMA1_Channel1,
    IRQ_TRG_DMA1_Channel2,
    IRQ_TRG_DMA1_Channel3,
    IRQ_TRG_DMA1_Channel4,
    IRQ_TRG_DMA1_Channel5,
    IRQ_TRG_DMA1_Channel6,
    IRQ_TRG_DMA1_Channel7,
    IRQ_TRG_DMA2_Channel1,
    IRQ_TRG_DMA2_Channel2,
    IRQ_TRG_DMA2_Channel3,
    IRQ_TRG_DMA2_Channel4,
    IRQ_TRG_DMA2_Channel5,
    
    IRQ_TRG_WWDG,
    IRQ_TRG_PVD,
    IRQ_TRG_TAMPER,
    IRQ_TRG_RTC,
    IRQ_TRG_FLASH,
    IRQ_TRG_RCC,
    IRQ_TRG_ADC1_2,
    IRQ_TRG_USB_HP_CAN1_TX,
    IRQ_TRG_USB_LP_CAN1_RX0,
    IRQ_TRG_CAN1_RX1,
    IRQ_TRG_CAN1_SCE,
    IRQ_TRG_TIM1_BRK,
    IRQ_TRG_TIM1_UP,
    IRQ_TRG_TIM1_TRG_COM,
    IRQ_TRG_TIM1_CC,
    IRQ_TRG_TIM2,
    IRQ_TRG_TIM3,
    IRQ_TRG_TIM4,
    IRQ_TRG_I2C1_EV,
    IRQ_TRG_I2C1_ER,
    IRQ_TRG_I2C2_EV,
    IRQ_TRG_I2C2_ER,
    IRQ_TRG_SPI1,
    IRQ_TRG_SPI2,
    IRQ_TRG_USART1,
    IRQ_TRG_USART2,
    IRQ_TRG_USART3,
    IRQ_TRG_RTCAlarm,
    IRQ_TRG_USBWakeUp,
    IRQ_TRG_TIM8_BRK,
    IRQ_TRG_TIM8_UP,
    IRQ_TRG_TIM8_TRG_COM,
    IRQ_TRG_TIM8_CC,
    IRQ_TRG_ADC3,
    IRQ_TRG_FSMC,
    IRQ_TRG_SDIO,
    IRQ_TRG_TIM5,
    IRQ_TRG_SPI3,
    IRQ_TRG_UART4,
    IRQ_TRG_UART5,
    IRQ_TRG_TIM6,
    IRQ_TRG_TIM7,
    
}IRQ_TRIGGER_SOURCE;
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HAL_IRQ_Init(void);


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_SetTrgCallback(void (*ptr)(void), uBit32 ulTrgSource);


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_ReleaseTrgCallback(uBit32 ulTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_IRQ_HANDLER_H */
