/**
  ******************************************************************************
  * @file    HW_Flash.c
  * @author  Duhanfneg
  * @version V1.1
  * @date    2017.08.22
  * @brief   Flash drivers
  ******************************************************************************
  * @attention
  * 
  * 设置重试机制,如果FLASH执行失败,重试一定次数.
  * 具体数值由宏FLASH_OPERA_RETRY_NUM 所定义.
  * 
  * 重要: 根据数据手册,每从RAM到FALSH写入256个字节的数据,占用时间为[0.95~1.05]MS
  * 
  * 
  * V1.1------------
  * 修改描述: 1.修改FLASH参数定义,以512K FLASH作为标准,以兼容LPC176X/5X全系芯片
  *           2.在FLASH操作失败重试前,增加一小段延时的操作;
  *           3.完善FLASH地址校验;
  * 修改作者: Duhanfeng
  * 当前版本: V1.1
  * 修改日期: 2017.12.07
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Flash.h"
#include "chip.h"


/*****************************************************************************
 * FLASH参数定义(不同芯片里面的不一样)
 ****************************************************************************/

//定义BANK_A的地址范围(LPC17xx)    
#define FLASH_BANK_A_START                          (0x00000000)    //FLASH区起始地址
#define FLASH_BANK_A_END                            (0x0007FFFF)    //FLASH区结束地址

#define LPC17XX_FLASH_MAX_SECTOR                    (30)    //最大扇区数
#define LPC17XX_FLASH_SMALL_CAPACITY_SECTOR         (16)    //小容量扇区数
#define LPC17XX_FLASH_BIG_CAPACITY_SECTOR           (14)    //大容量扇区数

#define LPC17XX_FLASH_MIX_WRITE_BYTE                (256)   //写操作最小的字节数

//定义flash内存尺寸
typedef enum
{
    SIZE_1K  = (1024)       ,
    SIZE_4K  = (SIZE_1K*4)  ,
    SIZE_8K  = (SIZE_1K*8)  ,
    SIZE_32K = (SIZE_1K*32) ,
    SIZE_64K = (SIZE_1K*64) ,
    
}FLASH_SIZE_T;


typedef struct 
{
    uint32_t ulSectors;
    uint32_t ulSize;
} FlashLayout;


//FLASH内部FLASH布局
static const FlashLayout m_FlashLayout[] =
{
    { LPC17XX_FLASH_SMALL_CAPACITY_SECTOR,    SIZE_4K },
    { LPC17XX_FLASH_BIG_CAPACITY_SECTOR ,     SIZE_32K}
  
};


/*****************************************************************************
 * FALSH相关操作参数定义及内部函数实现
 ****************************************************************************/

#pragma diag_suppress=Pe186 //屏蔽以下"ulFlashAddr >= FLASH_BANK_A_START"提示与0比较无意义的警告

#define FLASH_OPERA_RETRY_NUM   (20)            //失败重试的次数


/**
  * @brief  简单的延时函数
  * @param  None
  * @retval None
  * @note   在96MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  *         在204MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  */
static void HW_FLASH_SimpleDelay(void)
{
    for(volatile int i = 0; i < 10000; i++);
    
}


/**
  * @brief  Flash 地址检测
  * @param  ulFlashAddr Flash地址
  * @retval 0-成功 非0-失败
  */
static uint32_t HW_FLASH_CheckAddr(uint32_t ulFlashAddr)
{
    if ((ulFlashAddr >= FLASH_BANK_A_START) && (ulFlashAddr <= FLASH_BANK_A_END))
    {
        return 0;
    }

    return 1;
}


/*****************************************************************************
 * FALSH相关控制接口
 ****************************************************************************/

/**
  * @brief  FLASH最小写入字节获取
  * @param  None
  * @retval 最小写入的字节
  */
uint32_t HW_FLASH_GetMinWriteByte(void)
{
    
    return LPC17XX_FLASH_MIX_WRITE_BYTE;
}


/**
  * @brief  FLASH扇区号获取
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,0xFF表示无效结果
  */
uint32_t HW_FLASH_GetSector(uint32_t ulFlashAddr)
{
    uint32_t ulCurFlashAddr = 0;
    
    //校验FLASH地址
    if (HW_FLASH_CheckAddr(ulFlashAddr))
    {
        return 0xFF;
    }
    
    //计算扇区
    if (ulFlashAddr < (m_FlashLayout[0].ulSectors * m_FlashLayout[0].ulSize))
    {
        for (int i = 0; i < m_FlashLayout[0].ulSectors; i++)
        {
            ulCurFlashAddr += m_FlashLayout[0].ulSize;
            
            if (ulFlashAddr < ulCurFlashAddr)
            {
                return i;
            }
        }
    }
    else 
    {
        ulCurFlashAddr = m_FlashLayout[0].ulSectors * m_FlashLayout[0].ulSize;

        for (int i = 0; i < m_FlashLayout[1].ulSectors; i++)
        {
            ulCurFlashAddr += m_FlashLayout[1].ulSize;
            
            if (ulFlashAddr < ulCurFlashAddr)
            {
                return i + m_FlashLayout[0].ulSectors;  //扇区数偏移
            }
        }
    }
    
    return 0xFF;
}


