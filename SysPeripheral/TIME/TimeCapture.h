#ifndef  __TIME_CAPTURE_H
#define  __TIME_CAPTURE_H

#include "DataType/DataType.h"
#include "HAL/HAL_TimeCapture.h"


//定时器捕获边沿定义
#define TIM_CAP_EDGE_RISING      (HAL_TIM_CAP_EDGE_RISING )  //上升沿捕获
#define TIM_CAP_EDGE_FALLING     (HAL_TIM_CAP_EDGE_FALLING)  //下降沿捕获
#define TIM_CAP_EDGE_BOTH        (HAL_TIM_CAP_EDGE_BOTH   )  //双边沿捕获


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
uBit32 TIME_CAP_InitCapture(uBit8 uTimeNode, uBit8 uChannelMask, uBit8 uTrgSource);
      

/**
  * @brief  捕获源设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval None
  */
void TIME_CAP_SetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum, uBit8 uTrgSource);


/**
  * @brief  捕获源获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起) 
  * @retval 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  */
uBit8 TIME_CAP_GetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器输入使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void TIME_CAP_EnableCapture(uBit8 uTimeNode, bool bIsEnable);


/**
  * @brief  定时器输入捕获值复位
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_ResetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uBit32 TIME_CAP_GetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum);


/*****************************************************************************
 * 定时器捕获模式中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_EnableCaptureIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable);


/**
  * @brief  定时器捕获中断标志获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起标志
  */
bool TIME_CAP_GetCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器捕获中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_ClearCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum);



#ifdef __cplusplus
}
#endif

#endif /* __TIME_CAPTURE_H */
