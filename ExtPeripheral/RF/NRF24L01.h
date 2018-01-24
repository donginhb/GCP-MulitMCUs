#ifndef  __NRF24L01_H
#define  __NRF24L01_H

#include "DataType/DataType.h"

#define TX_ADR_WIDTH            (5)         //发射地址宽度
#define TX_PLOAD_WIDTH          (32)        //发射数据通道有效数据宽度0~32Byte

#define RX_ADR_WIDTH            (5)         //接收地址宽度
#define RX_PLOAD_WIDTH          (32)        //接收数据通道有效数据宽度0~32Byte

#define CHANAL                  (40)        //频道选择 

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * NRF相关控制接口
 ****************************************************************************/

/**
  * @brief  NRF24L01 硬件接口
  * @param  None
  * @retval 0-成功 1-失败
  */
uBit32 nRF24L01_HwCtrlInterFaces(uBit8 (*SPI_ReadWriteByte)(uBit8 cWriteData),
                                 void (*SetCSN)(bool bState),
                                 void (*SetCE)(bool bState),
                                 bool (*GetIRQ)(void),
                                 void (*BitDelayUs)(uBit32 ulUs));


/**
  * @brief  接收模式进入
  * @param  None
  * @retval None
  */
void nRF24L01_EnterRxMode(void);


/**
  * @brief  发送模式进入
  * @param  None
  * @retval None
  */
void nRF24L01_EnterTxMode(void);


/**
  * @brief  NRF连接检测
  * @param  None
  * @retval 0-成功 1-失败
  */
uBit32 nRF24L01_CheckConnect(void);


/**
  * @brief  NRF状态获取
  * @param  None
  * @retval 实际状态
  */
uBit32 nRF24L01_GetStatus(void);


/**
  * @brief  NRF地址设置
  * @param  pRFAddr 地址指针,长度为5个字节
  * @retval None
  * @note   发送和接收的节点需要相同地址才能通信
  */
void nRF24L01_SetAddress(uBit8 *pRFAddr);


/**
  * @brief  数组数据发送
  * @param  pTBuff 发送缓冲区,最大31个字节
  * @param  ulSize 要发送的数据字节数
  * @retval 发送结果
  * @note   值得注意的是,这个函数不能在本模块的中断里面执行
  */
uBit8 nRF24L01_SendBuff(uBit8 *pTBuff, uBit8 uSize);


/**
  * @brief  数据接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uBit32 nRF24L01_RecvBuff(void *pRBuff, uBit32 ulSize);


/**
  * @brief  接收处理
  * @param  None
  * @retval None
  */
void nRF24L01_RecvHandler(void);


#ifdef __cplusplus
}
#endif
                       
#endif /* __NRF24L01_H */
