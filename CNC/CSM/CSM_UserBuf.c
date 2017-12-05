
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_UserBuf.c

摘    要：用户自定义数据缓冲区管理模块

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月14日
*/

#include "string.h"
#include "CSM_Interface.h"
#include "CSM_CrdSysState.h"
#include "CSM_MainCtrl.h"
#include "CSM_UserBuf.h"
#include "CSM_ParmMan.h"

#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"
#include "CNC/ErrorCodeDef/CSM_ErrorCodeDef.h"

//
extern CSM_INTERFACE    m_CSMInterface;

static DATA_BUF_MAN        m_UserBufManData[CRDSYS_MAX_COUNT];
static unsigned long    m_ulUserBufAddr = 0;
static unsigned long    m_ulUserBufLen = 0;

static unsigned short   m_nUserBufCtrlMode[CRDSYS_MAX_COUNT];        //用户缓冲区控制模式，取值为：0-通道控制，1-解释器控制


//设置用户自定义数据缓冲区地址
void CSM_SetUserBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen)
{
    m_ulUserBufAddr = ulStartAddr;
    m_ulUserBufLen = ulBufLen;
}


/*
函 数 名：void IPO_InitIPOBuf()
功    能：初始化插补缓冲区
参    数：无
返 回 值：无
调用位置：（通道初始化时调用，坐标系设置改变时调用）,
注意事项：无
*/
void CSM_InitUserBuf()
{
    int i;
    int iCrdSysCount;
    long ulBufSize;

    //
    if (m_ulUserBufLen == 0 || m_ulUserBufAddr == 0)
    {
        for (i = 0; i < CRDSYS_MAX_COUNT; i++)
        {
            CSM_SetCrdSysAlarmBit(i, CRDSYS_ALARM_CSM_INIT_ERROR);
        }

        return;
    }

    //
    iCrdSysCount = CSM_GetCrdSysCount();

    ulBufSize = m_ulUserBufLen / iCrdSysCount;

    //（内存地址对齐处理）
    ulBufSize = ulBufSize - ulBufSize % 4;
    if (ulBufSize < 0)
    {
        ulBufSize = 0;
    }

    for (i = 0; i < iCrdSysCount; i++)
    {
        m_UserBufManData[i].ulStartAdr = m_ulUserBufAddr + i*ulBufSize;
        m_UserBufManData[i].ulSize = ulBufSize;
        m_UserBufManData[i].ulTotalSegmentCount = ulBufSize;

        m_UserBufManData[i].ulSegmentCount = 0;
        m_UserBufManData[i].ulReadPos = 0;
        m_UserBufManData[i].ulWritePos = 0;
    }    

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        m_UserBufManData[i].ulStatus = DATA_BUF_STATUS_STATIC | DATA_BUF_STATUS_EMPTY;

        m_nUserBufCtrlMode[i] = 0;
    }
}

//设置设置用户自定义数据缓冲区工作模式，iMode： 1-静态模式，0-动态模式
uBit32 CSM_SetUserBufMode(Bit32 iCrdSysIndex, Bit32 iMode)
{
    uBit32 ulStopBit;
    uBit32 ulErrorCode;

    ulStopBit = m_CrdSysStateData[iCrdSysIndex].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL);

    if (!ulStopBit)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_PROG_RUNNING, 
            CSM_CMD_SET_USER_BUF_MODE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    CSM_ClearUserData(iCrdSysIndex);

    //
    if (iMode)
    {
        m_UserBufManData[iCrdSysIndex].ulStatus |= DATA_BUF_STATUS_STATIC;
    }
    else
    {
        m_UserBufManData[iCrdSysIndex].ulStatus &= ~DATA_BUF_STATUS_STATIC;
    }

    return 0;
}

void CSM_OpenUserBuf(Bit32 iCrdSysIndex)
{
    m_UserBufManData[iCrdSysIndex].ulStatus |= DATA_BUF_STATUS_OPEN;
}


void CSM_CloseUserBuf(Bit32 iCrdSysIndex)
{
    m_UserBufManData[iCrdSysIndex].ulStatus &= ~DATA_BUF_STATUS_OPEN;
}

