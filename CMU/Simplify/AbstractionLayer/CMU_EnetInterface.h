#ifndef  __CMU_ENET_INTERFACE_H
#define  __CMU_ENET_INTERFACE_H

#include "DataType/DataType.h"

//CMU ENET 接口定义
typedef struct
{
    /**
      * @brief  ENET打开
      * @param  None
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_ENET_Open)(void);


    /**
      * @brief  ENET关闭
      * @param  None
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_ENET_Close)(void);
    
    
    /**
      * @brief  ENET数据发送(非阻塞)
      * @param  pSBuff 数据缓冲区地址
      * @param  nSize 发送的数量
      * @retval None
      */
    uBit32 (*pf_ENET_SendBuff)(uBit8 *pSBuff, uBit32 ulSize);
    
    
    /**
      * @brief  ENET接收(非阻塞)
      * @param  None
      * @param  pRBuff 要接收的缓冲区
      * @param  ulSize 要接收的数据长度
      * @retval 实际上接收到的数据长度
      */
    uBit32 (*pf_ENET_RecvBuff)(uBit8 *pRBuff, uBit32 ulSize);


    /**
      * @brief  数据接收处理(接收线程)
      * @param  None
      * @retval None
      * @retval 本函数是应用于轮训方式,单独一个线程做数据接收
      */
    void (*pf_ENET_RecvHandler)(void);
    
    
}CMU_ENET_INTERFACE;



#ifdef __cplusplus
extern "C"
{
#endif



/*****************************************************************************
 * CMU ENET相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU ENET 接口设置
  * @param  pEnetInterface 以太网接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_ENET_SetInterface(CMU_ENET_INTERFACE *pEnetInterface);
      

/**
  * @brief  CMU ENEI 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_ENET_CheckInterfaceValid(void);


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_Open(void);


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_Close(void);


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_ENET_RecvHandler(void);


/**
  * @brief  数据包获取
  * @param  pID(出参)
  * @param  pRcvBuf 要发送的数据缓冲区(出参)
  * @param  pRcvLen 数据缓冲区长度(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen);


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 数据缓冲区长度
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen);


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  None
  * @retval 数据长度
  */
uBit32 CMU_ENET_GetMaxSendLen(void);



#ifdef __cplusplus
}
#endif

#endif /* __CMU_ENET_INTERFACE_H */
