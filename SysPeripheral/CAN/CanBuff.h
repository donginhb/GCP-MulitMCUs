#ifndef  __CAN_BUF_H
#define  __CAN_BUF_H

#include "DataType/DataType.h"
#include "CanDataStructDef.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * CAN环形缓冲区相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN缓冲区初始化
  * @param  uCanNode CAN节点
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_InitBuff(uBit8 uCanNode);


/**
  * @brief  CAN缓冲发送
  * @param  uCanNode CAN节点
  * @param  CanFrame CAN数据帧
  * @param  ulFrameLenght 数据帧数量
  * @retval 0-成功 非0-失败
  */
uBit32 CAN_WriteForBuff(uBit8 uCanNode, CAN_FRAME_T *CanFrame, uBit32 ulFrameCount);


/**
  * @brief  CAN缓冲接收
  * @param  uCanNode CAN节点
  * @param  CanFrame CAN数据帧缓冲区(出参)
  * @param  ulFrameLenght 要接收的数量
  * @retval 实际接收到的数据帧数量
  */
uBit32 CAN_RecvForBuff(uBit8 uCanNode, CAN_FRAME_T *CanFrame, uBit32 ulFrameCount);



/**
  * @brief  CAN缓冲区发送接收处理
  * @param  uCanNode CAN节点
  * @retval None
  */
void CAN_BuffHandler(uBit8 uCanNode);



#ifdef __cplusplus
}
#endif

#endif /* __CAN_BUF_H */
