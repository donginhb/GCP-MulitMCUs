#ifndef  __CMU_CAN_INTERFACE_H
#define  __CMU_CAN_INTERFACE_H

#include "DataType/DataType.h"

//CMU CAN 接口定义
typedef struct
{
    /**
      * @brief  CAN打开
      * @param  uCanNode CAN节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_CAN_Open)(uBit8 uCanNode);


    /**
      * @brief  CAN关闭
      * @param  uCanNode CAN节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_CAN_Close)(uBit8 uCanNode);
    
    
    /**
      * @brief  CAN数据包发送(非阻塞)
      * @param  uCanNode CAN节点号
      * @param  pSBuff 数据缓冲区地址
      * @param  nSize 发送的数量
      * @retval 0-成功 非0-失败
      */
    uBit32 (*pf_CAN_SendPack)(uBit8 uCanNode, uBit32 ulID, uBit8 *pSBuff, uBit32 ulSize);
    
    
    /**
      * @brief  CAN数据包(非阻塞)
      * @param  uCanNode CAN节点号
      * @param  pRBuff 要接收的缓冲区
      * @param  ulSize 要接收的数据长度
      * @retval 0-成功 非0-失败
      */
    uBit32 (*pf_CAN_RecvPack)(uBit8 uCanNode, uBit32 *pID, uBit8 *pRBuff, uBit32 *pSize);


    /**
      * @brief  数据接收处理(接收线程)
      * @param  uCanNode CAN节点号
      * @retval None
      * @retval 本函数是应用于轮询方式,单独一个线程做数据接收
      */
    void (*pf_CAN_RecvHandler)(uBit8 uCanNode);
    
    
}CMU_CAN_INTERFACE;



#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CMU CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uCanNode CAN节点
  * @param  pCanInterface CAN接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_CAN_SetInterface(uBit8 uCanNode, CMU_CAN_INTERFACE *pCanInterface);


/**
  * @brief  CMU ENEI 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_CAN_CheckInterfaceValid(void);


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_Open(void);


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_Close(void);


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_CAN_RecvHandler(void);


/**
  * @brief  数据包获取
  * @param  pID(出参)
  * @param  pRcvBuf 要发送的数据缓冲区(出参)
  * @param  pRcvLen 数据缓冲区长度(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen);


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 数据缓冲区长度
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen);


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  None
  * @retval 数据长度
  */
uBit32 CMU_CAN_GetMaxSendLen(void);



#ifdef __cplusplus
}
#endif

#endif /* __CMU_CAN_INTERFACE_H */
