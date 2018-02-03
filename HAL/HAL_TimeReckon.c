/**
  ******************************************************************************
  * @file    HAL_TimeReckon.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.30
  * @brief   Hal time drivers
  ******************************************************************************
  * @attention
  * 
  * SysTimer实现的软件定时器可以实现MS级的计时功能,但是无法实现US级精度.
  * 本驱动应用于需要达到US级计时精度的场合
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_TimeReckon.h"
#include "DataType/DataType.h"
      
#if (defined(LPC17XX)||defined(LPC43XX))
#include "HwDrivers/HW_Rit.h"
#elif (defined(STM32F10X)||defined(STM32F0XX))
#include "HwDrivers/HW_TimeReckon.h"
#endif


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * 定时器计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  uTimeNode 定时器节点
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_TIME_InitReckonMode(uBit8 uTimeNode)
{
    
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_InitReckonMode();
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_InitReckonMode(uTimeNode);
#endif
    
    return 0;
}


/**
  * @brief  定时器最小计数单位设置(默认1US)
  * @param  uTimeNode 定时器节点
  * @param  uTimeNode 最小计时单位(us)
  * @retval None
  * @note   计时单位越小,计时的精度越高,同时计时的范围越小,受限于定时器的位数.
  *         系统初始化时默认1US计时一次,若要获取更广的范围或者更高的精度,则可
  *         以通过这个接口来配置,以适应不同的应用场合;
  */
void HAL_TIME_SetMinReckonUnit(uBit8 uTimeNode, uBit32 ulMinReckonUnit)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_SetMinReckonUnit(uTimeNode, ulMinReckonUnit);
#endif
    
}


/**
  * @brief  计时开始
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_StartReckon(uBit8 uTimeNode)
{
    
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_StartReckon();
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_StartReckon(uTimeNode);
#endif
    
}


/**
  * @brief  计时停止
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_StopReckon(uBit8 uTimeNode)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_StopReckon();
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_StopReckon(uTimeNode);
#endif
    
}


/**
  * @brief  计时值获取
  * @param  uTimeNode 定时器节点
  * @retval 计时值(单位: us)
  */
uBit32 HAL_TIME_GetReckonValue(uBit8 uTimeNode)
{
    uBit32 ulReckonValue = 0;
    
#if (defined(LPC17XX)||defined(LPC43XX))
    ulReckonValue = HW_RIT_GetReckonValue();
#elif (defined(STM32F10X)||defined(STM32F0XX))
    ulReckonValue = HW_TIME_GetReckonValue(uTimeNode);
#endif
    
    return ulReckonValue;
}


/**
  * @brief  计时值清零
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_ClearRecvkonValue(uBit8 uTimeNode)
{
    
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_RIT_ClearRecvkonValue();
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_ClearRecvkonValue(uTimeNode);
#endif
    
}