/**
  * @brief  FLASH擦除
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval 0-成功 非0-失败
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
uint32_t HW_FLASH_Erase(uint32_t ulStartSector, uint32_t ulEndSector)
{
    uint8_t uRetCode = 0xFF;
    
    //校验FLASH地址
    if ((ulStartSector > ulEndSector) || (ulEndSector >= (LPC17XX_FLASH_MAX_SECTOR)))
    {
        return 0xFF;
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
        
        //若失败,则延时一小段时间再进行失败重试
        HW_FLASH_SimpleDelay();
    }
    
    //擦除扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_EraseSector(ulStartSector, ulEndSector);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
        
        //若失败,则延时一小段时间再进行失败重试
        HW_FLASH_SimpleDelay();
    }
    
    //使能中断
    __enable_irq();
    
    return uRetCode;
}


/**
  * @brief  FLASH写入
  * @param  ulFlashAddr Flash地址
  * @param  pWriteBuff 写入缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 0-成功 非0-失败
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
uint32_t HW_FLASH_Write(uint32_t ulFlashAddr, void *pWriteBuff, uint32_t ulByteSize)
{
    uint8_t uRetCode = 0xFF;
    
    //校验FLASH地址
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize)))
    {
        return 0xFF;
    }
    
    //校验FLASH地址边界对齐
    //flash地址以256为边界,禁止对非256字节对齐的地址进行写操作
    if (ulFlashAddr & 0xFF)
    {
        return 0xFF;
    }
    
    //校验数据写入量
    //flash操作中,一次操作的字节数只能是以下几个之一
    if ((ulByteSize != 256) && (ulByteSize != 512) && (ulByteSize != 1024) && (ulByteSize != 4096))
    {
        return 0xFF;
    }
    
    //获取扇区
    int32_t ulStartSector = HW_FLASH_GetSector(ulFlashAddr);
    int32_t ulEndSector = HW_FLASH_GetSector(ulFlashAddr + ulByteSize -1);
    
    //扇区校验
    if ((ulStartSector == 0xFF) || (ulEndSector == 0xFF))
    {
        return 0xFF;
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
        
        //若失败,则延时一小段时间再进行失败重试
        HW_FLASH_SimpleDelay();
    }
    
    //将数据写入Flash
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_CopyRamToFlash(ulFlashAddr, (uint32_t *)pWriteBuff, ulByteSize);
        
        if (uRetCode == IAP_CMD_SUCCESS)
        {
            break;
        }
        
        //若失败,则延时一小段时间再进行失败重试
        HW_FLASH_SimpleDelay();
    }
    
    //使能中断
    __enable_irq();
    
    return uRetCode;
}


/**
  * @brief  FLASH读取
  * @param  ulFlashAddr Flash地址
  * @param  pRecvBuff 读取缓冲区的指针
  * @param  ulByteSize 字节数
  * @retval 实际读取到的字节数
  */
uint32_t HW_FLASH_Read(uint32_t ulFlashAddr, void *pRecvBuff, uint32_t ulByteSize)
{
    volatile uint8_t *pFlashAddr = (uint8_t *)ulFlashAddr; 
    uint8_t *pRecvPtr = pRecvBuff;
    
    //校验FLASH地址
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize)))
    {
        return 0;
    }
    
    //禁止中断
    __disable_irq();
    
    //数据读取
    for (int i = 0; i < ulByteSize; i++)
    {
        pRecvPtr[i] = pFlashAddr[i];
    }
    
    //使能中断
    __enable_irq();
    
    return ulByteSize;
}


/**
  * @brief  FLASH校验
  * @param  ulFlashAddr Flash地址
  * @param  ulByteSize 字节数
  * @param  ulCheckSize 校验位数
  * @retval 实际的校验值
  */
uint32_t HW_FLASH_GetCheckSum(uint32_t ulFlashAddr, uint32_t ulByteSize, uint32_t ulCheckSize)
{
    volatile uint8_t *pFlashAddr = (uint8_t *)ulFlashAddr; 
    
    //校验FLASH地址
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize)))
    {
        return 0;
    }
    
    switch (ulCheckSize)
    {
    case 1: 
        {
            uint8_t uCheckSum = 0;
            
            for (uint32_t i = 0; i < ulByteSize; i++)
            {
                uCheckSum += pFlashAddr[i];
            }
            
            return uCheckSum;
        }
    case 2:
        {
            uint16_t nCheckSum = 0;
            
            for (uint32_t i = 0; i < ulByteSize; i++)
            {
                nCheckSum += pFlashAddr[i];
            }
            
            return nCheckSum;
        }
    case 4:
        {
            uint32_t ulCheckSum = 0;
            
            for (uint32_t i = 0; i < ulByteSize; i++)
            {
                ulCheckSum += pFlashAddr[i];
            }
            
            return ulCheckSum;
            
        }
    default: return 0;
    
    }
    
}


