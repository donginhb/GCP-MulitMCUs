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
#include "HW_TimeComm.h"
#include "HW_Gpio.h"
#include "chip.h"
      

/*****************************************************************************
 * 定时器通用控制接口
 ****************************************************************************/

//定时器节点定义
TIM_TypeDef * const TIM[HW_TIM_NODE_NUM] = {   TIM1,TIM8, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, \
                                                TIM9, TIM10, TIM11, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17};


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
    case TIM1_BASE:  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE); break;
    case TIM2_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE); break;
    case TIM3_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE); break;
    case TIM4_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,  ENABLE); break;
    case TIM5_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,  ENABLE); break;
    case TIM6_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,  ENABLE); break;
    case TIM7_BASE:  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,  ENABLE); break;
    case TIM8_BASE:  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE); break;
    case TIM9_BASE:  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,  ENABLE); break;
    case TIM10_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); break;
    case TIM11_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); break;
    case TIM12_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); break;
    case TIM13_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); break;
    case TIM14_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); break;
    case TIM15_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE); break;
    case TIM16_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE); break;
    case TIM17_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE); break;
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
                HW_GPIO_ModeConfig(0, 8, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(0, 9, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(0, 10, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(0, 11, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            
            break;
        }
    case TIM2_BASE:
    case TIM5_BASE:
        {
            /* 根据状态字配置引脚 */
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(0, 0, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(0, 1, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(0, 2, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(0, 3, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            
            break;
        }
    case TIM3_BASE:
        {
            /* 根据状态字配置引脚 */
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(0, 6, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(0, 7, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(1, 0, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(1, 1, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            
            break;
        }
    case TIM4_BASE:
        {
            /* 根据状态字配置引脚 */
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(1, 6, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(1, 7, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }  
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(1, 8, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(1, 9, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            
            break;
        }
    case TIM8_BASE:
        {
            if (uChannelMask & (0X1<<0))
            {
                HW_GPIO_ModeConfig(2, 6, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<1))
            {
                HW_GPIO_ModeConfig(2, 7, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<2))
            {
                HW_GPIO_ModeConfig(2, 8, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }
            if (uChannelMask & (0X1<<3))
            {
                HW_GPIO_ModeConfig(2, 9, (uPortDir == HW_TIME_PORT_OUTPUT) ? HW_GPIO_AF_PP_H : HW_GPIO_FLOAT);
            }  
            break;
        }
        
    default: break;
    }
    
}


