/**
  ******************************************************************************
  * @file    hw_flash.c
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
  * 重要: 根据数据手册,每从RAM到FALSH写入512个字节的数据,占用时间为1MS
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "hw_flash.h"
#include "chip.h"

/*****************************************************************************
 * FLASH参数定义(不同芯片里面的不一样)
 ****************************************************************************/

//#define FLASH_MAX_ADDR          (0X0000FFFF)    //Flash 最大地址(LPC1752)
//#define FLASH_SECTOR_NUM        (15)            //总共15个扇区

//定义BANK_A和BANK_B的地址范围(LPC43xx)    
#define FLASH_BANK_A_START                  (0x1A000000)
#define FLASH_BANK_A_END                    (0x1A07FFFF)
#define FLASH_BANK_B_START                  (0x1B000000)
#define FLASH_BANK_B_END                    (0x1B07FFFF)

//定义LCP43XX系列各型号MCU的扇区数
#define LPC43XX_FLASH_MAX_SECTOR                    (30)    //最大扇区数
#define LPC43XX_FLASH_SMALL_CAPACITY_SECTOR         (8)     //小容量扇区数
#define LPC43X3_FLASH_BIG_CAPACITY_SECTOR           (11 - LPC43XX_FLASH_SMALL_CAPACITY_SECTOR)
#define LPC43X5_FLASH_BIG_CAPACITY_SECTOR           (13 - LPC43XX_FLASH_SMALL_CAPACITY_SECTOR)
#define LPC43X7_FLASH_BIG_CAPACITY_SECTOR           (15 - LPC43XX_FLASH_SMALL_CAPACITY_SECTOR)


#define LPC43XX_FLASH_MIN_WRITE_BYTE                (512)   //写操作最小的字节数


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
    { LPC43XX_FLASH_SMALL_CAPACITY_SECTOR,    SIZE_8K },
    { LPC43X7_FLASH_BIG_CAPACITY_SECTOR ,     SIZE_64K}
  
};


/*****************************************************************************
 * FALSH相关操作参数定义及内部函数实现
 ****************************************************************************/

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
    if (((ulFlashAddr >= FLASH_BANK_A_START) && (ulFlashAddr <= FLASH_BANK_A_END)) ||
        ((ulFlashAddr >= FLASH_BANK_B_START) && (ulFlashAddr <= FLASH_BANK_B_END)))
    {
        return 0;
    }

    return 1;
}


/**
  * @brief  Flash
  * @param  ulFlashAddr Flash地址
  * @retval 当前地址的地址块,0xFF表示无效地址
  */
static uint32_t HW_FLASH_GetFlashBank(uint32_t ulFlashAddr)
{
    uint32_t ulFlashBank = 0xFF;
    
    //计算基地址
    if ((ulFlashAddr >= FLASH_BANK_A_START) && (ulFlashAddr <= FLASH_BANK_A_END))
    {
        ulFlashBank = IAP_FLASH_BANK_A;
    }
    else if ((ulFlashAddr >= FLASH_BANK_B_START) && (ulFlashAddr <= FLASH_BANK_B_END))
    {
        ulFlashBank = IAP_FLASH_BANK_B;
    }

    return ulFlashBank;
}


/*****************************************************************************
 * FALSH相关操作接口函数
 ****************************************************************************/

/**
  * @brief  FLASH初始化
  * @param  None
  * @retval Flash操作结果
  */
uint32_t HW_FLASH_Init(void)
{
    
    return Chip_IAP_Init();
}


/**
  * @brief  FLASH最小写入字节获取
  * @param  None
  * @retval 最小写入的字节
  */
uint32_t HW_FLASH_GetMinWriteByte(void)
{
    
    return LPC43XX_FLASH_MIN_WRITE_BYTE;
}


/**
  * @brief  FLASH扇区号获取
  * @param  ulFlashAddr Flash地址
  * @retval 扇区号,-1表示无效结果
  */
