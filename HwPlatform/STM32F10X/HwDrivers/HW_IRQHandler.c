/**
  ******************************************************************************
  * @file    HW_IRQHandler.c
  * @author  Duhanfneg
  * @version V1.0 
  * @date    2017.08.08
  * @brief   LPC17xx IRQ Callback Funtion interface
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "chip.h"
#include "HW_IRQHandler.h"


#define STM32_GPIO_INT_COUNT                (16)
#define STM32_DMA_COUNT                     (2)
#define STM32_DMA_CH_COUNT                  (7)

typedef struct 
{
    void (*pf_SysTick_Update             )(void);
    void (*pf_SysTick_Handler            )(void);
    
    void (*pf_WWDG_IRQHandler            )(void);
    void (*pf_PVD_IRQHandler             )(void);
    void (*pf_TAMPER_IRQHandler          )(void);
    void (*pf_RTC_IRQHandler             )(void);
    void (*pf_FLASH_IRQHandler           )(void);
    void (*pf_RCC_IRQHandler             )(void);
    void (*pf_ADC1_2_IRQHandler          )(void);
    void (*pf_USB_HP_CAN1_TX_IRQHandler  )(void);
    void (*pf_USB_LP_CAN1_RX0_IRQHandler )(void);
    void (*pf_CAN1_RX1_IRQHandler        )(void);
    void (*pf_CAN1_SCE_IRQHandler        )(void);
    void (*pf_TIM1_BRK_IRQHandler        )(void);
    void (*pf_TIM1_UP_IRQHandler         )(void);
    void (*pf_TIM1_TRG_COM_IRQHandler    )(void);
    void (*pf_TIM1_CC_IRQHandler         )(void);
    void (*pf_TIM2_IRQHandler            )(void);
    void (*pf_TIM3_IRQHandler            )(void);
    void (*pf_TIM4_IRQHandler            )(void);
    void (*pf_I2C1_EV_IRQHandler         )(void);
    void (*pf_I2C1_ER_IRQHandler         )(void);
    void (*pf_I2C2_EV_IRQHandler         )(void);
    void (*pf_I2C2_ER_IRQHandler         )(void);
    void (*pf_SPI1_IRQHandler            )(void);
    void (*pf_SPI2_IRQHandler            )(void);
    void (*pf_USART1_IRQHandler          )(void);
    void (*pf_USART2_IRQHandler          )(void);
    void (*pf_USART3_IRQHandler          )(void);
    void (*pf_RTCAlarm_IRQHandler        )(void);
    void (*pf_USBWakeUp_IRQHandler       )(void);
    void (*pf_TIM8_BRK_IRQHandler        )(void);
    void (*pf_TIM8_UP_IRQHandler         )(void);
    void (*pf_TIM8_TRG_COM_IRQHandler    )(void);
    void (*pf_TIM8_CC_IRQHandler         )(void);
    void (*pf_ADC3_IRQHandler            )(void);
    void (*pf_FSMC_IRQHandler            )(void);
    void (*pf_SDIO_IRQHandler            )(void);
    void (*pf_TIM5_IRQHandler            )(void);
    void (*pf_SPI3_IRQHandler            )(void);
    void (*pf_UART4_IRQHandler           )(void);
    void (*pf_UART5_IRQHandler           )(void);
    void (*pf_TIM6_IRQHandler            )(void);
    void (*pf_TIM7_IRQHandler            )(void);

    void (*pf_EXTI_IRQHandler [STM32_GPIO_INT_COUNT])(void);
    void (*pf_DMA_CH_IRQHandler [STM32_DMA_COUNT][STM32_DMA_CH_COUNT])(void);
    
}STM32F0XX_IRQ_INTERFACE;


static STM32F0XX_IRQ_INTERFACE  g_IRQInterface;  //全局中断回调指针接口


/**
  * @brief  空指针入口
  * @param  None
  * @retval None
  */
void HW_IRQ_NullEntry(void){}


