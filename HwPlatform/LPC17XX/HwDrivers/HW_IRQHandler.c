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


#ifdef CHIP_LPC175X_6X

/* On the LPC1769, the GPIO interrupts share the EINT3 vector. */
#define GPIO_IRQHandler             EINT3_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    EINT3_IRQn    /* GPIO interrupt NVIC interrupt name */

#elif defined(CHIP_LPC177X_8X) || defined(CHIP_LPC40XX)

#define GPIO_IRQHandler             GPIO_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    GPIO_IRQn    /* GPIO interrupt NVIC interrupt name */

#endif

      
//DMA通道
#define LPC_DMA_CH_COUNT            (8)

//IO中断组
#define LPC_GPIOINT_PORT_COUNT      (2)
#define LPC_GPIOINT_PIN_COUNT       (32)

//MCPWM中断组
#define LPC_MCPWM_COUNT             (3)

typedef struct 
{
    void (*pf_SysTick_Update        )(void);
    void (*pf_SysTick_Handler       )(void);
    void (*pf_WDT_IRQHandler        )(void);
    void (*pf_TIMER0_IRQHandler     )(void);
    void (*pf_TIMER1_IRQHandler     )(void);
    void (*pf_TIMER2_IRQHandler     )(void);
    void (*pf_TIMER3_IRQHandler     )(void);
    void (*pf_UART0_IRQHandler      )(void);
    void (*pf_UART1_IRQHandler      )(void);
    void (*pf_UART2_IRQHandler      )(void);
    void (*pf_UART3_IRQHandler      )(void);
    void (*pf_PWM1_IRQHandler       )(void);
    void (*pf_I2C0_IRQHandler       )(void);
    void (*pf_I2C1_IRQHandler       )(void);
    void (*pf_I2C2_IRQHandler       )(void);
    void (*pf_SPI_IRQHandler        )(void);
    void (*pf_SSP0_IRQHandler       )(void);
    void (*pf_SSP1_IRQHandler       )(void);
    void (*pf_PLL0_IRQHandler       )(void);
    void (*pf_RTC_IRQHandler        )(void);
    void (*pf_EINT0_IRQHandler      )(void);
    void (*pf_EINT1_IRQHandler      )(void);
    void (*pf_EINT2_IRQHandler      )(void);
    void (*pf_ADC_IRQHandler        )(void);
    void (*pf_BOD_IRQHandler        )(void);
    void (*pf_USB_IRQHandler        )(void);
    void (*pf_CAN_IRQHandler        )(void);
    void (*pf_I2S_IRQHandler        )(void);
    void (*pf_ENET_IRQHandler       )(void);
    void (*pf_RIT_IRQHandler        )(void);
//    void (*pf_MCPWM_IRQHandler      )(void);
    void (*pf_QEI_IRQHandler        )(void);
    void (*pf_PLL1_IRQHandler       )(void);
    void (*pf_USBActivity_IRQHandler)(void);
    void (*pf_CANActivity_IRQHandler)(void);
    
    void (*pf_DMA_CHx_IRQHandler[LPC_DMA_CH_COUNT])(void);
    void (*pf_GPIO_IRQHandler[LPC_GPIOINT_PORT_COUNT][LPC_GPIOINT_PIN_COUNT])(void);
    void (*pf_MCPWM_IRQHandler[LPC_MCPWM_COUNT])(void);
    
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
    g_IRQInterface.pf_SysTick_Update         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SysTick_Handler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_WDT_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER0_IRQHandler      = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER1_IRQHandler      = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER2_IRQHandler      = HW_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER3_IRQHandler      = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART0_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART1_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART2_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_UART3_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_PWM1_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C0_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2C2_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SPI_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SSP0_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_SSP1_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_PLL0_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RTC_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_EINT0_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_EINT1_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_EINT2_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ADC_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_BOD_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USB_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CAN_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_I2S_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_ENET_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_RIT_IRQHandler         = HW_IRQ_NullEntry;
    //g_IRQInterface.pf_MCPWM_IRQHandler       = HW_IRQ_NullEntry;
    g_IRQInterface.pf_QEI_IRQHandler         = HW_IRQ_NullEntry;
    g_IRQInterface.pf_PLL1_IRQHandler        = HW_IRQ_NullEntry;
    g_IRQInterface.pf_USBActivity_IRQHandler = HW_IRQ_NullEntry;
    g_IRQInterface.pf_CANActivity_IRQHandler = HW_IRQ_NullEntry;
    
    for (int i = 0; i < LPC_DMA_CH_COUNT; i++)
    {
        g_IRQInterface.pf_DMA_CHx_IRQHandler[i]  = HW_IRQ_NullEntry;
    }
    
    for (int i = 0; i < LPC_GPIOINT_PORT_COUNT; i++)
    {
        for (int j = 0; j < LPC_GPIOINT_PIN_COUNT; j++)
        {
            g_IRQInterface.pf_GPIO_IRQHandler[i][j] = HW_IRQ_NullEntry;
        }
    }
    
    for (int i = 0; i < LPC_MCPWM_COUNT; i++)
    {
        g_IRQInterface.pf_MCPWM_IRQHandler[i] = HW_IRQ_NullEntry;
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
    
    //GPIO中断
    case LPC_IRQ_TRG_GPIOINT0_00:
    case LPC_IRQ_TRG_GPIOINT0_01:
    case LPC_IRQ_TRG_GPIOINT0_02:
    case LPC_IRQ_TRG_GPIOINT0_03:
    case LPC_IRQ_TRG_GPIOINT0_04:
    case LPC_IRQ_TRG_GPIOINT0_05:
    case LPC_IRQ_TRG_GPIOINT0_06:
    case LPC_IRQ_TRG_GPIOINT0_07:
    case LPC_IRQ_TRG_GPIOINT0_08:
    case LPC_IRQ_TRG_GPIOINT0_09:
    case LPC_IRQ_TRG_GPIOINT0_10:
    case LPC_IRQ_TRG_GPIOINT0_11:
    case LPC_IRQ_TRG_GPIOINT0_12:
    case LPC_IRQ_TRG_GPIOINT0_13:
    case LPC_IRQ_TRG_GPIOINT0_14:
    case LPC_IRQ_TRG_GPIOINT0_15:
    case LPC_IRQ_TRG_GPIOINT0_16:
    case LPC_IRQ_TRG_GPIOINT0_17:
    case LPC_IRQ_TRG_GPIOINT0_18:
    case LPC_IRQ_TRG_GPIOINT0_19:
    case LPC_IRQ_TRG_GPIOINT0_20:
    case LPC_IRQ_TRG_GPIOINT0_21:
    case LPC_IRQ_TRG_GPIOINT0_22:
    case LPC_IRQ_TRG_GPIOINT0_23:
    case LPC_IRQ_TRG_GPIOINT0_24:
    case LPC_IRQ_TRG_GPIOINT0_25:
    case LPC_IRQ_TRG_GPIOINT0_26:
    case LPC_IRQ_TRG_GPIOINT0_27:
    case LPC_IRQ_TRG_GPIOINT0_28:
    case LPC_IRQ_TRG_GPIOINT0_29:
    case LPC_IRQ_TRG_GPIOINT0_30:
    case LPC_IRQ_TRG_GPIOINT0_31:

    case LPC_IRQ_TRG_GPIOINT2_00:
    case LPC_IRQ_TRG_GPIOINT2_01:
    case LPC_IRQ_TRG_GPIOINT2_02:
    case LPC_IRQ_TRG_GPIOINT2_03:
    case LPC_IRQ_TRG_GPIOINT2_04:
    case LPC_IRQ_TRG_GPIOINT2_05:
    case LPC_IRQ_TRG_GPIOINT2_06:
    case LPC_IRQ_TRG_GPIOINT2_07:
    case LPC_IRQ_TRG_GPIOINT2_08:
    case LPC_IRQ_TRG_GPIOINT2_09:
    case LPC_IRQ_TRG_GPIOINT2_10:
    case LPC_IRQ_TRG_GPIOINT2_11:
    case LPC_IRQ_TRG_GPIOINT2_12:
    case LPC_IRQ_TRG_GPIOINT2_13:
    case LPC_IRQ_TRG_GPIOINT2_14:
    case LPC_IRQ_TRG_GPIOINT2_15:
    case LPC_IRQ_TRG_GPIOINT2_16:
    case LPC_IRQ_TRG_GPIOINT2_17:
    case LPC_IRQ_TRG_GPIOINT2_18:
    case LPC_IRQ_TRG_GPIOINT2_19:
    case LPC_IRQ_TRG_GPIOINT2_20:
    case LPC_IRQ_TRG_GPIOINT2_21:
    case LPC_IRQ_TRG_GPIOINT2_22:
    case LPC_IRQ_TRG_GPIOINT2_23:
    case LPC_IRQ_TRG_GPIOINT2_24:
    case LPC_IRQ_TRG_GPIOINT2_25:
    case LPC_IRQ_TRG_GPIOINT2_26:
    case LPC_IRQ_TRG_GPIOINT2_27:
    case LPC_IRQ_TRG_GPIOINT2_28:
    case LPC_IRQ_TRG_GPIOINT2_29:
    case LPC_IRQ_TRG_GPIOINT2_30:
    case LPC_IRQ_TRG_GPIOINT2_31: g_IRQInterface.pf_GPIO_IRQHandler[0][ulTrgSource - LPC_IRQ_TRG_GPIOINT0_00] = ptr; break;  //通过数组越界的方式去访问二维数组
    
    case LPC_IRQ_TRG_ENET: g_IRQInterface.pf_ENET_IRQHandler = ptr; break;
    
    case LPC_IRQ_TRG_TIME0: g_IRQInterface.pf_TIMER0_IRQHandler = ptr;
    case LPC_IRQ_TRG_TIME1: g_IRQInterface.pf_TIMER1_IRQHandler = ptr;
    case LPC_IRQ_TRG_TIME2: g_IRQInterface.pf_TIMER2_IRQHandler = ptr;
    case LPC_IRQ_TRG_TIME3: g_IRQInterface.pf_TIMER3_IRQHandler = ptr;
    
    case LPC_IRQ_TRG_MCPWM0: g_IRQInterface.pf_MCPWM_IRQHandler[0] = ptr;
    case LPC_IRQ_TRG_MCPWM1: g_IRQInterface.pf_MCPWM_IRQHandler[1] = ptr;
    case LPC_IRQ_TRG_MCPWM2: g_IRQInterface.pf_MCPWM_IRQHandler[2] = ptr;
    
    case LPC_IRQ_TRG_PWM0:  g_IRQInterface.pf_PWM1_IRQHandler = ptr;
    
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
    
    //GPIO中断
    case LPC_IRQ_TRG_GPIOINT0_00:
    case LPC_IRQ_TRG_GPIOINT0_01:
    case LPC_IRQ_TRG_GPIOINT0_02:
    case LPC_IRQ_TRG_GPIOINT0_03:
    case LPC_IRQ_TRG_GPIOINT0_04:
    case LPC_IRQ_TRG_GPIOINT0_05:
    case LPC_IRQ_TRG_GPIOINT0_06:
    case LPC_IRQ_TRG_GPIOINT0_07:
    case LPC_IRQ_TRG_GPIOINT0_08:
    case LPC_IRQ_TRG_GPIOINT0_09:
    case LPC_IRQ_TRG_GPIOINT0_10:
    case LPC_IRQ_TRG_GPIOINT0_11:
    case LPC_IRQ_TRG_GPIOINT0_12:
    case LPC_IRQ_TRG_GPIOINT0_13:
    case LPC_IRQ_TRG_GPIOINT0_14:
    case LPC_IRQ_TRG_GPIOINT0_15:
    case LPC_IRQ_TRG_GPIOINT0_16:
    case LPC_IRQ_TRG_GPIOINT0_17:
    case LPC_IRQ_TRG_GPIOINT0_18:
    case LPC_IRQ_TRG_GPIOINT0_19:
    case LPC_IRQ_TRG_GPIOINT0_20:
    case LPC_IRQ_TRG_GPIOINT0_21:
    case LPC_IRQ_TRG_GPIOINT0_22:
    case LPC_IRQ_TRG_GPIOINT0_23:
    case LPC_IRQ_TRG_GPIOINT0_24:
    case LPC_IRQ_TRG_GPIOINT0_25:
    case LPC_IRQ_TRG_GPIOINT0_26:
    case LPC_IRQ_TRG_GPIOINT0_27:
    case LPC_IRQ_TRG_GPIOINT0_28:
    case LPC_IRQ_TRG_GPIOINT0_29:
    case LPC_IRQ_TRG_GPIOINT0_30:
    case LPC_IRQ_TRG_GPIOINT0_31:

    case LPC_IRQ_TRG_GPIOINT2_00:
    case LPC_IRQ_TRG_GPIOINT2_01:
    case LPC_IRQ_TRG_GPIOINT2_02:
    case LPC_IRQ_TRG_GPIOINT2_03:
    case LPC_IRQ_TRG_GPIOINT2_04:
    case LPC_IRQ_TRG_GPIOINT2_05:
    case LPC_IRQ_TRG_GPIOINT2_06:
    case LPC_IRQ_TRG_GPIOINT2_07:
    case LPC_IRQ_TRG_GPIOINT2_08:
    case LPC_IRQ_TRG_GPIOINT2_09:
    case LPC_IRQ_TRG_GPIOINT2_10:
    case LPC_IRQ_TRG_GPIOINT2_11:
    case LPC_IRQ_TRG_GPIOINT2_12:
    case LPC_IRQ_TRG_GPIOINT2_13:
    case LPC_IRQ_TRG_GPIOINT2_14:
    case LPC_IRQ_TRG_GPIOINT2_15:
    case LPC_IRQ_TRG_GPIOINT2_16:
    case LPC_IRQ_TRG_GPIOINT2_17:
    case LPC_IRQ_TRG_GPIOINT2_18:
    case LPC_IRQ_TRG_GPIOINT2_19:
    case LPC_IRQ_TRG_GPIOINT2_20:
    case LPC_IRQ_TRG_GPIOINT2_21:
    case LPC_IRQ_TRG_GPIOINT2_22:
    case LPC_IRQ_TRG_GPIOINT2_23:
    case LPC_IRQ_TRG_GPIOINT2_24:
    case LPC_IRQ_TRG_GPIOINT2_25:
    case LPC_IRQ_TRG_GPIOINT2_26:
    case LPC_IRQ_TRG_GPIOINT2_27:
    case LPC_IRQ_TRG_GPIOINT2_28:
    case LPC_IRQ_TRG_GPIOINT2_29:
    case LPC_IRQ_TRG_GPIOINT2_30:
    case LPC_IRQ_TRG_GPIOINT2_31: g_IRQInterface.pf_GPIO_IRQHandler[0][ulTrgSource - LPC_IRQ_TRG_GPIOINT0_00] = HW_IRQ_NullEntry; break;  //通过数组越界的方式去访问二维数组
    
    case LPC_IRQ_TRG_TIME0: g_IRQInterface.pf_TIMER0_IRQHandler = HW_IRQ_NullEntry;
    case LPC_IRQ_TRG_TIME1: g_IRQInterface.pf_TIMER1_IRQHandler = HW_IRQ_NullEntry;
    case LPC_IRQ_TRG_TIME2: g_IRQInterface.pf_TIMER2_IRQHandler = HW_IRQ_NullEntry;
    case LPC_IRQ_TRG_TIME3: g_IRQInterface.pf_TIMER3_IRQHandler = HW_IRQ_NullEntry;
    
    case LPC_IRQ_TRG_MCPWM0: g_IRQInterface.pf_MCPWM_IRQHandler[0] = HW_IRQ_NullEntry;
    case LPC_IRQ_TRG_MCPWM1: g_IRQInterface.pf_MCPWM_IRQHandler[1] = HW_IRQ_NullEntry;
    case LPC_IRQ_TRG_MCPWM2: g_IRQInterface.pf_MCPWM_IRQHandler[2] = HW_IRQ_NullEntry;
    
    case LPC_IRQ_TRG_PWM0:  g_IRQInterface.pf_PWM1_IRQHandler = HW_IRQ_NullEntry;
    
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


/**
  * @brief  This function handles GPIO Handler.
  * @param  None
  * @retval None
  */
void GPIO_IRQHandler(void)
{
    LPC_GPIOINT_PORT_T GpioIntPortGroup[LPC_GPIOINT_PORT_COUNT] = {GPIOINT_PORT0, GPIOINT_PORT2};
    
    for (int iPortIndex = 0; iPortIndex < LPC_GPIOINT_PORT_COUNT; iPortIndex++)
    {
        if (Chip_GPIOINT_IsIntPending(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]))
        {
            //获取中断置位状态
            uint32_t IntStatus = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]) | 
                                 Chip_GPIOINT_GetStatusRising (LPC_GPIOINT, GpioIntPortGroup[iPortIndex]);
            
            //确认中断入口
            for (int iPinIndex = 0; iPinIndex < LPC_GPIOINT_PIN_COUNT; iPinIndex++)
            {
                if (IntStatus & (0x1<<iPinIndex))
                {
                    IntStatus &= ~(0x1<<iPinIndex);
                    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GpioIntPortGroup[iPortIndex], (0x1<<iPinIndex));    //清中断
                    
                    //执行中断回调
                    g_IRQInterface.pf_GPIO_IRQHandler[iPortIndex][iPinIndex]();
                    
                    if (IntStatus == 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    
}



/**
  * @brief  This function handles MCPWM Handler.
  * @param  None
  * @retval None
  */
void MCPWM_IRQHandler(void)
{
    for (int i = 0; i < LPC_MCPWM_COUNT; i++)
    {
        g_IRQInterface.pf_MCPWM_IRQHandler[i]();
    }
    
}




void WDT_IRQHandler         (void) { g_IRQInterface.pf_WDT_IRQHandler         (); }
void TIMER0_IRQHandler      (void) { g_IRQInterface.pf_TIMER0_IRQHandler      (); }
void TIMER1_IRQHandler      (void) { g_IRQInterface.pf_TIMER1_IRQHandler      (); }
void TIMER2_IRQHandler      (void) { g_IRQInterface.pf_TIMER2_IRQHandler      (); }
void TIMER3_IRQHandler      (void) { g_IRQInterface.pf_TIMER3_IRQHandler      (); }
void UART0_IRQHandler       (void) { g_IRQInterface.pf_UART0_IRQHandler       (); }
void UART1_IRQHandler       (void) { g_IRQInterface.pf_UART1_IRQHandler       (); }
void UART2_IRQHandler       (void) { g_IRQInterface.pf_UART2_IRQHandler       (); }
void UART3_IRQHandler       (void) { g_IRQInterface.pf_UART3_IRQHandler       (); }
void PWM1_IRQHandler        (void) { g_IRQInterface.pf_PWM1_IRQHandler        (); }
void I2C0_IRQHandler        (void) { g_IRQInterface.pf_I2C0_IRQHandler        (); }
void I2C1_IRQHandler        (void) { g_IRQInterface.pf_I2C1_IRQHandler        (); }
void I2C2_IRQHandler        (void) { g_IRQInterface.pf_I2C2_IRQHandler        (); }
void SPI_IRQHandler         (void) { g_IRQInterface.pf_SPI_IRQHandler         (); }
void SSP0_IRQHandler        (void) { g_IRQInterface.pf_SSP0_IRQHandler        (); }
void SSP1_IRQHandler        (void) { g_IRQInterface.pf_SSP1_IRQHandler        (); }
void PLL0_IRQHandler        (void) { g_IRQInterface.pf_PLL0_IRQHandler        (); }
void RTC_IRQHandler         (void) { g_IRQInterface.pf_RTC_IRQHandler         (); }
void EINT0_IRQHandler       (void) { g_IRQInterface.pf_EINT0_IRQHandler       (); }
void EINT1_IRQHandler       (void) { g_IRQInterface.pf_EINT1_IRQHandler       (); }
void EINT2_IRQHandler       (void) { g_IRQInterface.pf_EINT2_IRQHandler       (); }
//void EINT3_IRQHandler       (void) { g_IRQInterface.pf_EINT3_IRQHandler       (); }
void ADC_IRQHandler         (void) { g_IRQInterface.pf_ADC_IRQHandler         (); }
void BOD_IRQHandler         (void) { g_IRQInterface.pf_BOD_IRQHandler         (); }
void USB_IRQHandler         (void) { g_IRQInterface.pf_USB_IRQHandler         (); }
void CAN_IRQHandler         (void) { g_IRQInterface.pf_CAN_IRQHandler         (); }
//void DMA_IRQHandler         (void) { g_IRQInterface.pf_DMA_IRQHandler         (); }
void I2S_IRQHandler         (void) { g_IRQInterface.pf_I2S_IRQHandler         (); }
void ENET_IRQHandler        (void) { g_IRQInterface.pf_ENET_IRQHandler        (); }
void RIT_IRQHandler         (void) { g_IRQInterface.pf_RIT_IRQHandler         (); }
//void MCPWM_IRQHandler       (void) { g_IRQInterface.pf_MCPWM_IRQHandler       (); }
void QEI_IRQHandler         (void) { g_IRQInterface.pf_QEI_IRQHandler         (); }
void PLL1_IRQHandler        (void) { g_IRQInterface.pf_PLL1_IRQHandler        (); }
void USBActivity_IRQHandler (void) { g_IRQInterface.pf_USBActivity_IRQHandler (); }
void CANActivity_IRQHandler (void) { g_IRQInterface.pf_CANActivity_IRQHandler (); }



#if 0
/**
  * @brief  This function handles WDT Handler.
  * @param  None
  * @retval None
  */
void WDT_IRQHandler(void)
{
    g_IRQInterface.pf_WDT_IRQHandler();

}


/**
  * @brief  This function handles TIMER0 Handler.
  * @param  None
  * @retval None
  */
void TIMER0_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER0_IRQHandler();

}


/**
  * @brief  This function handles TIMER1 Handler.
  * @param  None
  * @retval None
  */
void TIMER1_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER1_IRQHandler();

}


/**
  * @brief  This function handles TIMER2 Handler.
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER2_IRQHandler();
    
}


/**
  * @brief  This function handles TIMER3 Handler.
  * @param  None
  * @retval None
  */
void TIMER3_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER3_IRQHandler();
    
}


