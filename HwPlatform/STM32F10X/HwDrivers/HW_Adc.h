#ifndef  __HW_ADC_H
#define  __HW_ADC_H

#include "chip.h"

//ADC节点定义
#define HW_ADC_NODE_0      (0)
#define HW_ADC_NODE_1      (1)
#define HW_ADC_NODE_2      (2)

#ifdef __cplusplus
extern "C"
{
#endif
    
/*****************************************************************************
 * ADC相关控制接口
 ****************************************************************************/

/**
  * @brief  ADC模式初始化
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @param  bScanModeFlag 扫描模式
  * @retval 0-成功 非0-失败
  * @note   非扫描模式下,只能使能一个通道,若入参多个通道,则默认只能最小的通道
  */
uint32_t HW_ADC_Init(uint8_t uAdcNode, uint32_t ulChMask, bool bScanModeFlag);


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueNoBlock(uint8_t uAdcNode, uint16_t *pAdcValue);


/** 
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueBlock(uint8_t uAdcNode, uint16_t *pAdcValue);


/**
  * @brief  ADC采集开始
  * @param  uAdcNode ADC节点
  * @retval None
  */
void HW_ADC_Start(uint8_t uAdcNode);


#ifdef __cplusplus
}
#endif

#endif /* __HW_ADC_H */
