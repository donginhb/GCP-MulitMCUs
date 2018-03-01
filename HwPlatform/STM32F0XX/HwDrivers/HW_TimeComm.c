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
  * TIM1:       CH1 --- PA8 [AF2] 
  *             CH2 --- PA9 [AF2] 
  *             CH3 --- PA10[AF2]
  *             CH4 --- PA11[AF2]
  * 
  * TIM2:       CH1 --- PA0
  *             CH2 --- PA1
  *             CH3 --- PA2
  *             CH4 --- PA3
  * 
  * TIM3:       CH1 --- PA6[AF1]     PB4[AF1] 
  *             CH2 --- PA7[AF1]     PB5[AF1]
  *             CH3 --- PB0[AF1]
  *             CH4 --- PB1[AF1]
  *                 
  * TIM14:      CH1 --- PA4[AF4]
  *             CH2 --- PA7[AF4]
  * 
  * TIM15[1]:   CH1 --- PA2[AF0]
  *             CH2 --- PA3[AF0]
  *             
  * TIM16:      CH1 --- PA6[AF5]
  * 
  * TIM17:      CH1 --- PA7[AF5]
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_TimeComm.h"
#include "HW_Gpio.h"
#include "chip.h"
      

/*****************************************************************************
 * 定时器通用控制接口
 ****************************************************************************/

//定时器节点定义
TIM_TypeDef * const TIM[HW_TIM_NODE_NUM] = {TIM1, TIM2, TIM3, TIM6, TIM7, TIM14, TIM15, TIM16, TIM17};


/**
  * @brief  定时器时钟开启
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HW_TIM_ClockEnable(uint8_t uTimeNode)
{
    uint32_t ulTimeBase = (uint32_t)TIM[uTimeNode];
    
    switch (ulTimeBase)
    {
    case TIM1_BASE:  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  break;
    case TIM2_BASE:  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  break;
    case TIM3_BASE:  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  break;
    case TIM6_BASE:  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  break;
    case TIM7_BASE:  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;  break;
    case TIM14_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; break;
    case TIM15_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; break;
    case TIM16_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; break;
    case TIM17_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM17EN; break;
    default: break;
    
    }
    
}


/**
  * @brief  定时器端口配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @param  uPortDir 端口方向(0-输出,1-输入)
  * @retval None
  */
void HW_TIM_PortConfig(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uPortDir)
{
    const uint32_t ulTimeBase = (uint32_t)TIM[uTimeNode];
    
    switch (ulTimeBase)
    {
    case TIM1_BASE:
        {
            /* 根据状态字配置引脚 */
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(0, 8, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 8, 2);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(0, 9, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 9, 2);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(0, 10, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 10, 2);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(0, 11, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 11, 2);
            }
            
            break;
        }
    case TIM2_BASE:
        {
            
            break;
        }
    case TIM3_BASE:
        {
            /* 根据状态字配置引脚 */
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(0, 6, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 6, 1);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(0, 7, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(0, 7, 1);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(1, 0, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(1, 0, 1);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(1, 1, HW_GPIO_AF_PP_H);
                HW_GPIO_SetAFMode(1, 1, 1);
            }
            
            break;
        }
        
    default: break;
    }
    
}