/**
  * @brief  This function handles UART0 Handler.
  * @param  None
  * @retval None
  */
void UART0_IRQHandler(void)
{
    g_IRQInterface.pf_UART0_IRQHandler();

}


/**
  * @brief  This function handles UART1 Handler.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    g_IRQInterface.pf_UART1_IRQHandler();
    
}


/**
  * @brief  This function handles UART2 Handler.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    g_IRQInterface.pf_UART2_IRQHandler();

}


/**
  * @brief  This function handles UART3 Handler.
  * @param  None
  * @retval None
  */
void UART3_IRQHandler(void)
{
    g_IRQInterface.pf_UART3_IRQHandler();

}


/**
  * @brief  This function handles PWM1 Handler.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{
    g_IRQInterface.pf_PWM1_IRQHandler();

}


/**
  * @brief  This function handles I2C0 Handler.
  * @param  None
  * @retval None
  */
void I2C0_IRQHandler(void)
{
    g_IRQInterface.pf_I2C0_IRQHandler();

}


/**
  * @brief  This function handles I2C1 Handler.
  * @param  None
  * @retval None
  */
void I2C1_IRQHandler(void)
{
    g_IRQInterface.pf_I2C1_IRQHandler();

}


/**
  * @brief  This function handles I2C2 Handler.
  * @param  None
  * @retval None
  */