/*
函 数 名：void CSM_ClearProgData(Bit32 iCrdSysIndex, Bit32 iClearMode)
功    能：清除程序数据
参    数：iCrdSysIndex --坐标系索引
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
uBit32 CSM_ClearUserData(Bit32 iCrdSysIndex)
{
    DATA_BUF_MAN* pBufManData = NULL;

    //
    pBufManData = &(m_UserBufManData[iCrdSysIndex]);

    pBufManData->ulSegmentCount = 0;
    pBufManData->ulReadPos = 0;
    pBufManData->ulWritePos = 0;

    pBufManData->ulStatus &= ~DATA_BUF_STATUS_USESTATUSMASK;
    pBufManData->ulStatus |= DATA_BUF_STATUS_EMPTY;

    return 0;
}

/*
函 数 名：Bit32 CSM_SetUserDataBufCtrlMode(Bit32 iCrdSysIndex, Bit32 iMode, DATA_BUF_MAN** ppDataBufMan)
功    能：设置用户自定义数据缓冲区控制模式
参    数：iCrdSysIndex --坐标系索引
          iMode --用户自定义数据缓冲区控制模式，取值：0-用户自定义数据缓冲区由通道管理模块控制，1-用户自定义数据缓冲区由解释器控制
          *ppDataBufMan --用户自定义数据缓冲区管理数据地址（出参）
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
Bit32 CSM_SetUserDataBufCtrlMode(Bit32 iCrdSysIndex, Bit32 iMode, DATA_BUF_MAN** ppDataBufMan)
{
    if (iMode == m_nUserBufCtrlMode[iCrdSysIndex])
    {
        m_UserBufManData[iCrdSysIndex].ulStatus &= ~DATA_BUF_STATUS_STATIC;  //设为动态模式
        *ppDataBufMan = m_UserBufManData + iCrdSysIndex;
        return 0;
    }

    CSM_ClearUserData(iCrdSysIndex);

    //
    if (iMode == 1)
    { //用户自定义数据缓冲区由解释器控制
        m_nUserBufCtrlMode[iCrdSysIndex] = 1;
        CSM_SetProgBufMode(iCrdSysIndex, 0);   //将程序缓冲区设为动态模式
        *ppDataBufMan = m_UserBufManData + iCrdSysIndex;
    }
    else
    {
        m_nUserBufCtrlMode[iCrdSysIndex] = 0;
    }
    
    return 0;
}

//(解释器专用)iMode=0插补器控制用户缓冲区，iMode=1解释器控制用户缓冲区
Bit32 CSM_SetUserDataBufCtrlMode_IPR(Bit32 iMode)
{
    int iCrdSysIndex;
    int iCrdSysCount;

    iCrdSysCount = CSM_GetCrdSysCount();

    for (iCrdSysIndex = 0; iCrdSysIndex < iCrdSysCount; iCrdSysIndex++)
    {
        CSM_ClearUserData(iCrdSysIndex);

        //
        if (iMode == 1)
        { //用户自定义数据缓冲区由解释器控制
            m_nUserBufCtrlMode[iCrdSysIndex] = 1;
            CSM_SetProgBufMode(iCrdSysIndex, 0);   //将程序缓冲区设为动态模式
        }
        else
        {
            m_nUserBufCtrlMode[iCrdSysIndex] = 0;
        }
    }
/*
    *pulStartAddr = m_ulUserBufAddr;
    *pulSize = m_ulUserBufLen;
    */
    return 0;
}


//
void CSM_UpdateUserBufUseStatus(int iCrdSysIndex)
{
    DATA_BUF_MAN* pBufManData = NULL;

    //
    pBufManData = &(m_UserBufManData[iCrdSysIndex]);

    pBufManData->ulStatus &= ~DATA_BUF_STATUS_USESTATUSMASK;

    if (pBufManData->ulSegmentCount == 0)
    {
        pBufManData->ulStatus |= DATA_BUF_STATUS_EMPTY;
    }
    else if (pBufManData->ulSegmentCount == pBufManData->ulTotalSegmentCount)
    {
        pBufManData->ulStatus |= DATA_BUF_STATUS_FULL;
    }
    else
    {
        pBufManData->ulStatus |= DATA_BUF_STATUS_HALF;
    }
}

