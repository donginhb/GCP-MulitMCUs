/**
  ******************************************************************************
  * @file    Bootloader.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.07
  * @brief   Bootloader Application
  ******************************************************************************
  * @attention
  * 
  * 基本思路:
  * 
  * 1. 定义一个数据缓冲区,将每次接收到的零散的数据存入缓冲区之中;
  * 
  * 2. 当缓冲区满之后,将其写入FLASH之中;
  * 
  * 3. 重复[1-2],直到数据数据全部写入FLASH;(需兼容最后一包数据但缓冲区未满的情况)
  * 
  * 扩展思路:
  * 
  * 1. 在升级之前,需要知道bin文件的长度和文件校验和;通过文件长度计算FLASH的结束
  *    扇区,擦除起始扇区到结束扇区之间的所有FLASH;
  * 
  * 2. 升级之后,需要将整个FLASH的数据读取出来,并进行校验,以确定烧录是否成功;
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Bootloader.h"
#include "DataType/DataType.h"
#include "SysCtrl/SYS_MemoryDef.h"
#include "SysPeriPheral/FLASH/Flash.h"

#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define UPDATE_DATA_BUFF_SIZE       (1024)                      //更新数据缓冲区大小
#define UPDATE_DATA_BUFF_MASK       (UPDATE_DATA_BUFF_SIZE - 1) //更新数据缓存区大小掩码

static uBit32 m_ulFlashBank = 0;                                //FLASH块定义

static uBit16 m_nBinFileCheckSum = 0;                           //bin文件校验和
static uBit32 m_ulBinFileLen = 0;                               //bin文件长度
static uBit32 m_ulCurFileLen = 0;                               //当前文件长度

static uBit8  m_uUpdateDataBuff[UPDATE_DATA_BUFF_SIZE] = {0};   //升级数据缓存

static uBit32 m_ulBuffIndex = 0;                                //缓存区位置索引
static uBit32 m_ulBaseFlashAddr = 0;                            //FLASH基地址
static uBit32 m_ulFlashIndex = 0;                               //当前FLASH烧录位置索引(相对于基地址)


/**
  * @brief  Bootloader 数据初始化
  * @param  None
  * @retval None
  */
static void BOOT_InitData(void)
{
    //清空文件校验信息
    m_nBinFileCheckSum = 0;
    m_ulBinFileLen = 0;
    m_ulCurFileLen = 0;
    
    //清空FLASH烧录信息
    m_ulFlashBank = 0;
    m_ulBaseFlashAddr = 0;
    m_ulBuffIndex = 0;
    m_ulFlashIndex = 0; 
    
    //清空数据缓冲区
    memset(m_uUpdateDataBuff, 0, UPDATE_DATA_BUFF_SIZE);
    
}


/**
  * @brief  BIN 文件参数设置
  * @param  nBinFileCheckSum BIN文件校验和
  * @param  ulBinFileLen  BIN文件长度
  * @retval 0-成功 非0-失败
  */
static uBit32 BOOT_SetBinFileParm(uBit16 nBinFileCheckSum, uBit32 ulBinFileLen)
{
    if (ulBinFileLen == 0)
    {
        return BOOT_ERR_FILE_INFO;
    }
    
    m_nBinFileCheckSum = nBinFileCheckSum;  //文件校验和
    m_ulBinFileLen = ulBinFileLen;          //文件长度
    
    return BOOT_ERR_SUCCESS;
}


/**
  * @brief  flash基地址设置(以兼容多块FLASH区烧录的情况)
  * @param  ulFlashBank FLASH块
  * @retval 0-成功 非0-失败
  */
static uBit32 BOOT_SetBaseFlashAddr(uBit32 ulFlashBank)
{
    switch (ulFlashBank)
    {
    case FLASH_BOOT_BANK:
        {
            m_ulFlashBank = ulFlashBank;
            m_ulBaseFlashAddr = FLASH_BOOT_START_ADDR;
            break;
        }
    case FLASH_APP_BANK:
        {
            m_ulFlashBank = ulFlashBank;
            m_ulBaseFlashAddr = FLASH_APP_START_ADDR;
            break;
        }
    case FLASH_SUB_APP_BANK:
        {
            m_ulFlashBank = ulFlashBank;
            m_ulBaseFlashAddr = FLASH_SUB_APP_START_ADDR;
            break;
        }
    default: 
        {
            m_ulFlashBank = ulFlashBank;
            m_ulBaseFlashAddr = FLASH_APP_START_ADDR;
            return BOOT_ERR_FLASH_ADDR;
        }
    }
    
    return BOOT_ERR_SUCCESS;
}


