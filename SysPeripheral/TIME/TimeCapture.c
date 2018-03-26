/**
  ******************************************************************************
  * @file    TimeCapture.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.02.02
  * @brief   time capture application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "TimeCapture.h"
#include "DataType/DataType.h"
#include "HAL/HAL_TimeCapture.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


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
uBit32 TIME_CAP_InitCapture(uBit8 uTimeNode, uBit8 uChannelMask, uBit8 uTrgSource)
{
    
    return HAL_TIME_InitCapture(uTimeNode, uChannelMask, uTrgSource);
}


/**
  * @brief  捕获源设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval None
  */
void TIME_CAP_SetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum, uBit8 uTrgSource)
{
    HAL_TIME_SetCaptureEdge(uTimeNode, uChannelNum, uTrgSource);
    
}


/**
  * @brief  捕获源获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起) 
  * @retval 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  */
uBit8 TIME_CAP_GetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum)
{
    
    return HAL_TIME_GetCaptureEdge(uTimeNode, uChannelNum);
}


/**
  * @brief  定时器输入使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void TIME_CAP_EnableCapture(uBit8 uTimeNode, bool bIsEnable)
{
    HAL_TIME_EnableCapture(uTimeNode, bIsEnable);
    
}


/**
  * @brief  定时器输入捕获值复位
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_ResetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum)
{
    HAL_TIME_ResetCaptureValue(uTimeNode, uChannelNum);
    
}


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uBit32 TIME_CAP_GetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum)
{
    
    return HAL_TIME_GetCaptureValue(uTimeNode, uChannelNum);
}


/*****************************************************************************
 * 定时器捕获模式中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_EnableCaptureIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable)
{
    HAL_TIME_EnableCaptureIRQ(uTimeNode, uChannelNum, bIsEnable);
    
}


/**
  * @brief  定时器捕获中断标志获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起标志
  */
bool TIME_CAP_GetCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum)
{
    
    return HAL_TIME_GetCaptureIRQFlag(uTimeNode, uChannelNum);
}


/**
  * @brief  定时器捕获中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void TIME_CAP_ClearCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum)
{
    HAL_TIME_ClearCaptureIRQFlag(uTimeNode, uChannelNum);
    
}

