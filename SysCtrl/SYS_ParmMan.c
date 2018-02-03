/**
  ******************************************************************************
  * @file    SYS_ParmMan.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.18
  * @brief   System parameter manage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SYS_ParmMan.h"
#include "SYS_MemoryDef.h"
#include "SysConfig.h"
#include "SysUpdate/SysUpdate.h"
#include "DataType/DataType.h"

#include "SysPeripheral/FLASH/Flash.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "Algorithm/CRC/CRC.h"

#include "Version/BootVersion.h"

#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define SYS_UPDATE_FLASH_OPERA_RETRY_NUM            (10)    //FLASH 操作失败重试次数
#define SYS_UPDATE_FLASH_MIN_WRITE_SIZE             (512)   //FLASH 最小写入的数量

#define COM_TYPE_NULL                               (0)     //没有连接
#define COM_TYPE_CAN                                (1)     //CAN 通信模式
#define COM_TYPE_ENET                               (2)     //ENET通信模式
#define COM_TYPE_UART                               (4)     //UART通信模式
     
#if SYS_PARM_MAN_USAGE
static bool m_sys_bBootParmValid = false;
static bool m_sys_bCmuParmValid = false;

static SYS_USER_PARM m_sys_ConfigParm = {0};
#endif


#if SYS_PARM_MAN_USAGE
/**
  * @brief  系统参数获取
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 SYS_GetSystemParm(void)
{
    uBit32 ulRet = 1;

    for (int i = 0; i < SYS_UPDATE_FLASH_OPERA_RETRY_NUM; i++)
    {
        //复位参数有效标志
        m_sys_bBootParmValid = false;
        m_sys_bCmuParmValid = false;
        
        //获取FLASH中的校验值
        FLASH_Read(FLASH_USER_START_ADDR, &m_sys_ConfigParm, sizeof(m_sys_ConfigParm));
        
        //计算实际校验值
        uBit32 ulBootloaderCrcValue = CRC16_GetValue((uBit8 *)&m_sys_ConfigParm.BootloaderData, sizeof(m_sys_ConfigParm.BootloaderData) - sizeof(uBit32));
        uBit32 ulCmuCrcValue = CRC16_GetValue((uBit8 *)&m_sys_ConfigParm.CmuData, sizeof(m_sys_ConfigParm.CmuData) - sizeof(uBit32));
        
        //比较校验值
        if (ulBootloaderCrcValue == m_sys_ConfigParm.BootloaderData.ulCrcValue)
        {
            m_sys_bBootParmValid = true;
        }
        if (ulCmuCrcValue == m_sys_ConfigParm.CmuData.ulCrcValue)
        {
            m_sys_bCmuParmValid = true;
        }
        
        if (m_sys_bBootParmValid && m_sys_bCmuParmValid)
        {
            ulRet = 0;
            break;
        }
        
        //延时
        SysTime_DelayMs(50);
    }

    return ulRet;
}
#endif


/*****************************************************************************
 * SYS参数管理相关控制接口
 ****************************************************************************/

/**
  * @brief  系统参数初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitSysParm(void)
{
#if SYS_PARM_MAN_USAGE
    if (SYS_GetSystemParm())
    {
#if SYS_BOOT_USAGE
        SYS_SetDefaultParm();
        SYS_StoreParmToFLash();
#endif
    }
#endif
    
    return 0;
}


/**
  * @brief  默认参数设置
  * @param  None
  * @retval None
  */
void SYS_SetDefaultParm(void)
{
#if SYS_PARM_MAN_USAGE
    //设置Bootloader默认参数
    m_sys_ConfigParm.BootloaderData.ulMainAppFlag           = 0;        //主APP存在标志
    m_sys_ConfigParm.BootloaderData.ulSubAppFlag            = 0;        //副APP存在标志
    m_sys_ConfigParm.BootloaderData.ulIgnoreSubAppFlag      = 1;        //忽略副APP标志
    m_sys_ConfigParm.BootloaderData.ulMainAppFlashLen       = 0;        //主APP FLASH长度
    m_sys_ConfigParm.BootloaderData.ulMainAppFlashCheckSum  = 0;        //主APP FALSH校验和
    m_sys_ConfigParm.BootloaderData.ulSubAppFlashLen        = 0;        //副APP FLASH长度
    m_sys_ConfigParm.BootloaderData.ulSubAppFlashCheckSum   = 0;        //副APP FALSH校验和
    m_sys_ConfigParm.BootloaderData.ulCrcValue = 0;
    
    //设置Bootloader版本参数
    memcpy(&m_sys_ConfigParm.BootloaderData.BootloaderVersion, BT_VER_STR, sizeof(SOFTWARE_VERSION));
    m_sys_ConfigParm.BootloaderData.BootloaderVersion.cSoftVer[0] = BT_VER_NO0;
    m_sys_ConfigParm.BootloaderData.BootloaderVersion.cSoftVer[1] = BT_VER_NO1;
    m_sys_ConfigParm.BootloaderData.BootloaderVersion.cPart = BT_VER_PART;
    m_sys_ConfigParm.BootloaderData.BootloaderVersion.cFlag = BT_VER_FLAG;
    
    //设置Cmu默认参数
    m_sys_ConfigParm.CmuData.ulComMode = COM_TYPE_UART;                 //通信类型
    m_sys_ConfigParm.CmuData.ComParm.CmuCanUartParm.ulComNode = SYS_CMU_DEF_UART_NODE;      //通信节点
    m_sys_ConfigParm.CmuData.ComParm.CmuCanUartParm.ulBaudRate = SYS_CMU_DEF_UART_BAUDRATE; //通信波特率
    m_sys_ConfigParm.CmuData.ulCrcValue = 0;
    
#endif
    
}