/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HW_IRQ_Init(void)
{
    g_IRQInterface.pf_SysTick_Update                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SysTick_Handler               = HW_IRQ_NullEntry;

    
    g_IRQInterface.pf_WWDG_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_PVD_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TAMPER_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RTC_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_FLASH_IRQHandler              = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RCC_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC1_2_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USB_HP_CAN1_TX_IRQHandler     = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USB_LP_CAN1_RX0_IRQHandler    = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CAN1_RX1_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CAN1_SCE_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_BRK_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_UP_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_TRG_COM_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_CC_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM2_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM3_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM4_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_EV_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_ER_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C2_EV_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C2_ER_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI1_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI2_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USART1_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USART2_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USART3_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RTCAlarm_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USBWakeUp_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM8_BRK_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM8_UP_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM8_TRG_COM_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM8_CC_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC3_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_FSMC_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SDIO_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM5_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI3_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART4_IRQHandler              = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART5_IRQHandler              = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM6_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM7_IRQHandler               = HW_IRQ_NullEntry;
    
    //EXTI Group
    for (int i = 0; i < STM32_GPIO_INT_COUNT; i++)
    {
        g_IRQInterface.pf_EXTI_IRQHandler[i] = HW_IRQ_NullEntry;
    }
    
    //DMA Group
    for (int i = 0; i < STM32_DMA_COUNT; i++)
    {
        for (int j = 0; j < STM32_DMA_CH_COUNT; j++)
        {
            g_IRQInterface.pf_DMA_CH_IRQHandler[i][j] = HW_IRQ_NullEntry;
        }
    }
    
}


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HW_IRQ_SetTrgCallback(void (*ptr)(void), uint32_t ulTrgSource)
{
    if (ptr == NULL) 
    {
        return;
    }
    
    switch (ulTrgSource)
    {

    //系统滴答
    case IRQ_TRG_SYSTICK_UPDATE:        g_IRQInterface.pf_SysTick_Update                  = ptr; break;
    case IRQ_TRG_SYSTICK_OS:            g_IRQInterface.pf_SysTick_Handler                 = ptr; break;
    
    //外部中断
    case IRQ_TRG_EXTI0: 
    case IRQ_TRG_EXTI1: 
    case IRQ_TRG_EXTI2: 
    case IRQ_TRG_EXTI3: 
    case IRQ_TRG_EXTI4: 
    case IRQ_TRG_EXTI5: 
    case IRQ_TRG_EXTI6: 
    case IRQ_TRG_EXTI7: 
    case IRQ_TRG_EXTI8: 
    case IRQ_TRG_EXTI9: 
    case IRQ_TRG_EXTI10: 
    case IRQ_TRG_EXTI11: 
    case IRQ_TRG_EXTI12: 
    case IRQ_TRG_EXTI13: 
    case IRQ_TRG_EXTI14: 
    case IRQ_TRG_EXTI15:                g_IRQInterface.pf_EXTI_IRQHandler[ulTrgSource-IRQ_TRG_EXTI0] = ptr; break;

    //DMA中断
    case IRQ_TRG_DMA1_Channel1: 
    case IRQ_TRG_DMA1_Channel2: 
    case IRQ_TRG_DMA1_Channel3: 
    case IRQ_TRG_DMA1_Channel4: 
    case IRQ_TRG_DMA1_Channel5: 
    case IRQ_TRG_DMA1_Channel6:     
    case IRQ_TRG_DMA1_Channel7:         g_IRQInterface.pf_DMA_CH_IRQHandler[0][ulTrgSource-IRQ_TRG_DMA1_Channel1] = ptr; break;
    case IRQ_TRG_DMA2_Channel1: 
    case IRQ_TRG_DMA2_Channel2: 
    case IRQ_TRG_DMA2_Channel3: 
    case IRQ_TRG_DMA2_Channel4: 
    case IRQ_TRG_DMA2_Channel5:         g_IRQInterface.pf_DMA_CH_IRQHandler[1][ulTrgSource-IRQ_TRG_DMA2_Channel1] = ptr; break;

    case IRQ_TRG_WWDG:                  g_IRQInterface.pf_WWDG_IRQHandler                 = ptr; break;
    case IRQ_TRG_PVD:                   g_IRQInterface.pf_PVD_IRQHandler                  = ptr; break;
    case IRQ_TRG_TAMPER:                g_IRQInterface.pf_TAMPER_IRQHandler               = ptr; break;
    case IRQ_TRG_RTC:                   g_IRQInterface.pf_RTC_IRQHandler                  = ptr; break;
    case IRQ_TRG_FLASH:                 g_IRQInterface.pf_FLASH_IRQHandler                = ptr; break;
    case IRQ_TRG_RCC:                   g_IRQInterface.pf_RCC_IRQHandler                  = ptr; break;
    case IRQ_TRG_ADC1_2:                g_IRQInterface.pf_ADC1_2_IRQHandler               = ptr; break;
    case IRQ_TRG_USB_HP_CAN1_TX:        g_IRQInterface.pf_USB_HP_CAN1_TX_IRQHandler       = ptr; break;
    case IRQ_TRG_USB_LP_CAN1_RX0:       g_IRQInterface.pf_USB_LP_CAN1_RX0_IRQHandler      = ptr; break;
    case IRQ_TRG_CAN1_RX1:              g_IRQInterface.pf_CAN1_RX1_IRQHandler             = ptr; break;
    case IRQ_TRG_CAN1_SCE:              g_IRQInterface.pf_CAN1_SCE_IRQHandler             = ptr; break;
    case IRQ_TRG_TIM1_BRK:              g_IRQInterface.pf_TIM1_BRK_IRQHandler             = ptr; break;
    case IRQ_TRG_TIM1_UP:               g_IRQInterface.pf_TIM1_UP_IRQHandler              = ptr; break;
    case IRQ_TRG_TIM1_TRG_COM:          g_IRQInterface.pf_TIM1_TRG_COM_IRQHandler         = ptr; break;
    case IRQ_TRG_TIM1_CC:               g_IRQInterface.pf_TIM1_CC_IRQHandler              = ptr; break;
    case IRQ_TRG_TIM2:                  g_IRQInterface.pf_TIM2_IRQHandler                 = ptr; break;
    case IRQ_TRG_TIM3:                  g_IRQInterface.pf_TIM3_IRQHandler                 = ptr; break;
    case IRQ_TRG_TIM4:                  g_IRQInterface.pf_TIM4_IRQHandler                 = ptr; break;
    case IRQ_TRG_I2C1_EV:               g_IRQInterface.pf_I2C1_EV_IRQHandler              = ptr; break;
    case IRQ_TRG_I2C1_ER:               g_IRQInterface.pf_I2C1_ER_IRQHandler              = ptr; break;
    case IRQ_TRG_I2C2_EV:               g_IRQInterface.pf_I2C2_EV_IRQHandler              = ptr; break;
    case IRQ_TRG_I2C2_ER:               g_IRQInterface.pf_I2C2_ER_IRQHandler              = ptr; break;
    case IRQ_TRG_SPI1:                  g_IRQInterface.pf_SPI1_IRQHandler                 = ptr; break;
    case IRQ_TRG_SPI2:                  g_IRQInterface.pf_SPI2_IRQHandler                 = ptr; break;
    case IRQ_TRG_USART1:                g_IRQInterface.pf_USART1_IRQHandler               = ptr; break;
    case IRQ_TRG_USART2:                g_IRQInterface.pf_USART2_IRQHandler               = ptr; break;
    case IRQ_TRG_USART3:                g_IRQInterface.pf_USART3_IRQHandler               = ptr; break;
    case IRQ_TRG_RTCAlarm:              g_IRQInterface.pf_RTCAlarm_IRQHandler             = ptr; break;
    case IRQ_TRG_USBWakeUp:             g_IRQInterface.pf_USBWakeUp_IRQHandler            = ptr; break;
    case IRQ_TRG_TIM8_BRK:              g_IRQInterface.pf_TIM8_BRK_IRQHandler             = ptr; break;
    case IRQ_TRG_TIM8_UP:               g_IRQInterface.pf_TIM8_UP_IRQHandler              = ptr; break;
    case IRQ_TRG_TIM8_TRG_COM:          g_IRQInterface.pf_TIM8_TRG_COM_IRQHandler         = ptr; break;
    case IRQ_TRG_TIM8_CC:               g_IRQInterface.pf_TIM8_CC_IRQHandler              = ptr; break;
    case IRQ_TRG_ADC3:                  g_IRQInterface.pf_ADC3_IRQHandler                 = ptr; break;
    case IRQ_TRG_FSMC:                  g_IRQInterface.pf_FSMC_IRQHandler                 = ptr; break;
    case IRQ_TRG_SDIO:                  g_IRQInterface.pf_SDIO_IRQHandler                 = ptr; break;
    case IRQ_TRG_TIM5:                  g_IRQInterface.pf_TIM5_IRQHandler                 = ptr; break;
    case IRQ_TRG_SPI3:                  g_IRQInterface.pf_SPI3_IRQHandler                 = ptr; break;
    case IRQ_TRG_UART4:                 g_IRQInterface.pf_UART4_IRQHandler                = ptr; break;
    case IRQ_TRG_UART5:                 g_IRQInterface.pf_UART5_IRQHandler                = ptr; break;
    case IRQ_TRG_TIM6:                  g_IRQInterface.pf_TIM6_IRQHandler                 = ptr; break;
    case IRQ_TRG_TIM7:                  g_IRQInterface.pf_TIM7_IRQHandler                 = ptr; break;

    default: break;
    }
    
}


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HW_IRQ_ReleaseTrgCallback(uint32_t ulTrgSource)
{
    switch (ulTrgSource)
    {

    //系统滴答
    case IRQ_TRG_SYSTICK_UPDATE:        g_IRQInterface.pf_SysTick_Update                  = HW_IRQ_NullEntry; break;
    case IRQ_TRG_SYSTICK_OS:            g_IRQInterface.pf_SysTick_Handler                 = HW_IRQ_NullEntry; break;
    
    //外部中断
    case IRQ_TRG_EXTI0: 
    case IRQ_TRG_EXTI1: 
    case IRQ_TRG_EXTI2: 
    case IRQ_TRG_EXTI3: 
    case IRQ_TRG_EXTI4: 
    case IRQ_TRG_EXTI5: 
    case IRQ_TRG_EXTI6: 
    case IRQ_TRG_EXTI7: 
    case IRQ_TRG_EXTI8: 
    case IRQ_TRG_EXTI9: 
    case IRQ_TRG_EXTI10: 
    case IRQ_TRG_EXTI11: 
    case IRQ_TRG_EXTI12: 
    case IRQ_TRG_EXTI13: 
    case IRQ_TRG_EXTI14: 
    case IRQ_TRG_EXTI15:                g_IRQInterface.pf_EXTI_IRQHandler[ulTrgSource-IRQ_TRG_EXTI0] = HW_IRQ_NullEntry; break;

    //DMA中断
    case IRQ_TRG_DMA1_Channel1:         
    case IRQ_TRG_DMA1_Channel2:         
    case IRQ_TRG_DMA1_Channel3:         
    case IRQ_TRG_DMA1_Channel4:         
    case IRQ_TRG_DMA1_Channel5:         
    case IRQ_TRG_DMA1_Channel6:         
    case IRQ_TRG_DMA1_Channel7:         g_IRQInterface.pf_DMA_CH_IRQHandler[0][ulTrgSource-IRQ_TRG_DMA1_Channel1] = HW_IRQ_NullEntry; break;
    case IRQ_TRG_DMA2_Channel1:         
    case IRQ_TRG_DMA2_Channel2:         
    case IRQ_TRG_DMA2_Channel3:         
    case IRQ_TRG_DMA2_Channel4:         
    case IRQ_TRG_DMA2_Channel5:         g_IRQInterface.pf_DMA_CH_IRQHandler[1][ulTrgSource-IRQ_TRG_DMA2_Channel1] = HW_IRQ_NullEntry; break;

    case IRQ_TRG_WWDG:                  g_IRQInterface.pf_WWDG_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_PVD:                   g_IRQInterface.pf_PVD_IRQHandler                  = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TAMPER:                g_IRQInterface.pf_TAMPER_IRQHandler               = HW_IRQ_NullEntry; break;
    case IRQ_TRG_RTC:                   g_IRQInterface.pf_RTC_IRQHandler                  = HW_IRQ_NullEntry; break;
    case IRQ_TRG_FLASH:                 g_IRQInterface.pf_FLASH_IRQHandler                = HW_IRQ_NullEntry; break;
    case IRQ_TRG_RCC:                   g_IRQInterface.pf_RCC_IRQHandler                  = HW_IRQ_NullEntry; break;
    case IRQ_TRG_ADC1_2:                g_IRQInterface.pf_ADC1_2_IRQHandler               = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USB_HP_CAN1_TX:        g_IRQInterface.pf_USB_HP_CAN1_TX_IRQHandler       = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USB_LP_CAN1_RX0:       g_IRQInterface.pf_USB_LP_CAN1_RX0_IRQHandler      = HW_IRQ_NullEntry; break;
    case IRQ_TRG_CAN1_RX1:              g_IRQInterface.pf_CAN1_RX1_IRQHandler             = HW_IRQ_NullEntry; break;
    case IRQ_TRG_CAN1_SCE:              g_IRQInterface.pf_CAN1_SCE_IRQHandler             = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM1_BRK:              g_IRQInterface.pf_TIM1_BRK_IRQHandler             = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM1_UP:               g_IRQInterface.pf_TIM1_UP_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM1_TRG_COM:          g_IRQInterface.pf_TIM1_TRG_COM_IRQHandler         = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM1_CC:               g_IRQInterface.pf_TIM1_CC_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM2:                  g_IRQInterface.pf_TIM2_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM3:                  g_IRQInterface.pf_TIM3_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM4:                  g_IRQInterface.pf_TIM4_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_I2C1_EV:               g_IRQInterface.pf_I2C1_EV_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_I2C1_ER:               g_IRQInterface.pf_I2C1_ER_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_I2C2_EV:               g_IRQInterface.pf_I2C2_EV_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_I2C2_ER:               g_IRQInterface.pf_I2C2_ER_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_SPI1:                  g_IRQInterface.pf_SPI1_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_SPI2:                  g_IRQInterface.pf_SPI2_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USART1:                g_IRQInterface.pf_USART1_IRQHandler               = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USART2:                g_IRQInterface.pf_USART2_IRQHandler               = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USART3:                g_IRQInterface.pf_USART3_IRQHandler               = HW_IRQ_NullEntry; break;
    case IRQ_TRG_RTCAlarm:              g_IRQInterface.pf_RTCAlarm_IRQHandler             = HW_IRQ_NullEntry; break;
    case IRQ_TRG_USBWakeUp:             g_IRQInterface.pf_USBWakeUp_IRQHandler            = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM8_BRK:              g_IRQInterface.pf_TIM8_BRK_IRQHandler             = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM8_UP:               g_IRQInterface.pf_TIM8_UP_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM8_TRG_COM:          g_IRQInterface.pf_TIM8_TRG_COM_IRQHandler         = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM8_CC:               g_IRQInterface.pf_TIM8_CC_IRQHandler              = HW_IRQ_NullEntry; break;
    case IRQ_TRG_ADC3:                  g_IRQInterface.pf_ADC3_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_FSMC:                  g_IRQInterface.pf_FSMC_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_SDIO:                  g_IRQInterface.pf_SDIO_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM5:                  g_IRQInterface.pf_TIM5_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_SPI3:                  g_IRQInterface.pf_SPI3_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_UART4:                 g_IRQInterface.pf_UART4_IRQHandler                = HW_IRQ_NullEntry; break;
    case IRQ_TRG_UART5:                 g_IRQInterface.pf_UART5_IRQHandler                = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM6:                  g_IRQInterface.pf_TIM6_IRQHandler                 = HW_IRQ_NullEntry; break;
    case IRQ_TRG_TIM7:                  g_IRQInterface.pf_TIM7_IRQHandler                 = HW_IRQ_NullEntry; break;

    default: break;
    }
    
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    g_IRQInterface.pf_SysTick_Update();
    g_IRQInterface.pf_SysTick_Handler();
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<0))
    {
        EXTI->PR |= (0x1<<0);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[0](); //执行中断回调
    }
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<1))
    {
        EXTI->PR |= (0x1<<1);   //清标志位
            
        g_IRQInterface.pf_EXTI_IRQHandler[1](); //执行中断回调
    } 
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<2))
    {
        EXTI->PR |= (0x1<<2);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[2](); //执行中断回调
    }
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<3))
    {
        EXTI->PR |= (0x1<<3);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[3](); //执行中断回调
    }
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<4))
    {
        EXTI->PR |= (0x1<<4);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[4](); //执行中断回调
    }
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    //判断中断入口
    for (int i = 5; i < 9; i++)
    {
        if (EXTI->PR & (0x1<<i))
        {
            EXTI->PR |= (0x1<<i);   //清标志位
            
            g_IRQInterface.pf_EXTI_IRQHandler[i](); //执行中断回调
        }
    }
    
}