void I2C2_IRQHandler(void)
{
    g_IRQInterface.pf_I2C2_IRQHandler();

}


/**
  * @brief  This function handles SPI Handler.
  * @param  None
  * @retval None
  */
void SPI_IRQHandler(void)
{
    g_IRQInterface.pf_SPI_IRQHandler();

}


/**
  * @brief  This function handles SSP0 Handler.
  * @param  None
  * @retval None
  */
void SSP0_IRQHandler(void)
{
    g_IRQInterface.pf_SSP0_IRQHandler();

}


/**
  * @brief  This function handles SSP1 Handler.
  * @param  None
  * @retval None
  */
void SSP1_IRQHandler(void)
{
    g_IRQInterface.pf_SSP1_IRQHandler();

}


/**
  * @brief  This function handles PLL0 Handler.
  * @param  None
  * @retval None
  */
void PLL0_IRQHandler(void)
{
    g_IRQInterface.pf_PLL0_IRQHandler();

}


/**
  * @brief  This function handles RTC Handler.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    g_IRQInterface.pf_RTC_IRQHandler();

}


/**
  * @brief  This function handles EINT0 Handler.
  * @param  None
  * @retval None
  */
void EINT0_IRQHandler(void)
{
    g_IRQInterface.pf_EINT0_IRQHandler();

}


