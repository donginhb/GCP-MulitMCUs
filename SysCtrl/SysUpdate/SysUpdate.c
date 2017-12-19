/**
  ******************************************************************************
  * @file    SysUpdate.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.19
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
#include "Bootloader.h"
#include "SysCtrl/SYS_MemoryDef.h"
#include "DataType/DataType.h"
#include "Algorithm/CRC/CRC.h"
#include "SysPeripheral/FLASH/Flash.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "HAL/HAL_Gpio.h"
#include "Version/Version.h"
#include <string.h>

      
/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * 系统Bootloader参数管理接口
 ****************************************************************************/

#define SYS_UPDATE_FLASH_OPERA_RETRY_NUM    (10)    //FLASH 操作失败重试次数
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
    
    uBit32 ulParmValidFlag;         //参数有效标志
    
    uBit32 ulLedPort;               //LED 端口
    uBit32 ulLedPin;                //LED 引脚
    
    uBit32 ulCrcValue;              //CRC16(考虑字节对齐,用32位变量存储)
    
}SYS_UAER_BOOTLOADER_DATE;          //用户Bootloader数据

static SYS_UAER_BOOTLOADER_DATE m_sys_BootloaderData = {0};         //系统Bootloader数据


/**
  * @brief  系统参数获取
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 SYS_UPDATE_GetSystemParm(void)
{
    uBit32 ulRet = 1;
    
    for (int i = 0; i < SYS_UPDATE_FLASH_OPERA_RETRY_NUM; i++)
    {
        //获取FLASH中的校验值
        FLASH_Read(FLASH_USER_START_ADDR, &m_sys_BootloaderData, sizeof(m_sys_BootloaderData));
        
        //计算实际校验值
        uBit32 ulCrcValue = CRC16_GetValue((uBit8 *)&m_sys_BootloaderData, sizeof(m_sys_BootloaderData) - sizeof(uBit32));
        
        //比较校验值
        if (ulCrcValue == m_sys_BootloaderData.ulCrcValue)
        {
            ulRet = 0;
            break;
        }
        
        //延时
        SysTime_DelayMs(50);
    }
    
    return ulRet;
}


/**
  * @brief  默认参数设置
  * @param  None
  * @retval None
  */
static void SYS_UPDATE_SetDefaultParm(void)
{
    //设置默认参数
    m_sys_BootloaderData.ulMainAppFlag           = 0;       //主APP存在标志
    m_sys_BootloaderData.ulSubAppFlag            = 0;       //副APP存在标志
    m_sys_BootloaderData.ulIgnoreSubAppFlag      = 1;       //忽略副APP标志
    m_sys_BootloaderData.ulMainAppFlashLen       = 0;       //主APP FLASH长度
    m_sys_BootloaderData.ulMainAppFlashCheckSum  = 0;       //主APP FALSH校验和
    m_sys_BootloaderData.ulSubAppFlashLen        = 0;       //副APP FLASH长度
    m_sys_BootloaderData.ulSubAppFlashCheckSum   = 0;       //副APP FALSH校验和
    m_sys_BootloaderData.ulParmValidFlag         = 1;       //参数有效标志
    m_sys_BootloaderData.ulLedPort               = 0xFFFF;  //LED 端口
    m_sys_BootloaderData.ulLedPin                = 0xFFFF;  //LED 引脚
    m_sys_BootloaderData.ulCrcValue              = 0;       //CRC16(考虑字节对齐,用32位变量存储)
    
}


/**
  * @brief  参数存储
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 SYS_UPDATE_StoreParmToFLash(void)
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


/*****************************************************************************
 * 系统Bootloader上电处理接口
 ****************************************************************************/

