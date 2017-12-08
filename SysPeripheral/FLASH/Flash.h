#ifndef  __FLASH_H
#define  __FLASH_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/*****************************************************************************
 * FLASH相关控制接口
 ****************************************************************************/

/**
  * @brief  FLASH最小写入字节获取
  * @param  None
  * @retval 最小写入的字节
  */
uBit32 FLASH_GetMinWriteByte(void);


/**
  * @brief  FLASH扇区号获取
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,0xFF表示无效结果
  */
uBit32 FLASH_GetSector(uBit32 ulFlashAddr);


/**
  * @brief  FLASH擦除
  * @param  ulFlashBank   FLASH块
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval 0-成功 非0-失败
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
uBit32 FLASH_Erase(uBit32 ulFlashBank, uBit32 ulStartSector, uBit32 ulEndSector);


/**
  * @brief  FLASH写入
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 0-成功 非0-失败
  * @note   写入的数据的字节数应当为 256, 512, 1024 4096;
  *         LPC17xx最小操作单位是256Byte,LPC43xx最小操作单位是512Byte;
  *         必须对整扇区擦除之后才能执行写操作
  */
uBit32 FLASH_Write(uBit32 ulFlashAddr, void *pWriteBuff, uBit32 ulByteSize);


/**
  * @brief  FLASH读取
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 实际读取到的字节数
  */
uBit32 FLASH_Read(uBit32 ulFlashAddr, void *pRecvBuff, uBit32 ulByteSize);


/**
  * @brief  FLASH校验
  * @param  ulFlashAddr Flash地址
  * @param  ulByteSize 字节数
  * @param  ulCheckSize 校验位数
  * @retval 实际的校验值
  */
uBit32 FLASH_GetCheckSum(uBit32 ulFlashAddr, uBit32 ulByteSize, uBit32 ulCheckSize);


#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H */
