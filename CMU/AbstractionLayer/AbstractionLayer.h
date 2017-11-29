#ifndef _ABSTRACTION_LAYER_H_
#define _ABSTRACTION_LAYER_H_

#include "../UIN_GlobalDataDef.h"
#include "../../DataType/DataType.h"
#include "../ProtocolLayer/ComDataStructDef.h"

typedef struct _COM_DATA_PACK
{
    COM_DATA_ID ulID;
    uBit32 ulDataLen;
    uBit8 *pDataBuf;
}COM_DATA_PACK;


/*****************************************************************************
 * CMU抽象层相关控制接口
 ****************************************************************************/

/**
  * @brief  抽象数据初始化
  * @param  None
  * @retval None
  */
void COM_AL_Init(void);


/**
  * @brief  通信端口打开
  * @param  ulComType 通信类型
  *   @arg COM_TYPE_NULL 无通信接口
  *   @arg COM_TYPE_CAN  CAN通信
  *   @arg COM_TYPE_ENET ENET通信
  *   @arg COM_TYPE_UART UART通信模式
  * @retval 当前通信类型
  */
uBit32 COM_AL_Open(uBit32 ulComType);


/**
  * @brief  当前通信类型获取
  * @param  None
  * @retval 当前通信类型
  */
uBit32 COM_AL_GetComType(void);


/**
  * @brief  通信接口挂接检测
  * @param  ulComType 接口
  * @retval true-已挂接 false-未挂接
  */
BooLean COM_AL_IsComTypeValid(uBit32 ulComType);


/**
  * @brief  通信关闭
  * @param  None
  * @retval None
  */
void COM_AL_Close(void);


/**
  * @brief  数据包获取
  * @param  pDataPack 数据包地址
  * @retval 0成功 非0-失败
  */
uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack);


/**
  * @brief  数据包发送
  * @param  ulID 数据包ID
  * @param  pDataBuf 数据缓冲区
  * @param  ulDataBufLen 数据区长度
  * @retval 0成功 非0-失败
  */
uBit32 COM_AL_SendPack(uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen);


/**
  * @brief  单次最大能发送的数据长度获取
  * @param  None
  * @retval 单包最大允许发送长度
  */
uBit32 COM_AL_GetMaxSendLen(void);


/**
  * @brief  抽象层可连续发送的数据包数量获取
  * @param  None
  * @retval 连续可发送的数据包个数
  * @note   最大连续可发送的数据包个数,对于CAN来说可以是多包,但是考虑到效率,必须指定合适值,对于以太网,必须每次
  *         只能发一包,否则在上传数据时,会存在丢包的风险;
  */
uBit32 COM_AL_GetMaxSendPacks(void);


/**
  * @brief  底层接收数据(在循环中调用)
  * @param  None
  * @retval None
  */
void COM_AL_RcvHandle(void);


//extern void* CMU_Malloc(uBit32 ulSize);



#endif