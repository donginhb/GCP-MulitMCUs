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
#include "SysCtrl/SYS_ParmMan.h"
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
static SYS_USER_BOOTLOADER_DATE *m_sys_pUpdateData = NULL;      //系统升级相关数据
static SYS_USER_IO_CFG_DATA     *m_sys_pIOConfigTable = NULL;   //IO配置表相关数据

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
        if (m_sys_pUpdateData->ulParmValidFlag == 0) break;
        
        //校验APP FLASH区数据
        if (m_sys_pUpdateData->ulMainAppFlag)
        {
            uBit16 nAppCheckSum = FLASH_GetCheckSum(FLASH_APP_START_ADDR, m_sys_pUpdateData->ulMainAppFlashLen, sizeof(uBit16));
            
            if (nAppCheckSum != (m_sys_pUpdateData->ulMainAppFlashCheckSum & 0xFFFF)) 
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
        if (m_sys_pUpdateData->ulIgnoreSubAppFlag == 0)
        {
            if (m_sys_pUpdateData->ulSubAppFlag)
            {
                uBit16 nSubAppCheckSum = FLASH_GetCheckSum(FLASH_SUB_APP_BANK, m_sys_pUpdateData->ulSubAppFlashLen, sizeof(uBit16));
                
                if (nSubAppCheckSum != (m_sys_pUpdateData->ulSubAppFlashLen & 0xFFFF)) 
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
    m_sys_pIOConfigTable = SYS_GetIOConfigTable();      //获取IO配置表
    m_sys_pUpdateData = SYS_GetSysUpadateParm();        //获取系统升级参数
    
    //判断系统参数
    if ((m_sys_pIOConfigTable == NULL) || (m_sys_pUpdateData == NULL))
    {
        return 1;
    }
    
    //初始化LED
    if (m_sys_pIOConfigTable->ulLedEnable)
    {
        HAL_GPIO_ConfigOutput(m_sys_pIOConfigTable->LedIO.nPort, m_sys_pIOConfigTable->LedIO.nPin);
    }
    
    //初始化配置表内的IO
    for (int i = 0; i < m_sys_pIOConfigTable->ulActiveIOCount; i++)
    {
        HAL_GPIO_ConfigOutput(m_sys_pIOConfigTable->ActiveOutputIO[i].nPort, 
                              m_sys_pIOConfigTable->ActiveOutputIO[i].nPin);
        
        HAL_GPIO_SetOutputState(m_sys_pIOConfigTable->ActiveOutputIO[i].nPort,
                                m_sys_pIOConfigTable->ActiveOutputIO[i].nPin,
                                true);
    }
    
    for (int i = 0; i < m_sys_pIOConfigTable->ulPassiveIOCount; i++)
    {
        HAL_GPIO_ConfigOutput(m_sys_pIOConfigTable->PassiveOutputIO[i].nPort, 
                              m_sys_pIOConfigTable->PassiveOutputIO[i].nPin);
        
        HAL_GPIO_SetOutputState(m_sys_pIOConfigTable->PassiveOutputIO[i].nPort,
                                m_sys_pIOConfigTable->PassiveOutputIO[i].nPin,
                                false);
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
            m_sys_pUpdateData->ulMainAppFlashCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
            m_sys_pUpdateData->ulMainAppFlashLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
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
            m_sys_pUpdateData->ulMainAppFlag = 1;
            
            //将数据写回FLASH
            ulRet = SYS_StoreParmToFLash();

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
            m_sys_pUpdateData->ulSubAppFlashCheckSum = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nPreLockCheckNum;
            m_sys_pUpdateData->ulSubAppFlashLen = ((P_SYS_UPDATE_FILE_INFO_PACK)pDataPtr)->nFileLenght;
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
            m_sys_pUpdateData->ulSubAppFlag = 1;
            
            //将数据写回FLASH
            ulRet = SYS_StoreParmToFLash();

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
    if (m_sys_pUpdateData->ulMainAppFlag)
    {
        //清除主App存在标志
        m_sys_pUpdateData->ulMainAppFlag = 0;
        
        //存储参数
        SYS_StoreParmToFLash();
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
    if (m_sys_pUpdateData->ulSubAppFlag)
    {
        //清除副App存在标志
        m_sys_pUpdateData->ulSubAppFlag = 0;
        
        //存储参数
        SYS_StoreParmToFLash();
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
    if (m_sys_pIOConfigTable->ulLedEnable)
    {
        //假如FLASH之中的参数与当前要设置的LED IO不一致,则更新并存储
        if ((m_sys_pIOConfigTable->LedIO.nPort != ulLedPort) ||
            (m_sys_pIOConfigTable->LedIO.nPin != ulLedPin))
        {
            //更新参数
            m_sys_pIOConfigTable->LedIO.nPort = ulLedPort;
            m_sys_pIOConfigTable->LedIO.nPin  = ulLedPin;
            
            //存储参数
            SYS_StoreParmToFLash();
        }
    }
    else 
    {
        //更新参数
        m_sys_pIOConfigTable->LedIO.nPort = ulLedPort;
        m_sys_pIOConfigTable->LedIO.nPin  = ulLedPin;
        m_sys_pIOConfigTable->ulLedEnable = 1;
        
        //存储参数
        SYS_StoreParmToFLash();
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
        if (m_sys_pIOConfigTable->ulLedEnable == 0)
        {
            SysTime_Cancel(&m_LedCtrlTimer);
            return ;
        }
        
        SysTime_StartOneShot(&m_LedCtrlTimer, SYS_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        HAL_GPIO_SetOutputState(m_sys_pIOConfigTable->LedIO.nPort, 
                                m_sys_pIOConfigTable->LedIO.nPin, 
                                !HAL_GPIO_GetIOState(m_sys_pIOConfigTable->LedIO.nPort, 
                                                     m_sys_pIOConfigTable->LedIO.nPin));
    }

}

