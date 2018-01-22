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


#define STM32_GPIO_INT_COUNT                 (16)


typedef struct 
{
    void (*pf_SysTick_Update                )(void);
    void (*pf_SysTick_Handler               )(void);
    void (*pf_PVD_IRQHandler                )(void);
    void (*pf_RTC_IRQHandler                )(void);
    void (*pf_FLASH_IRQHandler              )(void);
    void (*pf_RCC_IRQHandler                )(void);
    
    void (*pf_TS_IRQHandler                 )(void);
    void (*pf_DMA1_Channel1_IRQHandler      )(void);
    void (*pf_DMA1_Channel2_3_IRQHandler    )(void);
    void (*pf_DMA1_Channel4_5_IRQHandler    )(void);
    void (*pf_ADC1_COMP_IRQHandler          )(void);
    void (*pf_TIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pf_TIM1_CC_IRQHandler            )(void);
    void (*pf_TIM2_IRQHandler               )(void);
    void (*pf_TIM3_IRQHandler               )(void);
    void (*pf_TIM6_DAC_IRQHandler           )(void);
    void (*pf_TIM14_IRQHandler              )(void);
    void (*pf_TIM15_IRQHandler              )(void);
    void (*pf_TIM16_IRQHandler              )(void);
    void (*pf_TIM17_IRQHandler              )(void);
    void (*pf_I2C1_IRQHandler               )(void);
    void (*pf_I2C2_IRQHandler               )(void);
    void (*pf_SPI1_IRQHandler               )(void);
    void (*pf_SPI2_IRQHandler               )(void);
    void (*pf_USART1_IRQHandler             )(void);
    void (*pf_USART2_IRQHandler             )(void);
    
    void (*pf_EXTI_IRQHandler [STM32_GPIO_INT_COUNT])(void);
    
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
    g_IRQInterface.pf_SysTick_Update         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SysTick_Handler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_PVD_IRQHandler                 = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RTC_IRQHandler                 = HW_IRQ_NullEntry;
    g_IRQInterface.pf_FLASH_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RCC_IRQHandler                 = HW_IRQ_NullEntry;
    
    g_IRQInterface.pf_TS_IRQHandler                  = HW_IRQ_NullEntry;
    g_IRQInterface.pf_DMA1_Channel1_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_DMA1_Channel2_3_IRQHandler     = HW_IRQ_NullEntry;
    g_IRQInterface.pf_DMA1_Channel4_5_IRQHandler     = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC1_COMP_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_BRK_UP_TRG_COM_IRQHandler = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM1_CC_IRQHandler             = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM2_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM3_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM6_DAC_IRQHandler            = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM14_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM15_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM16_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIM17_IRQHandler               = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C2_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI1_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI2_IRQHandler                = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USART1_IRQHandler              = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USART2_IRQHandler              = HW_IRQ_NullEntry;
    
    //EXTI Group
    for (int i = 0; i < STM32_GPIO_INT_COUNT; i++)
    {
        g_IRQInterface.pf_EXTI_IRQHandler[i] = HW_IRQ_NullEntry;
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
    case STM32F0_IRQ_TRG_SYSTICK_UPDATE:        g_IRQInterface.pf_SysTick_Update                  = ptr; break;
    case STM32F0_IRQ_TRG_SYSTICK_OS:            g_IRQInterface.pf_SysTick_Handler                 = ptr; break;
    
    case STM32F0_IRQ_TRG_PVD:                   g_IRQInterface.pf_PVD_IRQHandler                  = ptr; break;
    case STM32F0_IRQ_TRG_RTC:                   g_IRQInterface.pf_RTC_IRQHandler                  = ptr; break;
    case STM32F0_IRQ_TRG_FLASH:                 g_IRQInterface.pf_FLASH_IRQHandler                = ptr; break;
    case STM32F0_IRQ_TRG_RCC:                   g_IRQInterface.pf_RCC_IRQHandler                  = ptr; break;
    
    case STM32F0_IRQ_TRG_EXTI0:
    case STM32F0_IRQ_TRG_EXTI1:
    case STM32F0_IRQ_TRG_EXTI2:
    case STM32F0_IRQ_TRG_EXTI3:
    case STM32F0_IRQ_TRG_EXTI4:
    case STM32F0_IRQ_TRG_EXTI5:
    case STM32F0_IRQ_TRG_EXTI6:
    case STM32F0_IRQ_TRG_EXTI7:
    case STM32F0_IRQ_TRG_EXTI8:
    case STM32F0_IRQ_TRG_EXTI9:
    case STM32F0_IRQ_TRG_EXTI10:
    case STM32F0_IRQ_TRG_EXTI11:
    case STM32F0_IRQ_TRG_EXTI12:
    case STM32F0_IRQ_TRG_EXTI13:
    case STM32F0_IRQ_TRG_EXTI14:
    case STM32F0_IRQ_TRG_EXTI15:                g_IRQInterface.pf_EXTI_IRQHandler[ulTrgSource-STM32F0_IRQ_TRG_EXTI0] = ptr; break;
    
    case STM32F0_IRQ_TRG_TS:                    g_IRQInterface.pf_TS_IRQHandler                   = ptr; break;
    case STM32F0_IRQ_TRG_DMA1_Channel1:         g_IRQInterface.pf_DMA1_Channel1_IRQHandler        = ptr; break;
    case STM32F0_IRQ_TRG_DMA1_Channel2_3:       g_IRQInterface.pf_DMA1_Channel2_3_IRQHandler      = ptr; break;
    case STM32F0_IRQ_TRG_DMA1_Channel4_5:       g_IRQInterface.pf_DMA1_Channel4_5_IRQHandler      = ptr; break;
    case STM32F0_IRQ_TRG_ADC1_COMP:             g_IRQInterface.pf_ADC1_COMP_IRQHandler            = ptr; break;
    case STM32F0_IRQ_TRG_TIM1_BRK_UP_TRG_COM:   g_IRQInterface.pf_TIM1_BRK_UP_TRG_COM_IRQHandler  = ptr; break;
    case STM32F0_IRQ_TRG_TIM1_CC:               g_IRQInterface.pf_TIM1_CC_IRQHandler              = ptr; break;
    case STM32F0_IRQ_TRG_TIM2:                  g_IRQInterface.pf_TIM2_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_TIM3:                  g_IRQInterface.pf_TIM3_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_TIM6_DAC:              g_IRQInterface.pf_TIM6_DAC_IRQHandler             = ptr; break;
    case STM32F0_IRQ_TRG_TIM14:                 g_IRQInterface.pf_TIM14_IRQHandler                = ptr; break;
    case STM32F0_IRQ_TRG_TIM15:                 g_IRQInterface.pf_TIM15_IRQHandler                = ptr; break;
    case STM32F0_IRQ_TRG_TIM16:                 g_IRQInterface.pf_TIM16_IRQHandler                = ptr; break;
    case STM32F0_IRQ_TRG_TIM17:                 g_IRQInterface.pf_TIM17_IRQHandler                = ptr; break;
    case STM32F0_IRQ_TRG_I2C1:                  g_IRQInterface.pf_I2C1_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_I2C2:                  g_IRQInterface.pf_I2C2_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_SPI1:                  g_IRQInterface.pf_SPI1_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_SPI2:                  g_IRQInterface.pf_SPI2_IRQHandler                 = ptr; break;
    case STM32F0_IRQ_TRG_USART1:                g_IRQInterface.pf_USART1_IRQHandler               = ptr; break;
    case STM32F0_IRQ_TRG_USART2:                g_IRQInterface.pf_USART2_IRQHandler               = ptr; break;
    
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
    case STM32F0_IRQ_TRG_SYSTICK_UPDATE: g_IRQInterface.pf_SysTick_Update =  HW_IRQ_NullEntry; break;
    case STM32F0_IRQ_TRG_SYSTICK_OS:  g_IRQInterface.pf_SysTick_Handler =  HW_IRQ_NullEntry; break;
    

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
void EXTI0_1_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<0))
    {
        EXTI->PR |= (0x1<<0);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[0](); //执行中断回调
    }
    
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
void EXTI2_3_IRQHandler(void)
{
    //判断中断入口
    if (EXTI->PR & (0x1<<2))
    {
        EXTI->PR |= (0x1<<2);   //清标志位
        
        g_IRQInterface.pf_EXTI_IRQHandler[2](); //执行中断回调
    }
    
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
void EXTI4_15_IRQHandler(void)
{
    
    for (int i = 4; i < 16; i++)
    {
        if (EXTI->PR & (0x1<<i))
        {
            EXTI->PR |= (0x1<<i);   //清标志位
            
            g_IRQInterface.pf_EXTI_IRQHandler[i](); //执行中断回调
        }
            
    }
    
}




void PVD_IRQHandler                 (void){g_IRQInterface.pf_PVD_IRQHandler                ();}
void RTC_IRQHandler                 (void){g_IRQInterface.pf_RTC_IRQHandler                ();}
void FLASH_IRQHandler               (void){g_IRQInterface.pf_FLASH_IRQHandler              ();}
void RCC_IRQHandler                 (void){g_IRQInterface.pf_RCC_IRQHandler                ();}

void TS_IRQHandler                  (void){g_IRQInterface.pf_TS_IRQHandler                 ();}
void DMA1_Channel1_IRQHandler       (void){g_IRQInterface.pf_DMA1_Channel1_IRQHandler      ();}
void DMA1_Channel2_3_IRQHandler     (void){g_IRQInterface.pf_DMA1_Channel2_3_IRQHandler    ();}
void DMA1_Channel4_5_IRQHandler     (void){g_IRQInterface.pf_DMA1_Channel4_5_IRQHandler    ();}
void ADC1_COMP_IRQHandler           (void){g_IRQInterface.pf_ADC1_COMP_IRQHandler          ();}
void TIM1_BRK_UP_TRG_COM_IRQHandler (void){g_IRQInterface.pf_TIM1_BRK_UP_TRG_COM_IRQHandler();}
void TIM1_CC_IRQHandler             (void){g_IRQInterface.pf_TIM1_CC_IRQHandler            ();}
void TIM2_IRQHandler                (void){g_IRQInterface.pf_TIM2_IRQHandler               ();}
void TIM3_IRQHandler                (void){g_IRQInterface.pf_TIM3_IRQHandler               ();}
void TIM6_DAC_IRQHandler            (void){g_IRQInterface.pf_TIM6_DAC_IRQHandler           ();}
void TIM14_IRQHandler               (void){g_IRQInterface.pf_TIM14_IRQHandler              ();}
void TIM15_IRQHandler               (void){g_IRQInterface.pf_TIM15_IRQHandler              ();}
void TIM16_IRQHandler               (void){g_IRQInterface.pf_TIM16_IRQHandler              ();}
void TIM17_IRQHandler               (void){g_IRQInterface.pf_TIM17_IRQHandler              ();}
void I2C1_IRQHandler                (void){g_IRQInterface.pf_I2C1_IRQHandler               ();}
void I2C2_IRQHandler                (void){g_IRQInterface.pf_I2C2_IRQHandler               ();}
void SPI1_IRQHandler                (void){g_IRQInterface.pf_SPI1_IRQHandler               ();}
void SPI2_IRQHandler                (void){g_IRQInterface.pf_SPI2_IRQHandler               ();}
void USART1_IRQHandler              (void){g_IRQInterface.pf_USART1_IRQHandler             ();}
void USART2_IRQHandler              (void){g_IRQInterface.pf_USART2_IRQHandler             ();}
