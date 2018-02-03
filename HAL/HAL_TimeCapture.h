#ifndef  __HAL_TIME_CAPTURE_H
#define  __HAL_TIME_CAPTURE_H

#include "DataType/DataType.h"

//定时器捕获边沿定义
#define HAL_TIM_CAP_EDGE_RISING      (0x01)  //上升沿捕获
#define HAL_TIM_CAP_EDGE_FALLING     (0x02)  //下降沿捕获
#define HAL_TIM_CAP_EDGE_BOTH        (0x03)  //双边沿捕获


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 定时器捕获模式相关控制接口
 ****************************************************************************/

/**
  * @brief  捕获模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_TIME_InitCapture(uBit8 uTimeNode, uBit8 uChannelMask, uBit8 uTrgSource);


/**
  * @brief  捕获模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval None
  */
void HAL_TIME_SetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum, uBit8 uTrgSource);


/**
  * @brief  定时器输入使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HAL_TIME_EnableCapture(uBit8 uTimeNode, bool bIsEnable);


/**
  * @brief  定时器输入捕获值复位
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIME_ResetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uBit32 HAL_TIME_GetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum);


/*****************************************************************************
 * 定时器捕获模式中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIME_EnableCaptureIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable);


/**
  * @brief  定时器捕获中断标志获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起标志
  */
bool HAL_TIME_GetCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器捕获中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIME_ClearCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_TIME_CAPTURE_H */
