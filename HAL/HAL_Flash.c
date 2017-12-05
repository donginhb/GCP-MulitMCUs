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


/**
  * @brief  根据Flash地址获取扇区号
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,-1表示无效结果
  */
Bit32 HAL_FLASH_GetSectorByAddr(uBit32 ulFlashAddr)
{
    Bit32 iRet = 0;
#if defined(LPC17XX)
    iRet = HW_FLASH_GetSectorByAddr(ulFlashAddr);
#endif
    
    return iRet;
}


/**
  * @brief  FLASH擦除函数
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval Flash操作结果
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
Bit32 HAL_FLASH_Erase(uBit32 ulStartSector, uBit32 ulEndSector)
{
    Bit32 iRet = 0;
#if defined(LPC17XX)
    iRet = HW_FLASH_Erase(ulStartSector, ulEndSector);
#endif
    
    return iRet;
}


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
Bit32 HAL_FLASH_Write(uBit32 ulFlashAddr, uBit8 *pWriteBuff, uBit32 ulByteSize)
{
    Bit32 iRet = 0;
#if defined(LPC17XX)
    iRet = HW_FLASH_Write(ulFlashAddr, pWriteBuff, ulByteSize);
#endif

    return iRet;
}


/**
  * @brief  FLASH读取函数
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 读取的字节数,-1为读取失败(flash地址无效)
  */
Bit32 HAL_FLASH_Read(uBit32 ulFlashAddr, uBit8 *pRecvBuff, uBit32 ulByteSize)
{
    Bit32 iRet = 0;
#if defined(LPC17XX)
    iRet = HW_FLASH_Read(ulFlashAddr, pRecvBuff, ulByteSize);
#endif
    
    return iRet;
}
