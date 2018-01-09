/**
  ******************************************************************************
  * @file    Can.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.06
  * @brief   Can Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Can.h"
#include "CanBuff.h"
#include "CanDataStructDef.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Can.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/*****************************************************************************
 * CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_Init(uBit8 uCanNode, uBit32 ulBitRate)
{
    HAL_CAN_Init(uCanNode, ulBitRate);
    CAN_InitBuff(uCanNode);
    
    return 0;
}


/**
  * @brief  CAN打开
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 CAN_Open(uBit8 uCanNode)
{
    
    return HAL_CAN_Open(uCanNode);
}


/**
  * @brief  CAN关闭
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 CAN_Close(uBit8 uCanNode)
{
    
    return HAL_CAN_Close(uCanNode);
}


/**
  * @brief  CAN发送
  * @param  uCanNode CAN节点
  * @param  pCanFrame CAN数据帧
  * @param  ulFrameLenght 数据帧数量
  * @param  bIsForce 是否强制立即发送,若否,则存入发送缓存区后再发送
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_Write(uBit8 uCanNode, CAN_FRAME_T *pCanFrame, uBit32 ulFrameCount, bool bIsForce)
{
    uBit32 ulRet = 0;
    
    if (bIsForce)
    {
        for (int i = 0; i < ulFrameCount; i++)
        {
            HAL_CAN_Send(uCanNode, &pCanFrame[i]);
        }
        
    }
    else 
    {
        CAN_WriteForBuff(uCanNode, pCanFrame, ulFrameCount);
    }
    
    return ulRet;
}


/**
  * @brief  CAN接收
  * @param  uCanNode CAN节点
  * @param  pCanFrame CAN数据帧缓冲区(出参)
  * @param  ulFrameLenght 要接收的数量
  * @retval 实际接收到的数据帧数量
  */
uBit32 CAN_Recv(uBit8 uCanNode, CAN_FRAME_T *pCanFrame, uBit32 ulFrameCount)
{
    uBit32 ulRecvCount = 0;
    
    ulRecvCount = CAN_RecvForBuff(uCanNode, pCanFrame, ulFrameCount);
    
    return ulRecvCount;
}


/**
  * @brief  CAN主任务处理
  * @param  uCanNode CAN节点
  * @retval None
  */
void CAN_MainHandler(uBit8 uCanNode)
{
    CAN_BuffHandler(uCanNode);
    
}

