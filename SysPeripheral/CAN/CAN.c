/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CAN.h"
#include "DataType/DataType.h"
#include "../HAL/HAL_Can.h"
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN初始化
  * @param  uCanNode   CAN节点号
  * @param  ulBaudRate 波特率
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_Init(uBit8 uCanNode, uBit32 ulBaudRate)
{
    HAL_CAN_Init(uCanNode, ulBaudRate);
      
    return 0;
}

/**
  * @brief  CAN 数据包发送
  * @param  uCanNode CAN节点号
  * @param  ulID   目标ID号
  * @param  pBuff  数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_SendPack(uBit8 uCanNode, uBit32 ulID, uBit8 *pBuff, uBit32 ulSize)
{
    CAN_MSG_T MsgObj = {0};
    
    MsgObj.ID = ulID;
    MsgObj.Type = 0;
    MsgObj.DLC = ulSize;
    memcpy(MsgObj.Data, pBuff, ulSize);
    
    uBit32 ulRet = HAL_CAN_SendMsg(uCanNode, &MsgObj);
    
    return ulRet;
}


/**
  * @brief  CAN 数据包接收
  * @param  uCanNode CAN节点号
  * @param  ulID   目标ID号
  * @param  pBuff  数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_RecvPack(uBit8 uCanNode, uBit32 *pID, uBit8 *pBuff, uBit32 *pSize)
{
    uBit32 ulRet = 0;
    CAN_MSG_T MsgObj = {0};
    
    ulRet = HAL_CAN_RecvMsg(uCanNode, &MsgObj);
    
    if (ulRet == 0)
    {
        *pID = MsgObj.ID;
        *pSize = MsgObj.DLC;
        memcpy(pBuff, MsgObj.Data, MsgObj.DLC);
    }
    
    return ulRet;
}


/**
  * @brief  CAN 数据包接收
  * @param  uCanNode CAN节点号
  * @retval None
  */
void CAN_RecvHandler(uBit8 uCanNode)
{
    
    
}














