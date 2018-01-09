#ifndef  __CAN_H
#define  __CAN_H

#include "CanDataStructDef.h"
#include "DataType/DataType.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_Init(uBit8 uCanNode, uBit32 ulBitRate);


/**
  * @brief  CAN打开
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 CAN_Open(uBit8 uCanNode);


/**
  * @brief  CAN关闭
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 CAN_Close(uBit8 uCanNode);


/**
  * @brief  CAN发送
  * @param  uCanNode CAN节点
  * @param  pCanFrame CAN数据帧
  * @param  ulFrameLenght 数据帧数量
  * @param  bIsForce 是否强制立即发送,若否,则存入发送缓存区后再发送
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_Write(uBit8 uCanNode, CAN_FRAME_T *pCanFrame, uBit32 ulFrameCount, bool bIsForce);


/**
  * @brief  CAN接收
  * @param  uCanNode CAN节点
  * @param  pCanFrame CAN数据帧缓冲区(出参)
  * @param  ulFrameLenght 要接收的数量
  * @retval 实际接收到的数据帧数量
  */
uBit32 CAN_Recv(uBit8 uCanNode, CAN_FRAME_T *pCanFrame, uBit32 ulFrameCount);


/**
  * @brief  CAN主任务处理
  * @param  uCanNode CAN节点
  * @retval None
  */
void CAN_MainHandler(uBit8 uCanNode);


#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */
