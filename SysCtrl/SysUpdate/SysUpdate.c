/**
  ******************************************************************************
  * @file    SysUpdate.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.07
  * @brief   System update mamage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysUpdate.h"
#include "SysCtrl/SYS_MemoryDef.h"
#include "DataType/DataType.h"
#include "Algorithm/CRC/CRC.h"
#include "SysPeripheral/FLASH/Flash.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

#include <string.h>


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define SYS_UPDATE_FLASH_MIN_WRITE_SIZE     (512)   //FLASH 最小写入的数量

typedef struct
{
    //Bootloader 相关
    uBit32 ulMainAppFlag;           //主APP存在标志
    uBit32 ulSubAppFlag;            //副APP存在标志
    uBit32 ulIgnoreSubAppFlag;      //忽略副APP标志
    
    uBit32 ulMainAppFlashLen;       //主APP FLASH长度
    uBit32 ulMainAppFlashCheckSum;  //主APP FALSH校验和
    uBit32 ulSubAppFlashLen;        //副APP FLASH长度
    uBit32 ulSubAppFlashCheckSum;   //副APP FALSH校验和
    
    uBit32 ulCrcValue;              //CRC16(考虑字节对齐,用32位变量存储)
    
}SYS_UAER_BOOTLOADER_DATE;  //用户Bootloader数据


static SYS_UAER_BOOTLOADER_DATE m_sys_BootloaderData = {0};

/*****************************************************************************
 * 程序跳转相关控制接口
 ****************************************************************************/


//系统参数获取
//0-成功 1-失败
uBit32 SYS_UPDATE_GetSystemParm(void)
{
    //获取数据
    FLASH_Read(FLASH_USER_START_ADDR, &m_sys_BootloaderData, sizeof(m_sys_BootloaderData));
    
    //校验数据
    uBit32 ulCrcValue = CRC16_GetValue((uBit8 *)&m_sys_BootloaderData, sizeof(m_sys_BootloaderData) - sizeof(uBit32));
    
    if (ulCrcValue != m_sys_BootloaderData.ulCrcValue)
    {
        return 1;
    }
    
    return 0;
}


//默认参数设置
void SYS_UPDATE_SetDefaultParm(void)
{
    //设置默认参数
    m_sys_BootloaderData.ulMainAppFlag           = 0;   //主APP存在标志
    m_sys_BootloaderData.ulSubAppFlag            = 0;   //副APP存在标志
    m_sys_BootloaderData.ulIgnoreSubAppFlag      = 1;   //忽略副APP标志
    m_sys_BootloaderData.ulMainAppFlashLen       = 0;   //主APP FLASH长度
    m_sys_BootloaderData.ulMainAppFlashCheckSum  = 0;   //主APP FALSH校验和
    m_sys_BootloaderData.ulSubAppFlashLen        = 0;   //副APP FLASH长度
    m_sys_BootloaderData.ulSubAppFlashCheckSum   = 0;   //副APP FALSH校验和
    m_sys_BootloaderData.ulCrcValue = 0;                //CRC16(考虑字节对齐,用32位变量存储)
    
}

//参数存储
uBit32 SYS_UPDATE_StoreParmToFLash(void)
{
    uBit32 ulRet = 0;
    uBit8 uFlashBuff[SYS_UPDATE_FLASH_MIN_WRITE_SIZE] = {0};
    
    //计算CRC
    m_sys_BootloaderData.ulCrcValue = CRC16_GetValue((uBit8 *)&m_sys_BootloaderData, sizeof(m_sys_BootloaderData) - sizeof(uBit32));
    
    //存参数入BUFF
    memcpy(uFlashBuff, &m_sys_BootloaderData, sizeof(m_sys_BootloaderData));
    
    //擦除FLASH
    if (ulRet = FLASH_Erase(FLASH_USER_BANK, FLASH_USER_START_SECTOR, FLASH_USER_END_SECTOR)) 
    {
        return ulRet;
    }
    SysTime_DelayMs(100);
    
    //写BUFF入FLASH
    ulRet = FLASH_Write(FLASH_USER_START_ADDR, uFlashBuff, SYS_UPDATE_FLASH_MIN_WRITE_SIZE);
    SysTime_DelayMs(10);
    
    return ulRet;
}



//应用程序有效性检测
//0-成功 1-失败
uBit32 SYS_UPDATE_CheckAppValidity(void)
{
    //校验APP标志
    uBit32 ulRet = 0;
    
    do 
    {
        //校验主APP存在标志
        if (!m_sys_BootloaderData.ulMainAppFlag)
        {
            ulRet = 1;
            break;
        }
        
    }while (0);
    
    return ulRet;
}


/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 1-失败
  * @note   本函数决定是跳转到APP还是执行Bootloader
  */
uBit32 SYS_UPDATE_Init(void)
{
    //校验Bootloader 数据
    if (SYS_UPDATE_GetSystemParm())
    {
        //若校验失败,则设置默认参数并写入FLASH
        SYS_UPDATE_SetDefaultParm();
        SYS_UPDATE_StoreParmToFLash();
        
        return 0;
    }

    //校验数据
    if (SYS_UPDATE_CheckAppValidity() == 0)
    {
        //若成功,则跳转到应用程序之中
        CoreCtrl_JumToApp();
    }
    
    return 0;
}










