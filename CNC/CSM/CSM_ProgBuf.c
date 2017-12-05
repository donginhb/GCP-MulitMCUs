
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_ProgBuf.c

摘    要：程序缓冲区管理模块

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月08日
*/

#include "string.h"
#include "CSM_ProgBuf.h"
#include "CSM_Interface.h"
#include "CSM_CrdSysState.h"
#include "CSM_MainCtrl.h"
#include "CSM_ParmMan.h"

#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"
#include "CNC/ErrorCodeDef/CSM_ErrorCodeDef.h"

//
extern CSM_INTERFACE    m_CSMInterface;

static DATA_BUF_MAN        m_ProgBufManData[CRDSYS_MAX_COUNT];
static unsigned long    m_ulProgBufAddr = 0;
static unsigned long    m_ulProgBufLen = 0;
static uBit32            m_StartReadPosProgLineNO[CRDSYS_MAX_COUNT];            //当前起始读位置对应的程序行号

//设置插补器插补缓冲区地址
void CSM_SetProgBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen)
{
    m_ulProgBufAddr = ulStartAddr;
    m_ulProgBufLen = ulBufLen;
}


/*
函 数 名：void IPO_InitIPOBuf()
功    能：初始化插补缓冲区
参    数：无
返 回 值：无
调用位置：插补器内部调用（插补器初始化时调用，坐标系设置改变时调用）,
注意事项：无
*/
void CSM_InitProgBuf()
{
    int i;
    int iCrdSysCount;
    long ulBufSize;

    //
    if (m_ulProgBufLen == 0 || m_ulProgBufAddr == 0)
    {
        for (i = 0; i < CRDSYS_MAX_COUNT; i++)
        {
            CSM_SetCrdSysAlarmBit(i, CRDSYS_ALARM_CSM_INIT_ERROR);
        }

        return;
    }

    //
    iCrdSysCount = CSM_GetCrdSysCount();

    ulBufSize = m_ulProgBufLen / iCrdSysCount;

    //（内存地址对齐处理）
    ulBufSize = ulBufSize - ulBufSize % 4;
    if (ulBufSize < 0)
    {
        ulBufSize = 0;
    }

    for (i = 0; i < iCrdSysCount; i++)
    {
        m_ProgBufManData[i].ulStartAdr = m_ulProgBufAddr + i*ulBufSize;
        m_ProgBufManData[i].ulSize = ulBufSize;
        m_ProgBufManData[i].ulTotalSegmentCount = ulBufSize / sizeof(IIP_BUF);

        m_ProgBufManData[i].ulSegmentCount = 0;
        m_ProgBufManData[i].ulReadPos = 0;
        m_ProgBufManData[i].ulWritePos = 0;
    }    

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        m_StartReadPosProgLineNO[i] = 1;
        m_pIprStartLineCount[i]=0;
        m_pIprFinishLineCount[i]=0;
        m_ProgBufManData[i].ulStatus = DATA_BUF_STATUS_STATIC | DATA_BUF_STATUS_EMPTY;
    }
}

//设置程序缓冲区工作模式，iMode： 1-静态模式，0-动态模式
uBit32 CSM_SetProgBufMode(Bit32 iCrdSysIndex, Bit32 iMode)
{
    uBit32 ulStopBit;
    uBit32 ulErrorCode;

    ulStopBit = m_CrdSysStateData[iCrdSysIndex].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL);

    if (!ulStopBit)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_PROG_RUNNING, 
            CSM_CMD_SET_PROG_BUF_MODE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (iMode)
    {
        m_ProgBufManData[iCrdSysIndex].ulStatus |= DATA_BUF_STATUS_STATIC;
    }
    else
    {
        m_ProgBufManData[iCrdSysIndex].ulStatus &= ~DATA_BUF_STATUS_STATIC;
    }

    return 0;
}

uBit32 CSM_OpenProgBuf(Bit32 iCrdSysIndex)
{
    m_ProgBufManData[iCrdSysIndex].ulStatus |= DATA_BUF_STATUS_OPEN;

    return 0;
}


