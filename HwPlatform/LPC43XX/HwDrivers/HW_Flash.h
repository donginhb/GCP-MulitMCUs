#ifndef  __HW_FLASH_H
#define  __HW_FLASH_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


//硬件FLASH块定义
#define HW_FLASH_M4_BANK        (0)     //M4 FLASH块定义
#define HW_FLASH_M0_BANK        (1)     //M0 FLASH块定义


/*****************************************************************************
 * FALSH相关操作接口函数
 ****************************************************************************/

/**
  * @brief  FLASH初始化
  * @param  None
  * @retval Flash操作结果
  */
uint32_t HW_FLASH_Init(void);


/**
  * @brief  FLASH最小写入字节获取
  * @param  None
  * @retval 最小写入的字节
  */
uint32_t HW_FLASH_GetMinWriteByte(void);


/**
  * @brief  FLASH扇区号获取
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,-1表示无效结果
  */
uint32_t HW_FLASH_GetSector(uint32_t ulFlashAddr);


/**
  * @brief  FLASH擦除
  * @param  ulFlashBank   FLASH块 0-M4_FLASH  1-M0_FLASH
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval Flash操作结果
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
uint32_t HW_FLASH_Erase(uint32_t ulFlashBank, uint32_t ulStartSector, uint32_t ulEndSector);


/**
  * @brief  FLASH写入
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
uint32_t HW_FLASH_Write(uint32_t ulFlashAddr, uint8_t *pWriteBuff, uint32_t ulByteSize);


/**
  * @brief  FLASH读取
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 实际读取到的字节数
  */
uint32_t HW_FLASH_Read(uint32_t ulFlashAddr, void *pRecvBuff, uint32_t ulByteSize);


/**
  * @brief  FLASH校验
  * @param  ulFlashAddr Flash地址
  * @param  ulByteSize 字节数
  * @param  ulCheckSize 校验位数
  * @retval 实际的校验值
  */
uint32_t HW_FLASH_GetCheckSum(uint32_t ulFlashAddr, uint32_t ulByteSize, uint32_t ulCheckSize);


#ifdef __cplusplus
}
#endif

#endif /* __HW_FLASH_H */
