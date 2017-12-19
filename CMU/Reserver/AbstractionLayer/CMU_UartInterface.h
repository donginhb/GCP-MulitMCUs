#ifndef  __CMU_UART_INTERFACE_H
#define  __CMU_UART_INTERFACE_H

#include "DataType/DataType.h"

//CMU UART 接口定义
typedef struct
{
    /**
      * @brief  串口打开
      * @param  uUartNode 串口节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_Open)(uBit8 uUartNode);


    /**
      * @brief  串口关闭
      * @param  uUartNode 串口节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_Close)(uBit8 uUartNode);
    
    
    /**
      * @brief  串口数据发送(非阻塞)
      * @param  uUartNode 串口节点号
      * @param  pSBuff 数据缓冲区地址
      * @param  nSize 发送的数量
      * @retval None
      */
    uBit32 (*pf_UART_SendBuff)(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize);
    
    
    /**
      * @brief  串口接收(非阻塞)
      * @param  uUartNode 串口节点号
      * @param  pRBuff 要接收的缓冲区
      * @param  ulSize 要接收的数据长度
      * @retval 实际上接收到的数据长度
      */
    uBit32 (*pf_UART_RecvBuff)(uBit8 uUartNode, uBit8 *pRBuff, uBit32 ulSize);


    /**
      * @brief  数据接收处理(接收线程)
      * @param  uUartNode 串口节点号
      * @retval None
      * @retval 本函数是应用于轮训方式,单独一个线程做数据接收
      */
    void (*pf_UART_RecvHandler)(uBit8 uUartNode);
    
    
}CMU_UART_INTERFACE;



#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CMU UART相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uUartNode 串口节点
  * @param  pUartInterface 串口接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_UART_SetInterface(uBit8 uUartNode, CMU_UART_INTERFACE *pUartInterface);


/**
  * @brief  CMU UART 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_UART_CheckInterfaceValid(void);


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_Open(void);


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_Close(void);


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_UART_RecvHandler(void);


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  pRcvLen 数据缓冲区长度(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen);


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 数据缓冲区长度
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen);


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  uUartNode 串口节点号
  * @retval 数据长度
  */
uBit32 CMU_UART_GetMaxSendLen(void);



#ifdef __cplusplus
}
#endif

#endif /* __CMU_UART_INTERFACE_H */