uBit32 CSM_CloseProgBuf(Bit32 iCrdSysIndex)
{
    m_ProgBufManData[iCrdSysIndex].ulStatus &= ~DATA_BUF_STATUS_OPEN;

    return 0;
}

/*
函 数 名：void CSM_ClearProgData(Bit32 iCrdSysIndex, Bit32 iClearMode)
功    能：清除程序数据
参    数：iCrdSysIndex --坐标系索引
          iClearMode --清除模式，取值为：
                        0-清除没有运行的数据段
                        1-清除所有的运动段，但不包括正在执行的运动段
                        2-清除所有的运动段，包括正在执行的运动段
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
uBit32 CSM_ClearProgData(Bit32 iCrdSysIndex, Bit32 iClearMode)
{
    uBit32 ulErrorCode;
    //int iCount;
    //int iReadPos;
    DATA_BUF_MAN* pBufManData = NULL;

    //
    if (!(m_CrdSysStateData[iCrdSysIndex].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_PROG_RUNNING, 
            CSM_CMD_CLEAR_PROG_DATA, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    pBufManData = &(m_ProgBufManData[iCrdSysIndex]);

    /*  本段程序不能要，当前剩余段数ulSegmentCount == 0时，还有其他状态需要清理
    if (pBufManData->ulSegmentCount == 0)
    {
        return 0;
    }
    */

    /*
    if (iClearMode == 0)
    { //清除没有运行的数据段
        iCount = m_StartReadPosProgLineNO[iCrdSysIndex] + pBufManData->ulSegmentCount - 1 - m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO;

        pBufManData->ulSegmentCount -= iCount;

        if (pBufManData->ulWritePos >= iCount)
        {
            pBufManData->ulWritePos -= iCount;
        }
        else
        {
            pBufManData->ulWritePos = pBufManData->ulTotalSegmentCount - (iCount - pBufManData->ulWritePos);
        }

        pBufManData->ulStatus &= ~DATA_BUF_STATUS_USESTATUSMASK;
        pBufManData->ulStatus |= DATA_BUF_STATUS_HALF;
    }
    else if (iClearMode == 1)
    { //清除所有的运动段，但不包括正在执行的运动段
        iCount = m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO - m_StartReadPosProgLineNO[iCrdSysIndex];
        iReadPos = pBufManData->ulReadPos + iCount;
        if (iReadPos >= pBufManData->ulTotalSegmentCount)
        {
            iReadPos = iCount - (pBufManData->ulTotalSegmentCount - pBufManData->ulReadPos);
        }
        pBufManData->ulReadPos = iReadPos;
        pBufManData->ulWritePos = pBufManData->ulReadPos + 1;
        pBufManData->ulSegmentCount = 1;
    }
    else */
    { //清除所有的运动段，包括正在执行的运动段
        pBufManData->ulSegmentCount = 0;
        pBufManData->ulReadPos = 0;
        pBufManData->ulWritePos = 0;

        pBufManData->ulStatus &= ~DATA_BUF_STATUS_USESTATUSMASK;
        pBufManData->ulStatus |= DATA_BUF_STATUS_EMPTY;

        m_StartReadPosProgLineNO[iCrdSysIndex] = 1;

        //
        m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO = 0;

        //////////////////////
        //清空程序缓冲区时，自动清除程序执行状态
        m_CSM_AutoCtrl[iCrdSysIndex].ulCurProcessLineNO = 1;
        m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO = 0;

        //m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO = 0;
        //m_CrdSysStateData[iCrdSysIndex].ulUserLineNO = 0;/Alan del at 2016-8-6
        m_CrdSysStateData[iCrdSysIndex].ulStartLineCount = 0;
        m_CrdSysStateData[iCrdSysIndex].ulFinishLineCount = 0;
        //////////////////////

    }

    return 0;
}