/*
函 数 名：int IPO_AddMotionDataToIPOBuf(int iCrdSysIndex, SEGMENT_MOTION_DATA* pMotionData)
功    能：添加运动段数据到插补缓冲区
参    数：iCrdSysIndex——坐标系索引，取值0-1
          *pMotionData——运动段数据
返 回 值：成功返回0，否则返回错误码
调用位置：应用层
注意事项：
*/
uBit32 CSM_AddUserDataToBuf(Bit32 iCrdSysIndex, Bit8* pUserData, Bit32 iDataLen)
{
    int iLengthTemp1;
    int iLengthTemp2;
    uBit32 ulErrorCode;
    DATA_BUF_MAN* pBufManData = NULL;
    Bit8* pDataStartAddr = NULL;

    pBufManData = &(m_UserBufManData[iCrdSysIndex]);

    //
    if (pBufManData->ulSegmentCount + iDataLen >= pBufManData->ulTotalSegmentCount)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_PROG_BUF_FULL, 
                                                                    CSM_CMD_ADD_USER_DATA, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //保存到缓冲区
    pDataStartAddr = (Bit8*)pBufManData->ulStartAdr;

    if(pBufManData->ulStatus & DATA_BUF_STATUS_STATIC)
    { //静态模式
        memcpy(pDataStartAddr + pBufManData->ulWritePos, pUserData, iDataLen);
        pBufManData->ulSegmentCount += iDataLen;
        pBufManData->ulWritePos += iDataLen;
    }
    else
    { //动态模式
        iLengthTemp1 = pBufManData->ulWritePos + iDataLen - pBufManData->ulTotalSegmentCount;
        iLengthTemp2 = iDataLen - iLengthTemp1;

        if(iLengthTemp1 > 0)
        {
            memcpy(pDataStartAddr + pBufManData->ulWritePos, pUserData, iLengthTemp2);
            memcpy(pDataStartAddr, pUserData + iLengthTemp2, iLengthTemp1);
            pBufManData->ulWritePos = iLengthTemp1;
        }
        else
        {
            memcpy(pDataStartAddr + pBufManData->ulWritePos, pUserData, iDataLen);
            pBufManData->ulWritePos += iDataLen;
        }
    }
    pBufManData->ulSegmentCount += iDataLen;

    CSM_UpdateUserBufUseStatus(iCrdSysIndex);

    return 0;
}

/*
函 数 名：int IPO_GetSegmentCountInBuf(int iCrdSysIndex)
功    能：获取插补缓冲区中的程序段数量
参    数：iCrdSysIndex——坐标系索引，取值0-1
返 回 值：插补缓冲区中的程序段数量
调用位置：插补器内部
注意事项：无
*/
int CSM_GetUserDataCountInBuf(int iCrdSysIndex)
{
    return m_UserBufManData[iCrdSysIndex].ulSegmentCount;
}

//设置读位置，成功返回0，否则返回非0值
uBit32 CSM_SetUserBufReadPos(Bit32 iCrdSysIndex, uBit32 ulReadPos)
{
    uBit32 ulErrorCode;

    //
    if (m_UserBufManData[iCrdSysIndex].ulStatus & DATA_BUF_STATUS_STATIC)
    {
        if (ulReadPos < m_UserBufManData[iCrdSysIndex].ulSegmentCount)
        {
            m_UserBufManData[iCrdSysIndex].ulReadPos = ulReadPos;
            return 0;
        }
    }

    ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_USER_BUF_DYNAMIC, 
        CSM_CMD_SET_USER_BUF_READ_POS, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    return ulErrorCode;
}

/*
函 数 名：int IPO_GetSegmentDataForExecuting(int iCrdSysIndex, SEGMENT_MOTION_DATA_PTE* pMotionDataPte)
功    能：获取一段程序进行插补
参    数：iCrdSysIndex——坐标系索引，取值0-1
          *pMotionDataPte——程序段数据（出参）
返 回 值：实际读取的用户数据长度
调用位置：插补器内部
注意事项：无
*/
Bit32 CSM_GetUserData(Bit32 iCrdSysIndex, Bit8* pUserData, uBit32 ulBufLen)
{
    int iLengthTemp1;
    int iLengthTemp2;
    uBit32 ulReadLen = 0;
    DATA_BUF_MAN* pBufManData = NULL;
    Bit8* pDataStartAddr = NULL;

    //
    if (m_nUserBufCtrlMode[iCrdSysIndex] == 1)
    { //用户缓冲区由解释器控制时，不能通过本函数读数据
        return 0;
    }

    //
    pBufManData = m_UserBufManData + iCrdSysIndex;
    pDataStartAddr = (Bit8*)pBufManData->ulStartAdr;

    //
    if(pBufManData->ulSegmentCount <= 0)
    {
        return 0;
    }

    if(pBufManData->ulStatus & DATA_BUF_STATUS_STATIC)
    { //静态模式
        if(pBufManData->ulReadPos >= pBufManData->ulSegmentCount)
        {
            return 0;
        }

        if((pBufManData->ulReadPos + ulBufLen > pBufManData->ulSegmentCount))
        {
            ulReadLen = pBufManData->ulSegmentCount - pBufManData->ulReadPos;
        }
        else
        {
            ulReadLen = ulBufLen;
        }

        memcpy(pUserData, pDataStartAddr + pBufManData->ulReadPos, ulReadLen);

        pBufManData->ulReadPos += ulReadLen;

        return ulReadLen;
    }
    else 
    { //动态模式
        if(pBufManData->ulSegmentCount <= ulBufLen)
        {
            ulReadLen = pBufManData->ulSegmentCount;
        }
        else
        {
            ulReadLen = ulBufLen;
        }

        iLengthTemp1 = pBufManData->ulReadPos + ulReadLen - pBufManData->ulTotalSegmentCount;
        iLengthTemp2 = pBufManData->ulSegmentCount - iLengthTemp1;
        if(iLengthTemp1 > 0)
        {
            memcpy(pUserData, pDataStartAddr + pBufManData->ulReadPos, iLengthTemp2);
            memcpy(pUserData + iLengthTemp2, pDataStartAddr, iLengthTemp1);
            pBufManData->ulReadPos = iLengthTemp1;
        }
        else
        {
            memcpy(pUserData, pDataStartAddr + pBufManData->ulReadPos, ulReadLen);
            pBufManData->ulReadPos += ulReadLen;
        }
        pBufManData->ulSegmentCount -= ulReadLen;
    }

    return ulReadLen;
}

