/**
  ******************************************************************************
  * @file    Demo.c
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
#define GET_BOOL(x)                 ((x) ? 1 : 0)
static const IRQn_Type EXTIx_IRQn[3] = { EXTI0_1_IRQn, EXTI2_3_IRQn, EXTI4_15_IRQn };

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
    //开SYSCFG时钟
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    
    //配置外部中断源
    SYSCFG->EXTICR[nPort/4] &= ~(0xF<<((nPort%4)*4));
    SYSCFG->EXTICR[nPort/4] |=  (nPort<<((nPort%4)*4));
    
    //配置边沿检测
    EXTI->RTSR &= ~(0X1<<nPin);
    EXTI->RTSR |=  (GET_BOOL(uTrgSource & HW_EXTI_TRG_RISING)<<nPin);  //配置线x的上升沿触发
    
    EXTI->FTSR &= ~(0X1<<nPin);
    EXTI->FTSR |=  (GET_BOOL(uTrgSource & HW_EXTI_TRG_FALLING)<<nPin);  //配置线x的下降沿触发
    
    //配置事件/中断
    EXTI->IMR |=  (0X1<<nPin);    //开放线x上的中断请求
    EXTI->EMR &= ~(0X1<<nPin);    //屏蔽线x上的事件请求
    
    //配置核级中断
    switch (nPin)
    {
    case 0:
    case 1:
        NVIC_SetPriority(EXTIx_IRQn[0], 2);
        NVIC_EnableIRQ(EXTIx_IRQn[0]);
        break;
        
    case 2:
    case 3:
        NVIC_SetPriority(EXTIx_IRQn[1], 2);
        NVIC_EnableIRQ(EXTIx_IRQn[1]);
        break;
        
    default: 
        NVIC_SetPriority(EXTIx_IRQn[2], 2);
        NVIC_EnableIRQ(EXTIx_IRQn[2]);
        break;
    }
    
    //开外部中断
    EXTI->PR  |=  (0X1<<nPin);    //清标志位
    EXTI->IMR |=  (0X1<<nPin);    //使能中断
    
    return 0;
}