/**
  * @brief  应用程序有效性检测
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 SYS_UPDATE_CheckAppValidity(void)
{
    uBit32 ulRet = 1;
    
    do 
    {
        //判断参数是否有效
        if (m_sys_BootloaderData.ulParmValidFlag == 0) break;
        
        //校验APP FLASH区数据
        if (m_sys_BootloaderData.ulMainAppFlag)
        {
            uBit16 nAppCheckSum = FLASH_GetCheckSum(FLASH_APP_START_ADDR, m_sys_BootloaderData.ulMainAppFlashLen, sizeof(uBit16));
            
            if (nAppCheckSum != (m_sys_BootloaderData.ulMainAppFlashCheckSum & 0xFFFF)) 
            {
                ulRet = 1;
                break;
            }
        }
        else
        {
            ulRet = 1;
            break;
        }
        
        //校验副-APP FLASH区数据
        if (m_sys_BootloaderData.ulIgnoreSubAppFlag == 0)
        {
            if (m_sys_BootloaderData.ulSubAppFlag)
            {
                uBit16 nSubAppCheckSum = FLASH_GetCheckSum(FLASH_SUB_APP_BANK, m_sys_BootloaderData.ulSubAppFlashLen, sizeof(uBit16));
                
                if (nSubAppCheckSum != (m_sys_BootloaderData.ulSubAppFlashLen & 0xFFFF)) 
                {
                    ulRet = 1;
                    break;
                }
            }
            else 
            {
                ulRet = 1;
                break;
            }
        }
        
        //设置校验结果
        ulRet = 0;
    }while (0);
    
    return ulRet;
}


/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UPDATE_Init(void)
{
    //校验Bootloader 数据
    if (SYS_UPDATE_GetSystemParm())
    {
        //若校验失败,则设置默认参数并写入FLASH
        SYS_UPDATE_SetDefaultParm();
        SYS_UPDATE_StoreParmToFLash();
    }
    
    //初始化LED
    if (m_sys_BootloaderData.ulLedPort != 0xFFFF)
    {
        HAL_GPIO_ConfigOutput(m_sys_BootloaderData.ulLedPort, m_sys_BootloaderData.ulLedPin);
    }
    
    return 0;
}


/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 非0-失败
  * @note   本函数决定是跳转到APP还是执行Bootloader
  */
uBit32 SYS_UPDATE_JumToApp(void)
{
    //校验数据
    if (SYS_UPDATE_CheckAppValidity() == 0)
    {
        //若成功,则跳转到应用程序之中
        CoreCtrl_JumToApp();
    }
    
    return 0;
}




/*****************************************************************************
 * 系统更新相关管理接口
 ****************************************************************************/

#define SYS_UPDATE_PACK_INFO_INDEX              (0)                 //升级数据包第一包为信息帧,此帧数据包含了升级文件的信息
#define SYS_UPDATE_PACK_END_INDEX               (0xFFFF)            //升级数据包最后一包

#define SYS_UPDATE_ID_LENGTH                    (10)                //ID长度
#define SYS_UPDATE_RAND_DATA_LENGTH             (40)                //随机数据区长度


//数据包头定义
typedef struct
{
    uBit16 ulPackIndex;                                             //升级数据包序号
    uBit16 ulDataLen;                                               //升级数据长度
    
}SYS_UPDATE_PACK_HEAD, *P_SYS_UPDATE_PACK_HEAD;


//更新文件信息包
typedef struct _UPDATE_FILE_HEAD
{
    Bit8 cRandData[SYS_UPDATE_RAND_DATA_LENGTH];                    //随机数
    Bit8 cID[SYS_UPDATE_ID_LENGTH];                                 //升级文件ID
    uBit16 nPreLockCheckNum;                                        //加密前bin文件校验和
    //uBit16 nAftLockCheckNum;                                      //加密后bin文件校验和
    uBit32 nFileLenght;                                             //文件长度
    SOFTWARE_VERSION sVersion;                                      //版本信息
    
}SYS_UPDATE_FILE_INFO_PACK, *P_SYS_UPDATE_FILE_INFO_PACK;


