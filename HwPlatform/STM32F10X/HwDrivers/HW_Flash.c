/**
  ******************************************************************************
  * @file    HW_Flash.c
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
#include "HW_Flash.h"
#include "chip.h"


#define FLASH_OPERA_RETRY_NUM   (20)            //失败重试的次数

#define  SIZE_1K                (1024)
#define  SIZE_256B              (SIZE_1K/4)  
#define  SIZE_4K                (SIZE_1K*4)
#define  SIZE_32K               (SIZE_1K*32)

#define FLASH_MAX_ADDR          (0X0000FFFF)    //Flash 最大地址(LPC1752)
#define FLASH_SECTOR_NUM        (16)            //总共16个扇区

typedef struct SLayout 
{
  int sectors;
  int size;
} Layout;

const Layout g_FlashLayout[] =
{
    {16,  SIZE_4K},   // 前16个扇区每个扇区的大小为 4k
    {14, SIZE_32K},   // 后14个扇区每个扇区的大小为 32k
};


/**
  * @brief  根据Flash地址获取扇区号
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,-1表示无效结果
  */
int32_t HW_FLASH_GetSectorByAddr(uint32_t ulFlashAddr)
{
    uint32_t ulCurAddr = 0;
    
    if (ulFlashAddr >= FLASH_MAX_ADDR)
    {
        return -1;
    }
    
    for (int i = 0; i < FLASH_SECTOR_NUM; i++)
    {
        if (i < g_FlashLayout[0].sectors)
        {
            ulCurAddr += g_FlashLayout[0].size;
        }
        else 
        {
            ulCurAddr += g_FlashLayout[1].size;
        }
        
        if (ulFlashAddr < ulCurAddr)
        {
            return i;
        }
    }
    
    return -1;
}


/**
  * @brief  FLASH擦除函数
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval Flash操作结果
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
int32_t HW_FLASH_Erase(uint32_t ulStartSector, uint32_t ulEndSector)
{
    uint8_t uRetCode = 0xFF;
    
    //FLASH地址校验
    if (ulStartSector > ulEndSector)
    {
        return -1;
    }
    
    //禁止中断
    __disable_irq();
    
    //准备去擦除/写扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_PreSectorForReadWrite(ulStartSector, ulEndSector);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
    }
    
    //擦除扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_EraseSector(ulStartSector, ulEndSector);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
    }
    
    //使能中断
    __enable_irq();
    
    return uRetCode;
}


/**
  * @brief  FLASH写入函数
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
int32_t HW_FLASH_Write(uint32_t ulFlashAddr, uint8_t *pWriteBuff, uint32_t ulByteSize)
{
    uint8_t uRetCode = 0xFF;
    
    //校验FLASH地址
    if (ulFlashAddr > FLASH_MAX_ADDR)
    {
        return -1;
    }
    
    //校验FLASH地址边界对齐
    //flash地址以256为边界,禁止对非256字节对齐的地址进行写操作
    if (ulFlashAddr & 0xFF)
    {
        return -1;
    }
    
    //校验数据写入量
    //flash操作中,一次操作的字节数只能是以下几个之一
    if ((ulByteSize != 256) && (ulByteSize != 512) && (ulByteSize != 1024) && (ulByteSize != 4096))
    {
        return -1;
    }
    
    //获取扇区
    int32_t ulStartSector = HW_FLASH_GetSectorByAddr(ulFlashAddr);
    int32_t ulEndSector = HW_FLASH_GetSectorByAddr(ulFlashAddr + ulByteSize -1);
    
    //扇区校验
    if ((ulStartSector == -1) || (ulEndSector == -1))
    {
        return -1;
    }
    
    //禁止中断
    __disable_irq();
    
    //准备去擦除/写扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_PreSectorForReadWrite(ulStartSector, ulEndSector);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
    }
    
    //将数据写入Flash
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_CopyRamToFlash(ulFlashAddr, (uint32_t *)pWriteBuff, ulByteSize);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
    }
    
    //使能中断
    __enable_irq();
    
    return uRetCode;
}



/**
  * @brief  FLASH读取函数
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 读取的字节数,-1为读取失败(flash地址无效)
  */
int32_t HW_FLASH_Read(uint32_t ulFlashAddr, uint8_t *pRecvBuff, uint32_t ulByteSize)
{
    volatile uint8_t *pFlashAddr = (uint8_t *)ulFlashAddr; 
    
    //校验FLASH地址
    if (ulFlashAddr > FLASH_MAX_ADDR)
    {
        return -1;
    }
    
    //禁止中断
    __disable_irq();
    
    //数据读取
    for (int i = 0; i < ulByteSize; i++)
    {
        pRecvBuff[i] = pFlashAddr[i];
    }
    
    //使能中断
    __enable_irq();
    
    return ulByteSize;
}



