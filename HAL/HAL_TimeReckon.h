#ifndef  __HAL_TIME_RECKON_H
#define  __HAL_TIME_RECKON_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * 定时器计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  uTimeNode 定时器节点
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_TIME_InitReckonMode(uBit8 uTimeNode);


/**
  * @brief  定时器最小计数单位设置(默认1US)
  * @param  uTimeNode 定时器节点
  * @param  uTimeNode 最小计时单位(us)
  * @retval None
  * @note   计时单位越小,计时的精度越高,同时计时的范围越小,受限于定时器的位数.
  *         系统初始化时默认1US计时一次,若要获取更广的范围或者更高的精度,则可
  *         以通过这个接口来配置,以适应不同的应用场合;
  */
void HAL_TIME_SetMinReckonUnit(uBit8 uTimeNode, uBit32 ulMinReckonUnit);


/**
  * @brief  计时开始
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_StartReckon(uBit8 uTimeNode);


/**
  * @brief  计时停止
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_StopReckon(uBit8 uTimeNode);


/**
  * @brief  计时值获取
  * @param  uTimeNode 定时器节点
  * @retval 计时值(单位: us)
  */
uBit32 HAL_TIME_GetReckonValue(uBit8 uTimeNode);


/**
  * @brief  计时值清零
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HAL_TIME_ClearRecvkonValue(uBit8 uTimeNode);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_TIME_RECKON_H */