/**
  * @brief  系统APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateAPP(uBit8 *pBuff, uBit32 ulSize)
{
    static uBit32 s_ulCurPackIndex = 0;         //当前升级数据包序号
    P_SYS_UPDATE_PACK_HEAD pPackHead = NULL;    //数据包头
    uBit8 *pDataPtr = NULL;                     //数据区指针
    uBit32 ulRet = BOOT_ERR_SUCCESS;            //执行结果
    
    //读取数据包头
    pPackHead = (P_SYS_UPDATE_PACK_HEAD)pBuff;
    pDataPtr  = pBuff + sizeof(SYS_UPDATE_PACK_HEAD);
    
    //确定是信息帧还是数据帧
    if (pPackHead->ulPackIndex == SYS_UPDATE_PACK_INFO_INDEX)   //信息帧
    {
        //数据校验
        if (pPackHead->ulDataLen != sizeof(SYS_UPDATE_FILE_INFO_PACK))
        {
            return BOOT_ERR_FILE_INFO;
        }
        
        //初始化相关参数
        s_ulCurPackIndex = SYS_UPDATE_PACK_INFO_INDEX;
        
        //获取文件信息
        uBit32 nBinFileCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
        uBit32 ulBinFileLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
        
        //配置文件信息
        ulRet = BOOT_InitFlash(FLASH_APP_BANK, nBinFileCheckSum, ulBinFileLen);
        
        //设置系统参数
        if (ulRet == BOOT_ERR_SUCCESS)
        {
            m_sys_BootloaderData.ulMainAppFlashCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
            m_sys_BootloaderData.ulMainAppFlashLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
        }
        
    }
    else    //数据帧
    {
        uBit32 ulFinshFlg = false;
        
        //判断是否最后一帧数据
        if (pPackHead->ulPackIndex != SYS_UPDATE_PACK_END_INDEX)
        {
            //更新当前数据包序号
            s_ulCurPackIndex++;
        
            //判断数据包是否连续
            if (pPackHead->ulPackIndex != s_ulCurPackIndex)
            {
                return BOOT_ERR_DATA_LOST;
            }
        }
        else 
        {
            ulFinshFlg = true;
        }
        
        //写入数据
        ulRet = BOOT_StoreUpdateDataToBuff(pDataPtr, pPackHead->ulDataLen, ulFinshFlg);
        
        //设置系统参数
        if ((ulRet == BOOT_ERR_SUCCESS) && (ulFinshFlg == true))
        {
            //校验flash数据
            ulRet = BOOT_CheckFlashLoaderFinsh();
            
            if (ulRet != BOOT_ERR_SUCCESS)
            {
                return BOOT_ERR_DATA_CHECK;
            }
            
            //设置相关标志位
            m_sys_BootloaderData.ulMainAppFlag = 1;
            
            //将数据写回FLASH
            ulRet = SYS_UPDATE_StoreParmToFLash();

            if (ulRet)
            {
                return BOOT_ERR_FLASH_OPS;
            }
            
            CoreCtrl_ResetSystemDelay(1000);
        }
        
    }
    
    return ulRet;
}


/**
  * @brief  系统副APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateSubAPP(uBit8 *pBuff, uBit32 ulSize)
{
    static uBit32 s_ulCurPackIndex = 0;         //当前升级数据包序号
    P_SYS_UPDATE_PACK_HEAD pPackHead = NULL;    //数据包头
    uBit8 *pDataPtr = NULL;                     //数据区指针
    uBit32 ulRet = BOOT_ERR_SUCCESS;            //执行结果
    
    //读取数据包头
    pPackHead = (P_SYS_UPDATE_PACK_HEAD)pBuff;
    pDataPtr  = pBuff + sizeof(SYS_UPDATE_PACK_HEAD);
    
    //确定是信息帧还是数据帧
    if (pPackHead->ulPackIndex == SYS_UPDATE_PACK_INFO_INDEX)   //信息帧
    {
        //数据校验
        if (pPackHead->ulDataLen != sizeof(SYS_UPDATE_FILE_INFO_PACK))
        {
            return BOOT_ERR_FILE_INFO;
        }
        
        //初始化相关参数
        s_ulCurPackIndex = SYS_UPDATE_PACK_INFO_INDEX;
        
        //获取文件信息
        uBit32 nBinFileCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
        uBit32 ulBinFileLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
        
        //配置文件信息
        ulRet = BOOT_InitFlash(FLASH_SUB_APP_BANK, nBinFileCheckSum, ulBinFileLen);
        
        //设置系统参数
        if (ulRet == BOOT_ERR_SUCCESS)
        {
            m_sys_BootloaderData.ulSubAppFlashCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
            m_sys_BootloaderData.ulSubAppFlashLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
        }
        
    }
    else    //数据帧
    {
        uBit32 ulFinshFlg = false;
        
        //判断是否最后一帧数据
        if (pPackHead->ulPackIndex != SYS_UPDATE_PACK_END_INDEX)
        {
            //更新当前数据包序号
            s_ulCurPackIndex++;
        
            //判断数据包是否连续
            if (pPackHead->ulPackIndex != s_ulCurPackIndex)
            {
                return BOOT_ERR_DATA_LOST;
            }
        }
        else 
        {
            ulFinshFlg = true;
        }
        
        //写入数据
        ulRet = BOOT_StoreUpdateDataToBuff(pDataPtr, pPackHead->ulDataLen, ulFinshFlg);
        
        //设置系统参数
        if ((ulRet == BOOT_ERR_SUCCESS) && (ulFinshFlg == true))
        {
            //校验flash数据
            ulRet = BOOT_CheckFlashLoaderFinsh();
            
            if (ulRet != BOOT_ERR_SUCCESS)
            {
                return BOOT_ERR_DATA_CHECK;
            }
            
            //设置相关标志位
            m_sys_BootloaderData.ulSubAppFlag = 1;
            
            //将数据写回FLASH
            ulRet = SYS_UPDATE_StoreParmToFLash();

            if (ulRet)
            {
                return BOOT_ERR_FLASH_OPS;
            }
            
        }
        
    }
    
    return ulRet;
}


/**
  * @brief  系统主APP存在标志清除
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_ClearAppFlag(void)
{
    if (m_sys_BootloaderData.ulMainAppFlag)
    {
        //清除主App存在标志
        m_sys_BootloaderData.ulMainAppFlag = 0;
        
        //存储参数
        SYS_UPDATE_StoreParmToFLash();
    }
    
    return 0;
}


/**
  * @brief  系统副APP存在标志清除
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_ClearSubAppFlag(void)
{
    if (m_sys_BootloaderData.ulSubAppFlag)
    {
        //清除副App存在标志
        m_sys_BootloaderData.ulSubAppFlag = 0;
        
        //存储参数
        SYS_UPDATE_StoreParmToFLash();
    }
    
    return 0;
}


/*****************************************************************************
 * Bootloader LED显示线程接口
 ****************************************************************************/
