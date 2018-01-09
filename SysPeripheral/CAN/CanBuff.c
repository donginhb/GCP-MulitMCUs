/**
  ******************************************************************************
  * @file    CanBuf.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.06
  * @brief   Can RingBuff Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CanBuff.h"
#include "CanDataStructDef.h"
#include "DataType/DataType.h"
#include "Algorithm/RingBuff/RingBuff.h"
#include "HAL/HAL_Can.h"
      
      
/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
static RINGBUFF_T  m_CanTxRing[CAN_NODE_COUNT] = {0};   //发送缓存区参数
static CAN_FRAME_T m_CanTxFrame[CAN_NODE_COUNT][CAN_TX_RING_BUFF_LEN] = {0};    //发送缓冲区

static RINGBUFF_T  m_CanRxRing[CAN_NODE_COUNT] = {0};   //接收缓存区参数
static CAN_FRAME_T m_CanRxFrame[CAN_NODE_COUNT][CAN_RX_RING_BUFF_LEN] = {0};    //接收缓存区


/*****************************************************************************
 * CAN环形缓冲区相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN缓冲区初始化
  * @param  uCanNode CAN节点
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_InitBuff(uBit8 uCanNode)
{
    //初始化环形FIFO
    RingBuff_Init(&m_CanTxRing[uCanNode], m_CanTxFrame[uCanNode], sizeof(CAN_FRAME_T), CAN_TX_RING_BUFF_LEN);
    RingBuff_Init(&m_CanRxRing[uCanNode], m_CanRxFrame[uCanNode], sizeof(CAN_FRAME_T), CAN_RX_RING_BUFF_LEN);

    return 0;
}


/**
  * @brief  CAN缓冲发送
  * @param  uCanNode CAN节点
  * @param  CanFrame CAN数据帧
  * @param  ulFrameLenght 数据帧数量
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_WriteForBuff(uBit8 uCanNode, CAN_FRAME_T *CanFrame, uBit32 ulFrameCount)
{
    uBit32 ulRet = 0;
    
    if (ulFrameCount == 1)
    {
        ulRet = RingBuff_Insert(&m_CanTxRing[uCanNode], CanFrame);
    }
    else 
    {
        ulRet = RingBuff_InsertMult(&m_CanTxRing[uCanNode], CanFrame, ulFrameCount);
    }
    
    return !ulRet;
}


/**
  * @brief  CAN缓冲接收
  * @param  uCanNode CAN节点
  * @param  CanFrame CAN数据帧缓冲区(出参)
  * @param  ulFrameLenght 要接收的数量
  * @retval 实际接收到的数据帧数量
  */
uBit32 CAN_RecvForBuff(uBit8 uCanNode, CAN_FRAME_T *CanFrame, uBit32 ulFrameCount)
{
    
    return RingBuff_PopMult(&m_CanRxRing[uCanNode], CanFrame, ulFrameCount);
}


/**
  * @brief  CAN缓冲区发送接收处理
  * @param  uCanNode CAN节点
  * @retval None
  */
void CAN_BuffHandler(uBit8 uCanNode)
{
    
    //假如发送缓冲区非空,则发送数据
    if (RingBuff_IsEmpty(&m_CanTxRing[uCanNode]) != 1)
    {
        //提取数据
        CAN_FRAME_T SendFrame = {0};
        RingBuff_Pop(&m_CanTxRing[uCanNode], &SendFrame);
        
        //发送数据
        HAL_CAN_Send(uCanNode, &SendFrame);
    }
    
    //假如接收缓存区没满,则接收数据并存储入缓冲区之中
    if (RingBuff_IsFull(&m_CanRxRing[uCanNode]) != 1)
    {
        uBit32 ulRet = 0;
        
        //接收数据
        CAN_FRAME_T RecvFrame = {0};
        ulRet = HAL_CAN_Recv(uCanNode, &RecvFrame);
        
        //若成功接收到数据,则存入缓冲区
        if (ulRet == 0)
        {
            RingBuff_Insert(&m_CanRxRing[uCanNode], &RecvFrame);
        }
    }
    
}

