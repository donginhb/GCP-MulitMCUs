#ifndef  __HW_IRQ_HANDLER_H
#define  __HW_IRQ_HANDLER_H

#include "chip.h"

typedef enum
{
    //系统滴答
    LPC_IRQ_TRG_SYSTICK_UPDATE = 0,
    LPC_IRQ_TRG_SYSTICK_OS,
    
    //串口
    LPC_IRQ_TRG_UART0,
    LPC_IRQ_TRG_UART1,
    LPC_IRQ_TRG_UART2,
    LPC_IRQ_TRG_UART3,

    //DMA通道
    LPC_IRQ_TRG_DMA_CH1,
    LPC_IRQ_TRG_DMA_CH2,
    LPC_IRQ_TRG_DMA_CH3,
    LPC_IRQ_TRG_DMA_CH4,
    LPC_IRQ_TRG_DMA_CH5,
    LPC_IRQ_TRG_DMA_CH6,
    LPC_IRQ_TRG_DMA_CH7,
    LPC_IRQ_TRG_DMA_CH8,
    
    //EXTI
    LPC_IRQ_TRG_EXTI_CH0,
    LPC_IRQ_TRG_EXTI_CH1,
    LPC_IRQ_TRG_EXTI_CH2,
    LPC_IRQ_TRG_EXTI_CH3,
    LPC_IRQ_TRG_EXTI_CH4,
    LPC_IRQ_TRG_EXTI_CH5,
    LPC_IRQ_TRG_EXTI_CH6,
    LPC_IRQ_TRG_EXTI_CH7,
    
    //定时器
    LPC_IRQ_TRG_TIME0,
    LPC_IRQ_TRG_TIME1,
    LPC_IRQ_TRG_TIME2,
    LPC_IRQ_TRG_TIME3,
    
    //MCPWM
    LPC_IRQ_TRG_MCPWM0,
    LPC_IRQ_TRG_MCPWM1,
    LPC_IRQ_TRG_MCPWM2,
    
}LPC_IRQ_TRIGGER_SOURCE;



#ifdef __cplusplus
extern "C" {
#endif


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
