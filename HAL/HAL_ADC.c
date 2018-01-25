/**
  ******************************************************************************
  * @file    HAL_ADC.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.20
  * @brief   Hal adc driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_ADC.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_Adc.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * ADC相关控制接口
 ****************************************************************************/

/**
  * @brief  ADC模式使能
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @param  bCtnConvertFlag 连续转换标志
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_ADC_Init(uBit8 uAdcNode, uBit32 ulChMask, bool bCtnConvertFlag)
{
    
    return HW_ADC_Init(uAdcNode, ulChMask, bCtnConvertFlag);
}


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_ADC_RecvValueNoBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue)
{
    uBit32 ulRet = 0;
    
#if defined(LPC17XX)
    ulRet = HW_ADC_RecvValueNoBlock(uAdcNode, ulChannel, pAdcValue);

#elif defined(LPC43XX)
    

#elif defined(STM32F10X)
    ulRet = HW_ADC_RecvValueNoBlock(uAdcNode, pAdcValue);
    
#elif defined(STM32F0XX)
    ulRet = HW_ADC_RecvValueNoBlock(uAdcNode, ulChannel, pAdcValue);
    
#endif

    return ulRet;
}


/**
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_ADC_RecvValueBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue)
{
    uBit32 ulRet = 0;
    
#if defined(LPC17XX)
    ulRet = HW_ADC_RecvValueBlock(uAdcNode, ulChannel, pAdcValue);

#elif defined(LPC43XX)
    

#elif defined(STM32F10X)
    ulRet = HW_ADC_RecvValueBlock(uAdcNode, pAdcValue);
    
#elif defined(STM32F0XX)
    ulRet = HW_ADC_RecvValueBlock(uAdcNode, ulChannel, pAdcValue);
    
#endif

    return ulRet;
}


/**
  * @brief  ADC采集开始
  * @param  uAdcNode ADC节点
  * @retval None
  */
void HAL_ADC_Start(uBit8 uAdcNode)
{
    HW_ADC_Start(uAdcNode);
    
}

