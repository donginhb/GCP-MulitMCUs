#ifndef  __HAL_FLASH_H
#define  __HAL_FLASH_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  FLASH写入函数
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
Bit32 HAL_FLASH_Write(uBit32 ulFlashAddr, uBit8 *pWriteBuff, uBit32 ulByteSize);

/**
  * @brief  FLASH读取函数
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 读取的字节数,-1为读取失败(flash地址无效)
  */
Bit32 HAL_FLASH_Read(uBit32 ulFlashAddr, uBit8 *pRecvBuff, uBit32 ulByteSize);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_FLASH_H */