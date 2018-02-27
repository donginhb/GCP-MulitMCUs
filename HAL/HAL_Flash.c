/**
  ******************************************************************************
  * @file    HAL_Flash.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.08.22
  * @brief   Flash drivers
  ******************************************************************************
  * @attention
  * 
  * 设置重试机制,如果FLASH执行失败,重试一定次数.
  * 具体数值由宏FLASH_OPERA_RETRY_NUM 所定义
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Flash.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_Flash.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#ifndef FLASH_BOOT_BANK
#define FLASH_BOOT_BANK                 (0)                 //BootloaderFLASH块
#endif

#ifndef FLASH_USER_BANK
#define FLASH_USER_BANK                 (1)                 //用户数据区FLASH块
#endif

#ifndef FLASH_APP_BANK
#define FLASH_APP_BANK                  (2)                 //应用程序FLASH块
#endif

#ifndef FLASH_SUB_APP_BANK
#define FLASH_SUB_APP_BANK              (3)                 //副应用程序FLASH块
#endif


/*****************************************************************************
 * FALSH相关控制接口
 ****************************************************************************/

/**
  * @brief  FLASH最小写入字节获取
  * @param  None
  * @retval 最小写入的字节
  */
uBit32 HAL_FLASH_GetMinWriteByte(void)
{
    
    return HW_FLASH_GetMinWriteByte();
}


/**
  * @brief  FLASH扇区号获取
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,0xFF表示无效结果
  */
uBit32 HAL_FLASH_GetSector(uBit32 ulFlashAddr)
{
    
    return HW_FLASH_GetSector(ulFlashAddr);
}


/**
  * @brief  FLASH擦除
  * @param  ulFlashBank   FLASH块
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval 0-成功 非0-失败
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
uBit32 HAL_FLASH_Erase(uBit32 ulFlashBank, uBit32 ulStartSector, uBit32 ulEndSector)
{
    uBit32 ulRet = 1;
    
#if defined(LPC17XX)
    ulRet = HW_FLASH_Erase(ulStartSector, ulEndSector);
    
#elif defined(LPC43XX)
    
    //计算实际硬件FLASH块
    uBit32 ulHwFlashBank = HW_FLASH_M4_BANK;
    
    switch (ulFlashBank)
    {
    case FLASH_BOOT_BANK:    ulHwFlashBank = HW_FLASH_M4_BANK; break;
    case FLASH_USER_BANK:    ulHwFlashBank = HW_FLASH_M4_BANK; break;
    case FLASH_APP_BANK:     ulHwFlashBank = HW_FLASH_M4_BANK; break;
    case FLASH_SUB_APP_BANK: ulHwFlashBank = HW_FLASH_M0_BANK; break;
    default: break;
    }
    
    ulRet = HW_FLASH_Erase(ulHwFlashBank, ulStartSector, ulEndSector);
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    
#endif
    
    return ulRet;
}


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
uBit32 HAL_FLASH_Write(uBit32 ulFlashAddr, void *pWriteBuff, uBit32 ulByteSize)
{
    
    return HW_FLASH_Write(ulFlashAddr, pWriteBuff, ulByteSize);
}


/**
  * @brief  FLASH读取
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 实际读取到的字节数
  */
uBit32 HAL_FLASH_Read(uBit32 ulFlashAddr, void *pRecvBuff, uBit32 ulByteSize)
{
    
    return HW_FLASH_Read(ulFlashAddr, pRecvBuff, ulByteSize);
}


/**
  * @brief  FLASH校验
  * @param  ulFlashAddr Flash地址
  * @param  ulByteSize 字节数
  * @param  ulCheckSize 校验位数
  * @retval 实际的校验值
  */
uBit32 HAL_FLASH_GetCheckSum(uBit32 ulFlashAddr, uBit32 ulByteSize, uBit32 ulCheckSize)
{
    
    return HW_FLASH_GetCheckSum(ulFlashAddr, ulByteSize, ulCheckSize);
}
