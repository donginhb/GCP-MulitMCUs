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


      
//DMA通道
#define LPC_DMA_CH_COUNT                (8)

//IO中断组
#define LPC_GPIOINT_PORT_COUNT          (2)
#define LPC_GPIOINT_PIN_COUNT           (32)

typedef struct 
{
    void (*pf_SysTick_Update           )(void);
    void (*pf_SysTick_Handler          )(void);
    void (*pf_DAC_IRQHandler           )(void);
    void (*pf_M0APP_IRQHandler         )(void);
//    void (*pf_DMA_IRQHandler           )(void);
    void (*pf_FLASH_EEPROM_IRQHandler  )(void);
    void (*pf_ETH_IRQHandler           )(void);
    void (*pf_SDIO_IRQHandler          )(void);
    void (*pf_LCD_IRQHandler           )(void);
    void (*pf_USB0_IRQHandler          )(void);
    void (*pf_USB1_IRQHandler          )(void);
    void (*pf_SCT_IRQHandler           )(void);
    void (*pf_RIT_IRQHandler           )(void);
    void (*pf_TIMER0_IRQHandler        )(void);
    void (*pf_TIMER1_IRQHandler        )(void);
    void (*pf_TIMER2_IRQHandler        )(void);
    void (*pf_TIMER3_IRQHandler        )(void);
    void (*pf_MCPWM_IRQHandler         )(void);
    void (*pf_ADC0_IRQHandler          )(void);
    void (*pf_I2C0_IRQHandler          )(void);
    void (*pf_I2C1_IRQHandler          )(void);
    void (*pf_SPI_IRQHandler           )(void);
    void (*pf_ADC1_IRQHandler          )(void);
    void (*pf_SSP0_IRQHandler          )(void);
    void (*pf_SSP1_IRQHandler          )(void);
    void (*pf_UART0_IRQHandler         )(void);
    void (*pf_UART1_IRQHandler         )(void);
    void (*pf_UART2_IRQHandler         )(void);
    void (*pf_UART3_IRQHandler         )(void);
    void (*pf_I2S0_IRQHandler          )(void);
    void (*pf_I2S1_IRQHandler          )(void);
    void (*pf_SPIFI_IRQHandler         )(void);
    void (*pf_SGPIO_IRQHandler         )(void);
    void (*pf_GPIO0_IRQHandler         )(void);
    void (*pf_GPIO1_IRQHandler         )(void);
    void (*pf_GPIO2_IRQHandler         )(void);
    void (*pf_GPIO3_IRQHandler         )(void);
    void (*pf_GPIO4_IRQHandler         )(void);
    void (*pf_GPIO5_IRQHandler         )(void);
    void (*pf_GPIO6_IRQHandler         )(void);
    void (*pf_GPIO7_IRQHandler         )(void);
    void (*pf_GINT0_IRQHandler         )(void);
    void (*pf_GINT1_IRQHandler         )(void);
    void (*pf_EVRT_IRQHandler          )(void);
    void (*pf_CAN1_IRQHandler          )(void);
    void (*pf_ADCHS_IRQHandler         )(void);
    void (*pf_ATIMER_IRQHandler        )(void);
    void (*pf_RTC_IRQHandler           )(void);
    void (*pf_WDT_IRQHandler           )(void);
    void (*pf_M0SUB_IRQHandler         )(void);
    void (*pf_CAN0_IRQHandler          )(void);
    void (*pf_QEI_IRQHandler           )(void);
    
    void (*pf_DMA_CHx_IRQHandler[LPC_DMA_CH_COUNT])(void);
    
}LPC17XX_IRQ_INTERFACE;


static LPC17XX_IRQ_INTERFACE  g_IRQInterface = {0};  //全局中断回调指针接口



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
    g_IRQInterface.pf_SysTick_Update           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SysTick_Handler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_DAC_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_M0APP_IRQHandler         = HW_IRQ_NullEntry;
