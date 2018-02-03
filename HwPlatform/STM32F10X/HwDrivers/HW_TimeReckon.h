#ifndef  __HW_TIME_RECKON_H
#define  __HW_TIME_RECKON_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * 定时器计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  None
  * @retval None
  */
void HW_TIME_InitReckonMode(uint8_t uTimeNode);


/**
  * @brief  定时器最小计数单位设置(默认1US)
  * @param  uTimeNode 定时器节点
  * @param  uTimeNode 最小计时单位(us)
  * @retval None
  * @note   计时单位越小,计时的精度越高,同时计时的范围越小,受限于定时器的位数.
  *         系统初始化时默认1US计时一次,若要获取更广的范围或者更高的精度,则可
  *         以通过这个接口来配置,以适应不同的应用场合;
  */
void HW_TIME_SetMinReckonUnit(uint8_t uTimeNode, uint32_t ulMinReckonUnit);


/**
  * @brief  计时开始
  * @param  None
  * @retval None
  */
void HW_TIME_StartReckon(uint8_t uTimeNode);


/**
  * @brief  计时停止
  * @param  None
  * @retval None
  */
void HW_TIME_StopReckon(uint8_t uTimeNode);


/**
  * @brief  计时值获取
  * @param  None
  * @retval 计时值(单位: us)
  */
uint16_t HW_TIME_GetReckonValue(uint8_t uTimeNode);


/**
  * @brief  计时值清零
  * @param  None
  * @retval None
  */
void HW_TIME_ClearRecvkonValue(uint8_t uTimeNode);

  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIME_RECKON_H */