/**
  * @brief  参数存储
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_StoreParmToFLash(void)
{
    uBit32 ulRet = 0;
    
#if SYS_PARM_MAN_USAGE
    uBit32 ulWriteCount = 0;    //写入次数
    
    uBit8 uFlashBuff[SYS_UPDATE_FLASH_MIN_WRITE_SIZE] = {0};
    
    //计算CRC
    m_sys_ConfigParm.BootloaderData.ulCrcValue = CRC16_GetValue((uBit8 *)&m_sys_ConfigParm.BootloaderData, sizeof(m_sys_ConfigParm.BootloaderData) - sizeof(uBit32));
    m_sys_ConfigParm.CmuData.ulCrcValue = CRC16_GetValue((uBit8 *)&m_sys_ConfigParm.CmuData, sizeof(m_sys_ConfigParm.CmuData) - sizeof(uBit32));
    
    //擦除FLASH
    if (ulRet = FLASH_Erase(FLASH_USER_BANK, FLASH_USER_START_SECTOR, FLASH_USER_END_SECTOR)) 
    {
        return ulRet;
    }
    SysTime_DelayMs(100);
    
    //存储参数
    ulWriteCount = ((sizeof(m_sys_ConfigParm) - 1) / SYS_UPDATE_FLASH_MIN_WRITE_SIZE) + 1;  //计算要写入的次数
    
    for (int i = 0; i < ulWriteCount; i++)
    {
        //获取本次烧录要写入的参数地址
        uBit8 *pParmStartAddr = (uBit8 *)(((uBit8 *)&m_sys_ConfigParm) + i*SYS_UPDATE_FLASH_MIN_WRITE_SIZE);
        
        //计算当前要存储的数据长度
        uBit32 ulWriteLenght = SYS_UPDATE_FLASH_MIN_WRITE_SIZE;   //本次写入的字节长度
        
        if (i == (ulWriteCount - 1))
        {
            ulWriteLenght = sizeof(m_sys_ConfigParm) - i*SYS_UPDATE_FLASH_MIN_WRITE_SIZE;
        }
        
        //存储参数入Buff之中
        memcpy(uFlashBuff, pParmStartAddr, ulWriteLenght);
        
        //写BUFF入FLASH
        ulRet = FLASH_Write(FLASH_USER_START_ADDR + i*SYS_UPDATE_FLASH_MIN_WRITE_SIZE, uFlashBuff, SYS_UPDATE_FLASH_MIN_WRITE_SIZE);
        SysTime_DelayMs(10);
    }

    if (ulRet == 0)
    {
        m_sys_bBootParmValid = 1;
        m_sys_bCmuParmValid = 1;
    }
    
#endif
    
    return ulRet;
}


/**
  * @brief  系统升级数据获取
  * @param  None
  * @retval 系统升级数据地址
  */
SYS_USER_BOOTLOADER_DATE *SYS_GetSysUpadateParm(void)
{
#if SYS_PARM_MAN_USAGE
    
    if (m_sys_bBootParmValid)
    {
        return &m_sys_ConfigParm.BootloaderData;
    }

#endif
    
    return NULL;
}


/**
  * @brief  CMU配置参数获取
  * @param  None
  * @retval CMU配置参数地址
  */
SYS_USER_CMU_DATA *SYS_GetCmuConfigParm(void)
{
#if SYS_PARM_MAN_USAGE
    
    if (m_sys_bCmuParmValid)
    {
        return &m_sys_ConfigParm.CmuData;
    }
    
#endif
    
    return NULL;
}

