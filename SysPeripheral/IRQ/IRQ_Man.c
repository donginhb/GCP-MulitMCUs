/**
  ******************************************************************************
  * @file    IRQ_Man.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   Hardware IRQ manage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "IRQ_Man.h"
#include "HAL/HAL_IRQHandler.h"
#include <stdio.h>


/**
  * @brief  中断接口初始化
  * @param  None
  * @retval None
  * @note   本函数必须在所有硬件模块配置之前调用(重要)
  *         
  */
void IRQ_Init(void)
{
    HAL_IRQ_Init();
    
}


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void IRQ_SetTrgCallback(void (*ptr)(void), uBit32 ulTrgSource)
{
    HAL_IRQ_SetTrgCallback(ptr, ulTrgSource);
    
}


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void IRQ_ReleaseTrgCallback(uBit32 ulTrgSource)
{
    IRQ_ReleaseTrgCallback(ulTrgSource);
    
}