/**
  * @brief  FLASH区擦除
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 BOOT_EraseAppFlash(void)
{
    uBit32 ulStartSector = 0;   //FLASH起始扇区
    uBit32 ulEndSector = 0;     //FLASH结束扇区
    uBit32 ulRet = 0;
    
    //计算扇区
    ulStartSector = FLASH_GetSector(m_ulBaseFlashAddr);                //起始扇区
    ulEndSector = FLASH_GetSector(m_ulBaseFlashAddr + m_ulBinFileLen); //结束扇区
    
    //校验数据
    if ((ulStartSector == 0xFF) || (ulEndSector == 0xFF))
    {
        return BOOT_ERR_FLASH_ADDR;
    }
    
    //擦除扇区(只擦除用上的部分)
    ulRet = FLASH_Erase(m_ulFlashBank, ulStartSector, ulEndSector);
    
    if (ulRet)
    {
        return BOOT_ERR_FLASH_OPS;
    }
    
    return BOOT_ERR_SUCCESS;
}


/**
  * @brief  将缓冲区数据写入FALSH之中
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 BOOT_WriteBuffToFlah(void)
{
    //校验缓冲区长度
    if (m_ulBuffIndex == 0)
    {
        return BOOT_ERR_PACK_SIZE;
    }
    
    //若缓冲区满,将数据写入FLASH之中
    if (FLASH_Write(m_ulBaseFlashAddr + m_ulFlashIndex, m_uUpdateDataBuff, UPDATE_DATA_BUFF_SIZE))
    {
        return BOOT_ERR_FLASH_OPS;   //(写入失败)
    }
    
    //写入成功,则进行数据更新
    m_ulBuffIndex = 0;                                      //清空Buff索引
    m_ulFlashIndex += UPDATE_DATA_BUFF_SIZE;                //更新Flash地址索引
    memset(m_uUpdateDataBuff, 0, UPDATE_DATA_BUFF_SIZE);    //清空Buff
    
    return BOOT_ERR_SUCCESS;
}


/*****************************************************************************
 * BOOTLOADER相关控制接口
 ****************************************************************************/

/**
  * @brief  Bootloader FLASH初始化
  * @param  ulFlashBank 目标FALSH块
  * @param  nBinFileCheckSum BIN文件校验和
  * @param  ulBinFileLen  BIN文件长度
  * @retval 0-成功 非0-失败
  * @note   执行此函数,会清除掉目标FLASH里面的数据,并中止当前的loader(若存在)
  */
uBit32 BOOT_InitFlash(uBit32 ulFlashBank, uBit16 nBinFileCheckSum, uBit32 ulBinFileLen)
{
    uBit32 ulRet = BOOT_ERR_SUCCESS;
    
    do 
    {
        //初始化数据
        BOOT_InitData();
        
        //设置BIN 文件参数
        if (ulRet = BOOT_SetBinFileParm(nBinFileCheckSum, ulBinFileLen)) break;
        
        //设置基地址
        if (ulRet = BOOT_SetBaseFlashAddr(ulFlashBank)) break;
        
        //擦除应用程序区FLASH
        if (ulRet = BOOT_EraseAppFlash()) break;
        
    }while (0);
    
    return ulRet;
}


/**
  * @brief  升级数据存储
  * @param  pBuff 升级数据
  * @param  ulSize 数据长度 
  * @param  ulFinshFlg 最后一帧数据标志
  * @retval 0-成功 非0-失败
  * @note   若ulFinshFlg 为1, 则会立即将缓冲区的数据写入FLASH之中(不管缓冲区是否
  *         满,未使用的部分用0填充)
  */
uBit32 BOOT_StoreUpdateDataToBuff(uBit8 *pBuff, uBit32 ulSize, uBit32 ulFinshFlg)
{
    //校验数据长度(除非是最后一包数据,否则一次写入的长度必须是2的幂)
    if ((ulSize & (ulSize-1)) && (!ulFinshFlg))
    {
        return BOOT_ERR_PACK_SIZE;
    }
    
    //判断是否会溢出
    if ((m_ulBuffIndex + ulSize) > UPDATE_DATA_BUFF_SIZE)
    {
        return BOOT_ERR_PACK_SIZE;
    }
    
    //复制数据到缓冲区
    memcpy(&m_uUpdateDataBuff[m_ulBuffIndex], pBuff, ulSize);
    m_ulBuffIndex += ulSize;
    
    //更新文件长度
    m_ulCurFileLen += ulSize;
    
    //判断是否最后一包
    if (ulFinshFlg)
    {
        return BOOT_WriteBuffToFlah();
    }
    
    //判断缓冲区是否满了
    if (m_ulBuffIndex == UPDATE_DATA_BUFF_SIZE)
    {
        return BOOT_WriteBuffToFlah();
    }
    
    return BOOT_ERR_SUCCESS;
}


/**
  * @brief  FLASH烧录完成校验
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 BOOT_CheckFlashLoaderFinsh(void)
{
    uBit16 nCurFileCheckSum = 0;    //当前文件校验和
    
    //若实际烧录的文件长度跟Bin文件信息中的不一致,报错
    if (m_ulCurFileLen != m_ulBinFileLen)
    {
        return BOOT_ERR_DATA_CHECK;
    }
    
    //校验整个FLASH
    nCurFileCheckSum = FLASH_GetCheckSum(m_ulBaseFlashAddr, m_ulBinFileLen, sizeof(m_nBinFileCheckSum));

    if (nCurFileCheckSum != m_nBinFileCheckSum)
    {
        return BOOT_ERR_DATA_CHECK;
    }
    
    return BOOT_ERR_SUCCESS;
}

