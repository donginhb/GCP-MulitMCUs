/**
  ******************************************************************************
  * @file    ADC.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.20
  * @brief   ADC Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "ADC.h"
#include "DataType/DataType.h"
#include "HAL/HAL_ADC.h"

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
uBit32 ADC_Init(uBit8 uAdcNode, uBit32 ulChMask, bool bCtnConvertFlag)
{
    
    return HAL_ADC_Init(uAdcNode, ulChMask, bCtnConvertFlag);
}


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 ADC_RecvValueNoBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue)
{
    
    return HAL_ADC_RecvValueNoBlock(uAdcNode, ulChannel, pAdcValue);
}


/**
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 ADC_RecvValueBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue)
{
    
    return HAL_ADC_RecvValueBlock(uAdcNode, ulChannel, pAdcValue);
}


/**
  * @brief  ADC采集开始
  * @param  uAdcNode ADC节点
  * @retval None
  */
void ADC_Start(uBit8 uAdcNode)
{
    HAL_ADC_Start(uAdcNode);
    
}