uint32_t HW_FLASH_GetSector(uint32_t ulFlashAddr)
{
    uint32_t ulCurFlashAddr = 0;    //当前计算的FLASH地址
    uint32_t ulBaseFlashAddr = 0;   //当前地址的FALSH基地址
    uint32_t ulRelFlashAddr = 0;    //当前地址于基地址的相对地址
    uint32_t ulFalshBank = 0;       //FLASH操作块
    
    //计算基地址
    ulFalshBank = HW_FLASH_GetFlashBank(ulFlashAddr);

    if (ulFalshBank == IAP_FLASH_BANK_A)
    {
        ulBaseFlashAddr = FLASH_BANK_A_START;
    }
    else if (ulFalshBank == IAP_FLASH_BANK_B)
    {
        ulBaseFlashAddr = FLASH_BANK_B_START;
    }
    else
    {
        return 0xFF;
    }
    
    //获取相对地址
    ulRelFlashAddr = ulFlashAddr - ulBaseFlashAddr;
    
    //计算扇区
    if (ulRelFlashAddr < (m_FlashLayout[0].ulSectors * m_FlashLayout[0].ulSize))
    {
        for (int i = 0; i < m_FlashLayout[0].ulSectors; i++)
        {
            ulCurFlashAddr += m_FlashLayout[0].ulSize;
            
            if (ulRelFlashAddr < ulCurFlashAddr)
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
            
            if (ulRelFlashAddr < ulCurFlashAddr)
            {
                return i + 8;
            }
        }
    }
    
    return 0xFF;  //非法的地址返回0xFF
}


/**
  * @brief  FLASH擦除
  * @param  ulFlashBank   FLASH块 0-M4_FLASH  1-M0_FLASH
  * @param  ulStartSector 开始扇区
  * @param  ulEndSector   结束扇区
  * @retval Flash操作结果
  * @note   当擦除一个扇区时,开始扇区等于结束扇区
  */
uint32_t HW_FLASH_Erase(uint32_t ulFlashBank, uint32_t ulStartSector, uint32_t ulEndSector)
{
    uint8_t uRetCode = 0xFF;
    
    //校验FLASH地址
    if (ulStartSector > ulEndSector)
    {
        return 0xFF;
    }
    
    //禁止中断
    __disable_irq();
    
    //准备去擦除/写扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_PreSectorForReadWrite(ulStartSector, ulEndSector, ulFlashBank);
        
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
        uRetCode = Chip_IAP_EraseSector(ulStartSector, ulEndSector, ulFlashBank);
        
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
  * @retval Flash操作结果
  * @note   写入的数据的字节数应当为 256, 512, 1024 或 4096
  */
uint32_t HW_FLASH_Write(uint32_t ulFlashAddr, uint8_t *pWriteBuff, uint32_t ulByteSize)
{
    uint8_t uRetCode = 0xFF;
    
    //校验FLASH地址
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize -1)))
    {
        return 0xFF;
    }
    
    //校验FLASH地址边界对齐
    //flash地址以256为边界,严禁对非256字节对齐的地址进行写操作
    if (ulFlashAddr & 0xFF)
    {
        return 0xFF;
    }
    
    //校验数据写入量
    //flash操作中,一次操作的字节数只能是以下几个之一
    if ((ulByteSize != 512) && (ulByteSize != 1024) && (ulByteSize != 4096))
    {
        return 0xFF;
    }
    
    //获取FLASH块
    uint32_t ulFlashBank = HW_FLASH_GetFlashBank(ulFlashAddr);
    
    //校验FLASH块
    if (ulFlashBank == 0xFF)
    {
        return 0xFF;
    }
    
    //获取扇区
    int32_t ulStartSector = HW_FLASH_GetSector(ulFlashAddr);
    int32_t ulEndSector = HW_FLASH_GetSector(ulFlashAddr + ulByteSize -1);
    
    //扇区校验
    if ((ulStartSector == -1) || (ulEndSector == -1))
    {
        return 0xFF;
    }
    
    //禁止中断
    __disable_irq();
    
    //准备去擦除/写扇区
    for (int i = 0; i < FLASH_OPERA_RETRY_NUM; i++)
    {
        uRetCode = Chip_IAP_PreSectorForReadWrite(ulStartSector, ulEndSector, ulFlashBank);
        
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
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize - 1)))
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
    if ((HW_FLASH_CheckAddr(ulFlashAddr)) || (HW_FLASH_CheckAddr(ulFlashAddr + ulByteSize - 1)))
    {
        return 0;
    }
    
    switch (ulCheckSize)
    {
    case sizeof(uint8_t): 
        {
            uint8_t uCheckSum = 0;
            
            for (uint32_t i = 0; i < ulByteSize; i++)
            {
                uCheckSum += pFlashAddr[i];
            }
            
            return uCheckSum;
        }
    case sizeof(uint16_t):
        {
            uint16_t nCheckSum = 0;
            
            for (uint32_t i = 0; i < ulByteSize; i++)
            {
                nCheckSum += pFlashAddr[i];
            }
            
            return nCheckSum;
        }
    case sizeof(uint32_t):
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
