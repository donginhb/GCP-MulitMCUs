#ifndef  __RF_H
#define  __RF_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * RF相关控制接口
 ****************************************************************************/

/**
  * @brief  RF模块初始化
  * @param  uSpiNode SPI节点号
  * @param  ulIRQNo  IRQ引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCSNNo  CSN引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCENo   CE引脚编号(IO资源表中输出IO组的位置)
  * @retval 0-成功 非0-失败
  */
uBit32 RF_Init(uBit8 uSpiNode, uBit32 ulIRQNo, uBit32 ulCSNNo, uBit32 ulCENo);


/**
  * @brief  NRF地址设置
  * @param  pRFAddr 地址指针,长度为5个字节
  * @retval None
  * @note   发送和接收的节点需要相同地址才能通信
  */
void RF_SetAddress(uBit8 *pRFAddr);


/**
  * @brief  数组数据发送
  * @param  pTBuff 发送缓冲区,最大31个字节
  * @param  ulSize 要发送的数据字节数
  * @retval 发送结果
  * @note   值得注意的是,这个函数不能在本模块的中断里面执行
  */
uBit8 RF_SendBuff(uBit8 *pTBuff, uBit8 uSize);


/**
  * @brief  数据接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uBit32 RF_RecvBuff(void *pRBuff, uBit32 ulSize);


/**
  * @brief  接收处理
  * @param  None
  * @retval None
  */
void RF_RecvHandler(void);
    

#ifdef __cplusplus
}
#endif

#endif /* __RF_H */