//    g_IRQInterface.pf_DMA_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_FLASH_EEPROM_IRQHandler  = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ETH_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SDIO_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_LCD_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USB0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USB1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SCT_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RIT_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER0_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER1_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER2_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER3_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_MCPWM_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SSP0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SSP1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART0_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART1_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART2_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART3_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2S0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2S1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPIFI_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SGPIO_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO0_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO1_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO2_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO3_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO4_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO5_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO6_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GPIO7_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GINT0_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_GINT1_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_EVRT_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CAN1_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADCHS_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ATIMER_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RTC_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_WDT_IRQHandler           = HW_IRQ_NullEntry;
    g_IRQInterface.pf_M0SUB_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CAN0_IRQHandler          = HW_IRQ_NullEntry;
    g_IRQInterface.pf_QEI_IRQHandler           = HW_IRQ_NullEntry;
    
    for (int i = 0; i < LPC_DMA_CH_COUNT; i++)
    {
        g_IRQInterface.pf_DMA_CHx_IRQHandler[i]  = HW_IRQ_NullEntry;
    }
    
    for (int i = 0; i < LPC_GPIOINT_PORT_COUNT; i++)
    {
        for (int j = 0; j < LPC_GPIOINT_PIN_COUNT; j++)
        {
            
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
    case LPC_IRQ_TRG_SYSTICK_UPDATE: g_IRQInterface.pf_SysTick_Update =  ptr; break;
    case LPC_IRQ_TRG_SYSTICK_OS:  g_IRQInterface.pf_SysTick_Handler =  ptr; break;
    
    //串口
    case LPC_IRQ_TRG_UART0: g_IRQInterface.pf_UART0_IRQHandler =  ptr; break;
    case LPC_IRQ_TRG_UART1: g_IRQInterface.pf_UART1_IRQHandler =  ptr; break;
    case LPC_IRQ_TRG_UART2: g_IRQInterface.pf_UART2_IRQHandler =  ptr; break;
    case LPC_IRQ_TRG_UART3: g_IRQInterface.pf_UART3_IRQHandler =  ptr; break;

    //DMA通道
    case LPC_IRQ_TRG_DMA_CH1: 
    case LPC_IRQ_TRG_DMA_CH2: 
    case LPC_IRQ_TRG_DMA_CH3: 
    case LPC_IRQ_TRG_DMA_CH4: 
    case LPC_IRQ_TRG_DMA_CH5: 
    case LPC_IRQ_TRG_DMA_CH6: 
    case LPC_IRQ_TRG_DMA_CH7: 
    case LPC_IRQ_TRG_DMA_CH8: g_IRQInterface.pf_DMA_CHx_IRQHandler[ulTrgSource - LPC_IRQ_TRG_DMA_CH1] =  ptr; break;
    
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
    case LPC_IRQ_TRG_SYSTICK_UPDATE: g_IRQInterface.pf_SysTick_Update =  HW_IRQ_NullEntry; break;
    case LPC_IRQ_TRG_SYSTICK_OS:  g_IRQInterface.pf_SysTick_Handler =  HW_IRQ_NullEntry; break;
    
    //串口
    case LPC_IRQ_TRG_UART0: g_IRQInterface.pf_UART0_IRQHandler =  HW_IRQ_NullEntry; break;
    case LPC_IRQ_TRG_UART1: g_IRQInterface.pf_UART1_IRQHandler =  HW_IRQ_NullEntry; break;
    case LPC_IRQ_TRG_UART2: g_IRQInterface.pf_UART2_IRQHandler =  HW_IRQ_NullEntry; break;
    case LPC_IRQ_TRG_UART3: g_IRQInterface.pf_UART3_IRQHandler =  HW_IRQ_NullEntry; break;

    //DMA通道
    case LPC_IRQ_TRG_DMA_CH1: 
    case LPC_IRQ_TRG_DMA_CH2: 
    case LPC_IRQ_TRG_DMA_CH3: 
    case LPC_IRQ_TRG_DMA_CH4: 
    case LPC_IRQ_TRG_DMA_CH5: 
    case LPC_IRQ_TRG_DMA_CH6: 
    case LPC_IRQ_TRG_DMA_CH7: 
    case LPC_IRQ_TRG_DMA_CH8: g_IRQInterface.pf_DMA_CHx_IRQHandler[ulTrgSource - LPC_IRQ_TRG_DMA_CH1] =  HW_IRQ_NullEntry; break;
    
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
  * @brief  This function handles DMA Handler.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
    for (int i = 0; i < LPC_DMA_CH_COUNT; i++)
    {
        //判断中断入口
        if (Chip_GPDMA_IntGetStatus(LPC_GPDMA, GPDMA_STAT_INTTC, i) == SET)
        {
            //清标志位
            Chip_GPDMA_ClearIntPending(LPC_GPDMA, GPDMA_STATCLR_INTTC, i);
            
            //执行回调
            g_IRQInterface.pf_DMA_CHx_IRQHandler[i]();
        }
    }
    
}



void DAC_IRQHandler           (void){g_IRQInterface.pf_DAC_IRQHandler         ();}
void M0APP_IRQHandler         (void){g_IRQInterface.pf_M0APP_IRQHandler       ();}
//void DMA_IRQHandler           (void){g_IRQInterface.pf_DMA_IRQHandler         ();}
void FLASH_EEPROM_IRQHandler  (void){g_IRQInterface.pf_FLASH_EEPROM_IRQHandler();}
void ETH_IRQHandler           (void){g_IRQInterface.pf_ETH_IRQHandler         ();}
void SDIO_IRQHandler          (void){g_IRQInterface.pf_SDIO_IRQHandler        ();}
void LCD_IRQHandler           (void){g_IRQInterface.pf_LCD_IRQHandler         ();}
void USB0_IRQHandler          (void){g_IRQInterface.pf_USB0_IRQHandler        ();}
void USB1_IRQHandler          (void){g_IRQInterface.pf_USB1_IRQHandler        ();}
void SCT_IRQHandler           (void){g_IRQInterface.pf_SCT_IRQHandler         ();}
void RIT_IRQHandler           (void){g_IRQInterface.pf_RIT_IRQHandler         ();}
void TIMER0_IRQHandler        (void){g_IRQInterface.pf_TIMER0_IRQHandler      ();}
void TIMER1_IRQHandler        (void){g_IRQInterface.pf_TIMER1_IRQHandler      ();}
void TIMER2_IRQHandler        (void){g_IRQInterface.pf_TIMER2_IRQHandler      ();}
void TIMER3_IRQHandler        (void){g_IRQInterface.pf_TIMER3_IRQHandler      ();}
void MCPWM_IRQHandler         (void){g_IRQInterface.pf_MCPWM_IRQHandler       ();}
void ADC0_IRQHandler          (void){g_IRQInterface.pf_ADC0_IRQHandler        ();}
void I2C0_IRQHandler          (void){g_IRQInterface.pf_I2C0_IRQHandler        ();}
void I2C1_IRQHandler          (void){g_IRQInterface.pf_I2C1_IRQHandler        ();}
void SPI_IRQHandler           (void){g_IRQInterface.pf_SPI_IRQHandler         ();}
void ADC1_IRQHandler          (void){g_IRQInterface.pf_ADC1_IRQHandler        ();}
void SSP0_IRQHandler          (void){g_IRQInterface.pf_SSP0_IRQHandler        ();}
void SSP1_IRQHandler          (void){g_IRQInterface.pf_SSP1_IRQHandler        ();}
void UART0_IRQHandler         (void){g_IRQInterface.pf_UART0_IRQHandler       ();}
void UART1_IRQHandler         (void){g_IRQInterface.pf_UART1_IRQHandler       ();}
void UART2_IRQHandler         (void){g_IRQInterface.pf_UART2_IRQHandler       ();}
void UART3_IRQHandler         (void){g_IRQInterface.pf_UART3_IRQHandler       ();}
void I2S0_IRQHandler          (void){g_IRQInterface.pf_I2S0_IRQHandler        ();}
void I2S1_IRQHandler          (void){g_IRQInterface.pf_I2S1_IRQHandler        ();}
void SPIFI_IRQHandler         (void){g_IRQInterface.pf_SPIFI_IRQHandler       ();}
void SGPIO_IRQHandler         (void){g_IRQInterface.pf_SGPIO_IRQHandler       ();}
void GPIO0_IRQHandler         (void){g_IRQInterface.pf_GPIO0_IRQHandler       ();}
void GPIO1_IRQHandler         (void){g_IRQInterface.pf_GPIO1_IRQHandler       ();}
void GPIO2_IRQHandler         (void){g_IRQInterface.pf_GPIO2_IRQHandler       ();}
void GPIO3_IRQHandler         (void){g_IRQInterface.pf_GPIO3_IRQHandler       ();}
void GPIO4_IRQHandler         (void){g_IRQInterface.pf_GPIO4_IRQHandler       ();}
void GPIO5_IRQHandler         (void){g_IRQInterface.pf_GPIO5_IRQHandler       ();}
void GPIO6_IRQHandler         (void){g_IRQInterface.pf_GPIO6_IRQHandler       ();}
void GPIO7_IRQHandler         (void){g_IRQInterface.pf_GPIO7_IRQHandler       ();}
void GINT0_IRQHandler         (void){g_IRQInterface.pf_GINT0_IRQHandler       ();}
void GINT1_IRQHandler         (void){g_IRQInterface.pf_GINT1_IRQHandler       ();}
void EVRT_IRQHandler          (void){g_IRQInterface.pf_EVRT_IRQHandler        ();}
void CAN1_IRQHandler          (void){g_IRQInterface.pf_CAN1_IRQHandler        ();}
void ADCHS_IRQHandler         (void){g_IRQInterface.pf_ADCHS_IRQHandler       ();}
void ATIMER_IRQHandler        (void){g_IRQInterface.pf_ATIMER_IRQHandler      ();}
void RTC_IRQHandler           (void){g_IRQInterface.pf_RTC_IRQHandler         ();}
void WDT_IRQHandler           (void){g_IRQInterface.pf_WDT_IRQHandler         ();}
void M0SUB_IRQHandler         (void){g_IRQInterface.pf_M0SUB_IRQHandler       ();}
void CAN0_IRQHandler          (void){g_IRQInterface.pf_CAN0_IRQHandler        ();}
void QEI_IRQHandler           (void){g_IRQInterface.pf_QEI_IRQHandler         ();}

