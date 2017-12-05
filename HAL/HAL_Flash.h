#ifndef  __HAL_FLASH_H
#define  __HAL_FLASH_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  根据Flash地址获取扇区号
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,-1表示无效结果
  */
Bit32 HAL_FLASH_GetSectorByAddr(uBit32 ulFlashAddr);


/**
  * @brief  FLASH擦除函数
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval Flash操作结果
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
Bit32 HAL_FLASH_Erase(uBit32 ulStartSector, uBit32 ulEndSector);


/**
  * @brief  FLASH写入函数
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 4096;
  *         LPC17xx最小操作单位是256Byte,LPC43xx最小操作单位是512Byte;
  *         必须对整扇区擦除之后才能执行写操作
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