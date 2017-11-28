/**
  ******************************************************************************
  * @file    HAL_IRQHandler.c
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
#include "HAL_IRQHandler.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_IRQHandler.h"

/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HAL_IRQ_Init(void)
{
    HW_IRQ_Init();
    
}


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_SetTrgCallback(void (*ptr)(void), uBit32 ulTrgSource)
{
    HW_IRQ_SetTrgCallback(ptr, ulTrgSource);
    
}


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_ReleaseTrgCallback(uBit32 ulTrgSource)
{
    HW_IRQ_ReleaseTrgCallback(ulTrgSource);
    
}