//
void CSM_UpdateProgBufUseStatus(int iCrdSysIndex)
{
    DATA_BUF_MAN* pBufManData = NULL;

    //
    pBufManData = &(m_ProgBufManData[iCrdSysIndex]);

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
函 数 名uBit32 CSM_AddProgDataToBuf(Bit32 iCrdSysIndex, IIP_BUF* pProgData)
功    能：添加运动段数据到插补缓冲区
参    数：iCrdSysIndex    ——坐标系索引，取值0-1
          pProgData        ——插补数据段
返 回 值：成功返回0，否则返回错误码
调用位置：应用层
注意事项：
*/
uBit32 CSM_AddProgDataToBuf(Bit32 iCrdSysIndex, IIP_BUF* pProgData)
{
    uBit32 ulErrorCode;
    IIP_BUF* pProgDataStartAddr = NULL;
    DATA_BUF_MAN* pBufManData = NULL;

    pBufManData = &(m_ProgBufManData[iCrdSysIndex]);

    if(CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_AUTO)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
                                                                    CSM_CMD_ADD_PROG_DATA, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (pBufManData->ulSegmentCount >= pBufManData->ulTotalSegmentCount)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_PROG_BUF_FULL, 
                                                                    CSM_CMD_ADD_PROG_DATA, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //保存到程序缓冲区
    pProgDataStartAddr = (IIP_BUF*)pBufManData->ulStartAdr;
    *(pProgDataStartAddr + pBufManData->ulWritePos) = *pProgData;
    pBufManData->ulSegmentCount++;
    pBufManData->ulWritePos++;
    if (pBufManData->ulWritePos == pBufManData->ulTotalSegmentCount)
    {
        pBufManData->ulWritePos = 0;
    }

    CSM_UpdateProgBufUseStatus(iCrdSysIndex);

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
int CSM_GetProgSegmentCountInBuf(int iCrdSysIndex)
{
    return m_ProgBufManData[iCrdSysIndex].ulSegmentCount;
}


/*
函 数 名：int CSM_GetProgData(int iCrdSysIndex, unsigned long ulProgLineNO, IIP_BUF* pProgData)
功    能：获取一段程序进行插补
参    数：iCrdSysIndex——坐标系索引，取值0-1
          *pMotionDataPte——程序段数据（出参）
返 回 值：成功返回0，否则返回非0值
调用位置：插补器内部
注意事项：无
*/
int CSM_GetProgData(int iCrdSysIndex, unsigned long ulProgLineNO, IIP_BUF* pProgData)
{
    DATA_BUF_MAN* pBufManData = NULL;
    IIP_BUF* pProgDataStartAddr = NULL;
    unsigned long ulLIneOffset = 0;
    unsigned long ulReadPos;

    //
    pBufManData = &(m_ProgBufManData[iCrdSysIndex]);

    //
    if (pBufManData->ulSegmentCount == 0)
    {
        return 1;
    }
    
    //
    if (ulProgLineNO >  m_StartReadPosProgLineNO[iCrdSysIndex] + pBufManData->ulSegmentCount - 1)
    {
        return 1;
    }

    //
    if (ulProgLineNO < m_StartReadPosProgLineNO[iCrdSysIndex])
    {
        CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
        return 1;        //如果出现该错误，则说明系统程序设计存在错误
    }

    ulLIneOffset = ulProgLineNO - m_StartReadPosProgLineNO[iCrdSysIndex];
    
    //
    pProgDataStartAddr = (IIP_BUF*)pBufManData->ulStartAdr;
    ulReadPos = pBufManData->ulReadPos + ulLIneOffset;
    if (ulReadPos >= pBufManData->ulTotalSegmentCount)
    {
        ulReadPos = ulLIneOffset - (pBufManData->ulTotalSegmentCount - pBufManData->ulReadPos);
    }
    *pProgData = *(pProgDataStartAddr + ulReadPos);

    //pBufManData->ulSegmentCount--;
    //pBufManData->ulReadPos++;
    //if (pBufManData->ulReadPos == pBufManData->ulTotalSegmentCount)
    //{
    //    pBufManData->ulReadPos = 0;
    //}

    return 0;
}

//释放程序段（程序段执行完成之后）
void CSM_ReleaseProgData(int iCrdSysIndex, unsigned long ulProgLineNO)
{
    DATA_BUF_MAN* pBufManData = NULL;

    pBufManData = &(m_ProgBufManData[iCrdSysIndex]);

    //静态模式下不释放已执行完成的程序段
    if (pBufManData->ulStatus & DATA_BUF_STATUS_STATIC)
    {
        return;
    }

    //
    if (ulProgLineNO != m_StartReadPosProgLineNO[iCrdSysIndex])
    {
        CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
        return;    
    }

    m_StartReadPosProgLineNO[iCrdSysIndex]++;

    pBufManData->ulSegmentCount--;
    pBufManData->ulReadPos++;
    if (pBufManData->ulReadPos == pBufManData->ulTotalSegmentCount)
    {
        pBufManData->ulReadPos = 0;
    }

    //
    CSM_UpdateProgBufUseStatus(iCrdSysIndex);
}

//获取程序缓冲区状态字
uBit32 CSM_GetProgBufStatus(int iCrdSysIndex)
{
    return m_ProgBufManData[iCrdSysIndex].ulStatus;
}


/*
函 数 名：int IPO_CheckBufFull(int iCrdSysIndex)
功    能：检测插补缓冲区是否满
参    数：iCrdSysIndex——坐标系索引，取值0-1
返 回 值：缓冲区满返回1，否则返回0
调用位置：插补器内部
注意事项：无
*/
int CSM_CheckProgBufFull(int iCrdSysIndex)
{
    if (m_ProgBufManData[iCrdSysIndex].ulSegmentCount == m_ProgBufManData[iCrdSysIndex].ulTotalSegmentCount)
    {
        return 1;
    }

    return 0;
}

//检测通道程序缓冲区是否为空，为空返回1，否则返回0
Bit32 CSM_CheckProgBufEmpty(Bit32 iCrdSysIndex)
{
    if (m_ProgBufManData[iCrdSysIndex].ulSegmentCount == 0)
    {
        return 1;
    }

    return 0;
}


//获取程序缓冲区空闲段数
Bit32 CSM_GetProgBufFreeCount(Bit32 iCrdSysIndex)
{
    return m_ProgBufManData[iCrdSysIndex].ulTotalSegmentCount - m_ProgBufManData[iCrdSysIndex].ulSegmentCount;
}



/*
函 数 名：uBit32 CSM_UploadProgData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen)
功    能：读取程序数据用于上传
参    数：iCrdSysIndex --通道索引
          ulStartPos --数据读取起始位置，取值从0开始
          pData --数据缓冲区起始地址
          ulLen  --数据缓冲区长度
返 回 值：实际读取的程序数据长度
调用位置：应用层调用
注意事项：
*/
uBit32 CSM_UploadProgData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen)
{
    DATA_BUF_MAN* pBufManData = NULL;
    Bit8* pDataStartAddr = NULL;
    uBit32 ulReadCount = 0;
    int iSegmentLen = 1;

    //
    pBufManData = m_ProgBufManData + iCrdSysIndex;
    pDataStartAddr = (Bit8*)pBufManData->ulStartAdr;
    iSegmentLen = sizeof(IIP_BUF);

    //
    if(!(pBufManData->ulStatus & DATA_BUF_STATUS_STATIC))
    {
        return 0;
    }

    //
    if (ulStartPos >= pBufManData->ulSegmentCount * iSegmentLen)
    {
        return 0;
    }
    else
    {
        if (pBufManData->ulSegmentCount * iSegmentLen >= ulStartPos + ulLen)
        {
            ulReadCount = ulLen;
        }
        else
        {
            ulReadCount = pBufManData->ulSegmentCount * iSegmentLen - ulStartPos;
        }

        memcpy(pData, pDataStartAddr + ulStartPos, ulReadCount);
    }

    return ulReadCount;
}

const DATA_BUF_MAN* CSM_GetProgBufStateReadAddr(Bit32 iCrdSysIndex)
{
    return m_ProgBufManData + iCrdSysIndex;
}
