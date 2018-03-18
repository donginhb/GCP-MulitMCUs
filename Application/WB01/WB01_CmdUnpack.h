#ifndef  __WB01_CMD_UNPACK_H
#define  __WB01_CMD_UNPACK_H

#include "DataType/DataType.h"




/*****************************************************************************
 * 数据打包接口
 ****************************************************************************/

/**
  * @brief  数据打包
  * @param  pSendBuff 要发送的数据缓冲区
  * @param  ulSendLen 发送数据的长度
  * @retval 0-成功  非0-失败
  */
uBit32 WB01_Pack(uBit8* pSendBuff, uBit32 ulSendLen);

/*****************************************************************************
 * 数据解包接口
 ****************************************************************************/

/**
  * @brief  数据解包
  * @param  pRcvBuf 接收数据缓冲区
  * @param  pRcvLen 接收到的数据(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 WB01_UnPack(uBit8* pRcvBuf, uBit32* pRcvLen);



#endif /* __WB01_CMD_UNPACK_H */
