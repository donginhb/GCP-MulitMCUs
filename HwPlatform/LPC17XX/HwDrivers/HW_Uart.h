#ifndef  __HW_UART_H
#define  __HW_UART_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * UART 基本功能接口
 ****************************************************************************/

/**
  * @brief  串口初始化
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @retval None
  * Note    当调用初始化函数后,无需再调用使能函数去使能串口
  */
void HW_UART_Init(uint8_t uUartNode, uint32_t ulBaudRate);


/**
  * @brief  串口使能
  * @param  uUartNode 串口节点号
  * @param  bIsEnable 使能标志
  * @retval None
  */
void HW_UART_Enable(uint8_t uUartNode, bool bIsEnable);


/**
  * @brief  串口发送状态获取
  * @param  uUartNode 串口节点号
  * @retval 0-发送中 非0-发送完成
  */
uint32_t HW_UART_GetTransStatus(uint8_t uUartNode);


/**
  * @brief  单字节发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  uData 要发送的字节数据
  * @retval None
  */
void HW_UART_SendByte(uint8_t uUartNode, uint8_t uData);


/**
  * @brief  缓冲区发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的缓冲区
  * @param  ulSize 要发送的字节数
  * @retval None
  */
void HW_UART_SendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize);


/**
  * @brief  字符串发送函数(阻塞性发送)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HW_UART_SendStr(uint8_t uUartNode, uint8_t *pSBuff);


/**
  * @brief  串口接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  uUartNode 串口节点号
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uint32_t HW_UART_RecvBuff(uint8_t uUartNode, void *pRBuff, uint32_t ulSize);


/**
  * @brief  数据接收处理(非阻塞接收线程)
  * @param  uUartNode 串口节点号
  * @retval None
  * @retval 本函数是应用于非中断接收的情况,单独一个线程做数据接收
  */
void HW_UART_RecvHandler(uint8_t uUartNode);


/*****************************************************************************
 * DMA链表传输接口
 ****************************************************************************/

/**
  * @brief  串口数据DMA发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void HW_UART_DmaSendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize);


/**
  * @brief  串口字符串DMA发送函数(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HW_UART_DmaSendStr(uint8_t uUartNode, uint8_t *pSBuff);


#ifdef __cplusplus
}
#endif

#endif /* __HW_UART_H */