/*
函 数 名：uBit32 CSM_UploadUserData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen)
功    能：读取用户数据用于上传
参    数：iCrdSysIndex --通道索引
          ulStartPos --数据读取起始位置，取值从0开始
          pData --数据缓冲区起始地址
          ulLen  --数据缓冲区长度
返 回 值：实际读取的用户数据长度
调用位置：应用层调用
注意事项：
*/
uBit32 CSM_UploadUserData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen)
{
    DATA_BUF_MAN* pBufManData = NULL;
    Bit8* pDataStartAddr = NULL;
    uBit32 ulReadCount = 0;

    //
    pBufManData = m_UserBufManData + iCrdSysIndex;
    pDataStartAddr = (Bit8*)pBufManData->ulStartAdr;

    //
    if(!(pBufManData->ulStatus & DATA_BUF_STATUS_STATIC))
    {
        return 0;
    }

    //
    if (ulStartPos >= pBufManData->ulSegmentCount)
    {
        return 0;
    }
    else
    {
        if (pBufManData->ulSegmentCount >= ulStartPos + ulLen)
        {
            ulReadCount = ulLen;
        }
        else
        {
            ulReadCount = pBufManData->ulSegmentCount - ulStartPos;
        }

        memcpy(pData, pDataStartAddr + ulStartPos, ulReadCount);
    }

    return ulReadCount;
}



/*
函 数 名：int IPO_CheckBufFull(int iCrdSysIndex)
功    能：检测插补缓冲区是否满
参    数：iCrdSysIndex——坐标系索引，取值0-1
返 回 值：缓冲区满返回1，否则返回0
调用位置：插补器内部
注意事项：无
*/
int CSM_CheckUserBufFull(int iCrdSysIndex)
{
    if (m_UserBufManData[iCrdSysIndex].ulSegmentCount == m_UserBufManData[iCrdSysIndex].ulTotalSegmentCount)
    {
        return 1;
    }

    return 0;
}

//检测通道程序缓冲区是否为空，为空返回1，否则返回0
Bit32 CSM_CheckUserBufEmpty(Bit32 iCrdSysIndex)
{
    if (m_UserBufManData[iCrdSysIndex].ulSegmentCount == 0)
    {
        return 1;
    }

    return 0;
}


//获取程序缓冲区空闲段数
Bit32 CSM_GetUserBufFreeSize(Bit32 iCrdSysIndex)
{
    return m_UserBufManData[iCrdSysIndex].ulTotalSegmentCount - m_UserBufManData[iCrdSysIndex].ulSegmentCount;
}


Bit32 CSM_GetUserBufTotalSize(Bit32 iCrdSysIndex)
{
    return m_UserBufManData[iCrdSysIndex].ulTotalSegmentCount;
}


void CSM_GetUserBufStateData(Bit32 iCrdSysIndex, DATA_BUF_MAN* pUserBufState)
{
    *pUserBufState = m_UserBufManData[iCrdSysIndex];
}

const DATA_BUF_MAN* CSM_GetUserBufStateReadAddr(Bit32 iCrdSysIndex)
{
    return m_UserBufManData + iCrdSysIndex;
}
