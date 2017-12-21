#ifndef  __HAL_ADC_H
#define  __HAL_ADC_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


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
uBit32 HAL_ADC_Init(uBit8 uAdcNode, uBit32 ulChMask, bool bCtnConvertFlag);


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_ADC_RecvValueNoBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue);


/**
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_ADC_RecvValueBlock(uBit8 uAdcNode, uBit32 ulChannel, uBit16 *pAdcValue);


/**
  * @brief  ADC采集开始
  * @param  uAdcNode ADC节点
  * @retval None
  */
void HAL_ADC_Start(uBit8 uAdcNode);
    

#ifdef __cplusplus
}
#endif

#endif /* __HAL_ADC_H */