/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    //判断中断入口
    for (int i = 10; i < 15; i++)
    {
        if (EXTI->PR & (0x1<<i))
        {
            EXTI->PR |= (0x1<<i);   //清标志位
            
            g_IRQInterface.pf_EXTI_IRQHandler[i](); //执行中断回调
        }
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<1))
    {
        DMA1->IFCR |= (0x1<<0);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][0]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<5))
    {
        DMA1->IFCR |= (0x1<<4);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][1]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel3_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<9))
    {
        DMA1->IFCR |= (0x1<<8);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][2]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<13))
    {
        DMA1->IFCR |= (0x1<<12);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][3]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel5_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<17))
    {
        DMA1->IFCR |= (0x1<<16);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][4]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<21))
    {
        DMA1->IFCR |= (0x1<<20);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][5]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<25))
    {
        DMA1->IFCR |= (0x1<<24);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[0][6]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel1_IRQHandler(void)
{
    if (DMA2->ISR & (0x1<<1))
    {
        DMA2->IFCR |= (0x1<<0);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[1][0]();
    }
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel2_IRQHandler(void)
{
    if (DMA2->ISR & (0x1<<5))
    {
        DMA2->IFCR |= (0x1<<4);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[1][1]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel3_IRQHandler(void)
{
    if (DMA2->ISR & (0x1<<9))
    {
        DMA2->IFCR |= (0x1<<8);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[1][2]();
    }
    
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel4_5_IRQHandler(void)
{
    // Channel4
    if (DMA2->ISR & (0x1<<13))
    {
        DMA2->IFCR |= (0x1<<12);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[1][3]();
    }
    
    // Channel5
    if (DMA2->ISR & (0x1<<17))
    {
        DMA2->IFCR |= (0x1<<16);  //清全局标志位
        
        g_IRQInterface.pf_DMA_CH_IRQHandler[1][4]();
    }
    
}




void WWDG_IRQHandler            (void) { g_IRQInterface.pf_WWDG_IRQHandler           (); }
void PVD_IRQHandler             (void) { g_IRQInterface.pf_PVD_IRQHandler            (); }
void TAMPER_IRQHandler          (void) { g_IRQInterface.pf_TAMPER_IRQHandler         (); }
void RTC_IRQHandler             (void) { g_IRQInterface.pf_RTC_IRQHandler            (); }
void FLASH_IRQHandler           (void) { g_IRQInterface.pf_FLASH_IRQHandler          (); }
void RCC_IRQHandler             (void) { g_IRQInterface.pf_RCC_IRQHandler            (); }
void ADC1_2_IRQHandler          (void) { g_IRQInterface.pf_ADC1_2_IRQHandler         (); }
void USB_HP_CAN1_TX_IRQHandler  (void) { g_IRQInterface.pf_USB_HP_CAN1_TX_IRQHandler (); }
void USB_LP_CAN1_RX0_IRQHandler (void) { g_IRQInterface.pf_USB_LP_CAN1_RX0_IRQHandler(); }
void CAN1_RX1_IRQHandler        (void) { g_IRQInterface.pf_CAN1_RX1_IRQHandler       (); }
void CAN1_SCE_IRQHandler        (void) { g_IRQInterface.pf_CAN1_SCE_IRQHandler       (); }
void TIM1_BRK_IRQHandler        (void) { g_IRQInterface.pf_TIM1_BRK_IRQHandler       (); }
void TIM1_UP_IRQHandler         (void) { g_IRQInterface.pf_TIM1_UP_IRQHandler        (); }
void TIM1_TRG_COM_IRQHandler    (void) { g_IRQInterface.pf_TIM1_TRG_COM_IRQHandler   (); }
void TIM1_CC_IRQHandler         (void) { g_IRQInterface.pf_TIM1_CC_IRQHandler        (); }
void TIM2_IRQHandler            (void) { g_IRQInterface.pf_TIM2_IRQHandler           (); }
void TIM3_IRQHandler            (void) { g_IRQInterface.pf_TIM3_IRQHandler           (); }
void TIM4_IRQHandler            (void) { g_IRQInterface.pf_TIM4_IRQHandler           (); }
void I2C1_EV_IRQHandler         (void) { g_IRQInterface.pf_I2C1_EV_IRQHandler        (); }
void I2C1_ER_IRQHandler         (void) { g_IRQInterface.pf_I2C1_ER_IRQHandler        (); }
void I2C2_EV_IRQHandler         (void) { g_IRQInterface.pf_I2C2_EV_IRQHandler        (); }
void I2C2_ER_IRQHandler         (void) { g_IRQInterface.pf_I2C2_ER_IRQHandler        (); }
void SPI1_IRQHandler            (void) { g_IRQInterface.pf_SPI1_IRQHandler           (); }
void SPI2_IRQHandler            (void) { g_IRQInterface.pf_SPI2_IRQHandler           (); }
void USART1_IRQHandler          (void) { g_IRQInterface.pf_USART1_IRQHandler         (); }
void USART2_IRQHandler          (void) { g_IRQInterface.pf_USART2_IRQHandler         (); }
void USART3_IRQHandler          (void) { g_IRQInterface.pf_USART3_IRQHandler         (); }
void RTCAlarm_IRQHandler        (void) { g_IRQInterface.pf_RTCAlarm_IRQHandler       (); }
void USBWakeUp_IRQHandler       (void) { g_IRQInterface.pf_USBWakeUp_IRQHandler      (); }
void TIM8_BRK_IRQHandler        (void) { g_IRQInterface.pf_TIM8_BRK_IRQHandler       (); }
void TIM8_UP_IRQHandler         (void) { g_IRQInterface.pf_TIM8_UP_IRQHandler        (); }
void TIM8_TRG_COM_IRQHandler    (void) { g_IRQInterface.pf_TIM8_TRG_COM_IRQHandler   (); }
void TIM8_CC_IRQHandler         (void) { g_IRQInterface.pf_TIM8_CC_IRQHandler        (); }
void ADC3_IRQHandler            (void) { g_IRQInterface.pf_ADC3_IRQHandler           (); }
void FSMC_IRQHandler            (void) { g_IRQInterface.pf_FSMC_IRQHandler           (); }
void SDIO_IRQHandler            (void) { g_IRQInterface.pf_SDIO_IRQHandler           (); }
void TIM5_IRQHandler            (void) { g_IRQInterface.pf_TIM5_IRQHandler           (); }
void SPI3_IRQHandler            (void) { g_IRQInterface.pf_SPI3_IRQHandler           (); }
void UART4_IRQHandler           (void) { g_IRQInterface.pf_UART4_IRQHandler          (); }
void UART5_IRQHandler           (void) { g_IRQInterface.pf_UART5_IRQHandler          (); }
void TIM6_IRQHandler            (void) { g_IRQInterface.pf_TIM6_IRQHandler           (); }
void TIM7_IRQHandler            (void) { g_IRQInterface.pf_TIM7_IRQHandler           (); }



