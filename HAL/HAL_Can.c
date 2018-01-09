/**
  ******************************************************************************
  * @file    HAL_Can.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.06
  * @brief   Hal can driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Can.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_Can.h"   
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/*****************************************************************************
 * CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN总线初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval None
  */
void HAL_CAN_Init(uBit8 uCanNode, uBit32 ulBitRate)
{
    HW_CAN_Init(uCanNode, ulBitRate);
      
}


/**
  * @brief  CAN打开
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 HAL_CAN_Open(uBit8 uCanNode)
{
    HW_CAN_Enable(uCanNode, true);
    
    return 0;
}


/**
  * @brief  CAN关闭
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 HAL_CAN_Close(uBit8 uCanNode)
{
    HW_CAN_Enable(uCanNode, false);
    
    return 0;
}


/**
  * @brief  CAN数据发送
  * @param  uCanNode CAN节点号
  * @param  pSendFrame 发送帧数据
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_CAN_Send(uBit8 uCanNode, HAL_CAN_FRAME_T *pSendFrame)
{
    uBit32 ulRet = 0;
    CAN_MSG_T TempMsg = {0};
    
    //匹配数据
    TempMsg.DLC = pSendFrame->uDLC;
    TempMsg.ID = pSendFrame->ulID;
    memcpy(TempMsg.Data, pSendFrame->uData, TempMsg.DLC);
    
    if (pSendFrame->uType & HAL_CAN_TYPE_REMOTE)    //数据帧/遥控帧解析
    {
        TempMsg.Type = 1;
    }
    
    if (pSendFrame->uType & HAL_CAN_TYPE_EXT_FRAME) //标准帧/扩展帧解析
    {
        TempMsg.ID |= CAN_EXTEND_ID_USAGE;  //扩展帧标志
    }
    
    //发送数据
    ulRet = HW_CAN_SendMsg(uCanNode, &TempMsg);
    
    return ulRet;
}


/**
  * @brief  CAN数据接收
  * @param  uCanNode CAN节点号
  * @param  pRecvFrame 接收帧数据
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_CAN_Recv(uBit8 uCanNode, HAL_CAN_FRAME_T *pRecvFrame)
{
    uBit32 ulRet = 0;
    CAN_MSG_T TempMsg = {0};
    
    //接收数据
    ulRet = HW_CAN_RecvMsg(uCanNode, &TempMsg);
    
    //匹配数据
    if (ulRet == 0)
    {
        pRecvFrame->uDLC = TempMsg.DLC;
        
        memcpy(pRecvFrame->uData, TempMsg.Data, pRecvFrame->uDLC);
        
        if (TempMsg.Type == CAN_REMOTE_MSG)     //数据帧/遥控帧解析
        {
            pRecvFrame->uType = HAL_CAN_TYPE_REMOTE;
        }
        
        if (TempMsg.ID & CAN_EXTEND_ID_USAGE)   //标准帧/扩展帧解析
        {
            pRecvFrame->ulID = TempMsg.ID & CAN_EXT_ENTRY_ID_MASK;
            pRecvFrame->uType |= HAL_CAN_TYPE_EXT_FRAME;
        }
    }
    
    return ulRet;
}