#define SYS_LED_TOGGLE_TIME          (500)      //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

/**
  * @brief  LED灯设置
  * @param  None
  * @retval None
  */
void SYS_UPDATE_SetMainWorkLed(uBit32 ulLedPort, uBit32 ulLedPin)
{
    if ((m_sys_BootloaderData.ulLedPort != ulLedPort) || 
        (m_sys_BootloaderData.ulLedPin != ulLedPin))
    {
        //设置LED端口号
        m_sys_BootloaderData.ulLedPort = ulLedPort;
        
        //设置LED引脚号
        m_sys_BootloaderData.ulLedPin = ulLedPin;
        
        //存储参数
        SYS_UPDATE_StoreParmToFLash();
    }
    
}


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void SYS_UPDATE_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        if (m_sys_BootloaderData.ulLedPort == 0xFFFF)
        {
            SysTime_Cancel(&m_LedCtrlTimer);
            return ;
        }
        
        SysTime_StartOneShot(&m_LedCtrlTimer, SYS_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        HAL_GPIO_SetOutputState(m_sys_BootloaderData.ulLedPort, 
                                m_sys_BootloaderData.ulLedPin, 
                                !HAL_GPIO_GetIOState(m_sys_BootloaderData.ulLedPort, 
                                                     m_sys_BootloaderData.ulLedPin));
    }

}

