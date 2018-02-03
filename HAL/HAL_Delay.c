/**
  ******************************************************************************
  * @file    HAL_Delay.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   Hal delay drivers
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Delay.h"
#include "DataType/DataType.h"


#if (defined(LPC17XX)||defined(LPC43XX))
#include "HwDrivers/HW_Rit.h"
#elif defined(STM32F10X)
#include "HwDrivers/HW_TimeDelay.h"
#endif

      
/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#if defined(STM32F10X)
static uBit8 m_DelayTimeNode = 0;
#endif

/*****************************************************************************
 * 定时器延时相关控制接口
 ****************************************************************************/

/**
  * @brief  延时定时器初始化
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_DelayInit(uBit8 uTimeNode)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_InitDelayMode();
#elif defined(STM32F10X)
    m_DelayTimeNode = uTimeNode;
    HW_TIM_DelayInit(uTimeNode);
#endif
}


/**
  * @brief  us级延时
  * @param  要延时的us数
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HAL_DelayUs(uBit32 ulUs)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_DelayUs(ulUs);   //取值范围为: 0xFFFF FFFF/0x18 ==> 约178S
#elif defined(STM32F10X)
    HW_TIM_DelayUs(m_DelayTimeNode, ulUs);
#endif
    
}


/**
  * @brief  Ms级延时
  * @param  要延时的Ms数
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HAL_DelayMs(uBit32 ulMs)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_DelayMs(ulMs);   //取值范围为: 0xFFFF FFFF/24000
#elif defined(STM32F10X)
    HW_TIM_DelayMs(m_DelayTimeNode, ulMs);
#endif
    
}

