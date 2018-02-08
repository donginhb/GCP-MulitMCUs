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
//串口数量定义
#define HW_EXTI_COUNT       (8)

static const IRQn_Type EXTI_IRQn[HW_EXTI_COUNT] = {PIN_INT0_IRQn, PIN_INT1_IRQn, PIN_INT2_IRQn, PIN_INT3_IRQn, 
                                                   PIN_INT4_IRQn, PIN_INT5_IRQn, PIN_INT6_IRQn, PIN_INT7_IRQn};   //中断向量

static uint8_t m_uCurIntPortSel = 0;

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
    //选择中断端口
    Chip_SCU_GPIOIntPinSel(m_uCurIntPortSel, nPort, nPin);
    
    //配置中断模式
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(m_uCurIntPortSel));   //设置为边沿触发模式
    
    //配置触发边沿
    if (uTrgSource & HW_EXTI_TRG_RISING)        //上升沿
    {
        Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(m_uCurIntPortSel));
    }
    else if (uTrgSource & HW_EXTI_TRG_FALLING)  //下降沿
    {
        Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(m_uCurIntPortSel));
    }
    
    //使能NVIC中断
    NVIC_ClearPendingIRQ(EXTI_IRQn[m_uCurIntPortSel]);
    NVIC_EnableIRQ(EXTI_IRQn[m_uCurIntPortSel]);
    
    //递增中断端口
    m_uCurIntPortSel++;
        
    return 0;
}

