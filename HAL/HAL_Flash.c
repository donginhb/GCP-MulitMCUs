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


/**
  * @brief  FLASH写入函数
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
Bit32 HAL_FLASH_Write(uBit32 ulFlashAddr, uBit8 *pWriteBuff, uBit32 ulByteSize)
{

    
    return 0;
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

    
    
    return ulByteSize;
}