/**
  * @brief  This function handles EINT1 Handler.
  * @param  None
  * @retval None
  */
void EINT1_IRQHandler(void)
{
    g_IRQInterface.pf_EINT1_IRQHandler();

}


/**
  * @brief  This function handles EINT2 Handler.
  * @param  None
  * @retval None
  */
void EINT2_IRQHandler(void)
{
    g_IRQInterface.pf_EINT2_IRQHandler();

}


/**
  * @brief  This function handles GPIO Handler.
  * @param  None
  * @retval None
  */
void GPIO_IRQHandler(void)
{
    LPC_GPIOINT_PORT_T GpioIntPortGroup[LPC_GPIOINT_PORT_COUNT] = {GPIOINT_PORT0, GPIOINT_PORT2};
    
    for (int iPortIndex = 0; iPortIndex < LPC_GPIOINT_PORT_COUNT; iPortIndex++)
    {
        if (Chip_GPIOINT_IsIntPending(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]))
        {
            //获取中断置位状态
            uint32_t IntStatus = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]) | 
                                 Chip_GPIOINT_GetStatusRising (LPC_GPIOINT, GpioIntPortGroup[iPortIndex]);
            
            //确认中断入口
            for (int iPinIndex = 0; iPinIndex < LPC_GPIOINT_PIN_COUNT; iPinIndex++)
            {
                if (IntStatus & (0x1<<iPinIndex))
                {
                    IntStatus &= ~(0x1<<iPinIndex);
                    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GpioIntPortGroup[iPortIndex], (0x1<<iPinIndex));    //清中断
                    
                    //执行中断回调
                    g_IRQInterface.pf_GPIO_IRQHandler[iPortIndex][iPinIndex]();
                    
                    if (IntStatus == 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    
}


/**
  * @brief  This function handles ADC Handler.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
    g_IRQInterface.pf_ADC_IRQHandler();

}


/**
  * @brief  This function handles BOD Handler.
  * @param  None
  * @retval None
  */
void BOD_IRQHandler(void)
{
    g_IRQInterface.pf_BOD_IRQHandler();

}


/**
  * @brief  This function handles USB Handler.
  * @param  None
  * @retval None
  */
void USB_IRQHandler(void)
{
    g_IRQInterface.pf_USB_IRQHandler();

}


/**
  * @brief  This function handles CAN Handler.
  * @param  None
  * @retval None
  */
void CAN_IRQHandler(void)
{
    g_IRQInterface.pf_CAN_IRQHandler();

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


/**
  * @brief  This function handles I2S Handler.
  * @param  None
  * @retval None
  */
void I2S_IRQHandler(void)
{
    g_IRQInterface.pf_I2S_IRQHandler();

}


/**
  * @brief  This function handles ENET Handler.
  * @param  None
  * @retval None
  */
void ENET_IRQHandler_back(void)
{
    g_IRQInterface.pf_ENET_IRQHandler();

}


/**
  * @brief  This function handles RIT Handler.
  * @param  None
  * @retval None
  */
void RIT_IRQHandler(void)
{
    g_IRQInterface.pf_RIT_IRQHandler();

}


/**
  * @brief  This function handles MCPWM Handler.
  * @param  None
  * @retval None
  */
void MCPWM_IRQHandler(void)
{
    g_IRQInterface.pf_MCPWM_IRQHandler();

}


/**
  * @brief  This function handles QEI Handler.
  * @param  None
  * @retval None
  */
void QEI_IRQHandler(void)
{
    g_IRQInterface.pf_QEI_IRQHandler();

}


/**
  * @brief  This function handles PLL1 Handler.
  * @param  None
  * @retval None
  */
void PLL1_IRQHandler(void)
{
    g_IRQInterface.pf_PLL1_IRQHandler();

}


/**
  * @brief  This function handles USBActivity Handler.
  * @param  None
  * @retval None
  */
void USBActivity_IRQHandler(void)
{
    g_IRQInterface.pf_USBActivity_IRQHandler();

}


/**
  * @brief  This function handles CANActivity Handler.
  * @param  None
  * @retval None
  */
void CANActivity_IRQHandler(void)
{
    g_IRQInterface.pf_CANActivity_IRQHandler();

}

#endif


















