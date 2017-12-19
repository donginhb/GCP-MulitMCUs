#ifndef  __CMU_UNPACK_H
#define  __CMU_UNPACK_H

#include "DataType/DataType.h"


/*****************************************************************************
 * CMU 数据解包接口
 ****************************************************************************/

/**
  * @brief  数据接收接口设置
  * @param  pf_RecvBuff 数据接口函数指针
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_SetUnpackRecvInterface(uBit32 (*pf_RecvBuff)(uBit8 *, uBit32));


/**
  * @brief  数据解包
  * @param  pRcvBuf 接收数据缓冲区
  * @param  pRcvLen 接收到的数据(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UnPack(uBit8* pRcvBuf, uBit32* pRcvLen);


#endif
