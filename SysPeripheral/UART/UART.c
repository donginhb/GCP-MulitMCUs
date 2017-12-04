/**
  ******************************************************************************
  * @file    Uart.c
  * @author  Duhanfneg
  * @version V4.2
  * @date    2017.11.17
  * @brief   UART drivers
  ******************************************************************************
  * @attention
  * 
  *   
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Uart.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Uart.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * UART 基本功能接口
 ****************************************************************************/

/**
  * @brief  串口初始化
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @retval 0-成功  非0-失败
  */
uBit32 UART_Init(uBit8 uUartNode, uBit32 ulBaudRate)
{
    HAL_UART_Init(uUartNode, ulBaudRate);
    
    return 0;
}


/**
  * @brief  串口打开
  * @param  uUartNode 串口节点号
  * @retval 0-成功  非0-失败
  */
uBit32 UART_Open(uBit8 uUartNode)
{
    HAL_UART_Open(uUartNode);
    
    return 0;
}


/**
  * @brief  串口关闭
  * @param  uUartNode 串口节点号
  * @retval 0-成功  非0-失败
  */
uBit32 UART_Close(uBit8 uUartNode)
{
    HAL_UART_Close(uUartNode);
    
    return 0;
}
    
    
/**
  * @brief  串口发送状态获取
  * @param  uUartNode 串口节点号
  * @retval 0-发送中 非0-发送完成
  */
uBit32 UART_GetTransStatus(uBit8 uUartNode)
{
    HAL_UART_GetTransStatus(uUartNode);
    
    return 0;
}


/**
  * @brief  串口数据发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval 0-成功  非0-失败
  */
uBit32 UART_BlockSendBuff(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize)
{
    HAL_UART_BlockSendBuff(uUartNode, pSBuff, ulSize);
    
    return 0;
}


/**
  * @brief  串口字符串发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSString 字符串区地址(结尾必须有'\0')
  * @retval 0-成功  非0-失败
  */
uBit32 UART_BlockSendStr(uBit8 uUartNode, uBit8 *pSString)
{
    HAL_UART_BlockSendStr(uUartNode, pSString);
    
    return 0;
}


/**
  * @brief  串口数据发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval 0-成功  非0-失败
  */
uBit32 UART_SendBuff(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize)
{
    HAL_UART_SendBuff(uUartNode, pSBuff, ulSize);
    
    return 0;
}


/**
  * @brief  串口数据发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSString 字符串区地址(结尾必须有'\0')
  * @retval 0-成功  非0-失败
  */
uBit32 UART_SendStr(uBit8 uUartNode, uBit8 *pSString)
{
    HAL_UART_SendStr(uUartNode, pSString);
    
    return 0;
}


/**
  * @brief  串口接收(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uBit32 实际上接收到的数据长度
  */
uBit32 UART_RecvBuff(uBit8 uUartNode, uBit8 *pRBuff, uBit32 ulSize)
{
    
    return HAL_UART_RecvBuff(uUartNode, pRBuff, ulSize);
}


/**
  * @brief  数据接收处理(接收线程)
  * @param  uUartNode 串口节点号
  * @retval None
  * @retval 本函数是应用于轮训方式,单独一个线程做数据接收
  */
void UART_RecvHandler(uBit8 uUartNode)
{
    HAL_UART_RecvHandler(uUartNode);
    
}
