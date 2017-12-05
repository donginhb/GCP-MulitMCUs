/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_ParmMan.C

摘    要：通道系统参数管理

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月25日
*/


#include "string.h"
#include "CSM_ParmMan.h"
#include "CSM_Interface.h"
#include "CSM_MainCtrl.h"
#include "CNC/DataStructDef/AXIS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"

#if(__FPU_USED ==1)
    #include <arm_math.h>
#else
    #include <math.h>
#endif


const SYS_CTRL_PARM*                m_CSM_pSysCtrlParm = NULL;
const CRDSYS_AXIS_MAP_TABLE*        m_CSM_pCrdSysAxisMapTable = NULL;
const CRDSYS_PARM*                    m_CSM_pCrdParm = NULL;
const AXIS_PARM*                    m_CSM_pAxisParm = NULL;

int m_CSM_CrdSysMotorIndex[CRDSYS_MAX_COUNT][AXIS_MAX_COUNT];        //坐标系中的坐标轴对应的电机索引（注意：1个坐标轴可以映射2个电机）


/*
函 数 名：void CSM_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
                                        const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm)
功    能：初始化系统参数读取地址
参    数：
返 回 值：无
调用位置：系统上电时，在系统参数管理模块初始化之后调用
注意事项：无
*/
void CSM_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
                            const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm)
{
    m_CSM_pSysCtrlParm = pSysCtrlParm;
    m_CSM_pCrdSysAxisMapTable = pCrdSysAxisMapTab;
    m_CSM_pCrdParm = pCrdParm;
    m_CSM_pAxisParm = pAxisParm;
}

//获取主轴编码器分辨率
Bit32 CSM_GetSpindleQeiSense(Bit32 iCrdSysIndex)
{
    return m_CSM_pCrdParm->lSpindleQeiSense;
}

//检测通道是否使能
Bit32 CSM_GetEnable(Bit32 iCrdSysIndex)
{
    
    //if (m_CSM_pSysCtrlParm->nCrdSysCount >= iCrdSysIndex)
    if (m_CSM_pSysCtrlParm->nCrdSysCount > iCrdSysIndex)//Alan modified at 2016-7-20
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void CSM_ParmManInit()
{
    int i, j;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        for (j = 0; j < AXIS_MAX_COUNT; j++)
        {
            m_CSM_CrdSysMotorIndex[i][j] = -1;
        }
    }
}

/*
函 数 名：void IPO_InitCrdSysMotorIndex()
功    能：初始化坐标系电机编号表
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：插补器内部调用(坐标系映射表改变时调用)
注意事项：(主要为了方便插补器内部使用)
*/
unsigned long CSM_InitCrdSysMotorIndex()
{
    int i, j, k;
    int iIndex;
    int iCrdSysCount;
    unsigned short nAxisCount;
    const CRD_AXIS_MAP* pCrdSysAxisMap;

    //
    iCrdSysCount = m_CSM_pSysCtrlParm->nCrdSysCount;
    for (i = 0; i < iCrdSysCount; i++)
    {
        //
        for (j = 0; j < AXIS_MAX_COUNT; j++)
        {
            m_CSM_CrdSysMotorIndex[i][j] = -1;
        }

        //
        pCrdSysAxisMap = m_CSM_pCrdSysAxisMapTable[i].CrdSysAxisMap;
        nAxisCount = m_CSM_pCrdSysAxisMapTable[i].nCrdAxisCount;
        m_CSM_NCAxisData[i] = (1<<m_CSM_pCrdSysAxisMapTable[i].nCrdAxisCount)-1;

        iIndex = 0;

        for (j = 0; j < nAxisCount; j++)
        {
            for (k = 0; k < AXIS_MAP_MOTOR_MAX_COUNT; k++)
            {
                if (pCrdSysAxisMap[j].cMotorNO[k] >= AXIS_MAX_COUNT)
                {
                    break;
                }

                m_CSM_CrdSysMotorIndex[i][iIndex] = pCrdSysAxisMap[j].cMotorNO[k];
                iIndex++;
            }
        }
    }

    return 0;
}



/*
函 数 名：int SPM_GetCrdSysCount()
功    能：获取系统坐标系数量
参    数：无
返 回 值：系统坐标系数量
调用位置：
注意事项：
*/
int CSM_GetCrdSysCount()
{
    return m_CSM_pSysCtrlParm->nCrdSysCount;
}

/*
函 数 名：int SPM_CheckCrdSysInterrelation()
功    能：检测坐标系是否关联
参    数：无
返 回 值：坐标系关联返回1，否则返回0
调用位置：
注意事项：
*/
int CSM_CheckCrdSysInterrelation()
{
    if (m_CSM_pSysCtrlParm->nSysSet & SYS_SET_CRDSYSINTERRELATION)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//获取系统所有坐标系的掩码
unsigned long CSM_GetAllCrdSysMask()
{
    int i;
    uBit32 ulMask = 0;

    for (i = 0; i < m_CSM_pSysCtrlParm->nCrdSysCount; i++)
    {
        ulMask |= (0x0001 << i);
    }

    return ulMask;
}

//获取坐标系坐标轴数量
int CSM_GetCrdSysAxisCount(int iCrdSysIndex)
{
    return m_CSM_pCrdSysAxisMapTable[iCrdSysIndex].nCrdAxisCount;
}

//获取坐标轴映射的电机编号
int CSM_GetAxisMapMotorNO(int iCrdSysIndex, int iAxisIndex, int iFirstMotor)
{
    if (iFirstMotor == 1)
    {
        return m_CSM_pCrdSysAxisMapTable[iCrdSysIndex].CrdSysAxisMap[iAxisIndex].cMotorNO[0];
    }
    else
    {
        return m_CSM_pCrdSysAxisMapTable[iCrdSysIndex].CrdSysAxisMap[iAxisIndex].cMotorNO[1];
    }
}

//获取坐标轴映射的电机掩码
uBit32 CSM_GetAxisMapMotorMask(int iCrdSysIndex, int iAxisIndex)
{
    int i;
    uBit32 ulMotorNO;
    uBit32 ulMotorMask = 0;

    for (i = 0; i < AXIS_MAP_MOTOR_MAX_COUNT; i++)
    {
        ulMotorNO = m_CSM_pCrdSysAxisMapTable[iCrdSysIndex].CrdSysAxisMap[iAxisIndex].cMotorNO[i];
        if (ulMotorNO < AXIS_MAX_COUNT)
        {
            ulMotorMask |= 0x0001 << ulMotorNO;
        }
        else
        {
            break;
        }
    }

    return ulMotorMask;
}

//获取坐标系中分配的电机的掩码
uBit32 CSM_GetCrdSysMapMotorMask(int iCrdSysIndex)
{
    int i;
    uBit32 ulMotorMask = 0;

    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        if (m_CSM_CrdSysMotorIndex[iCrdSysIndex][i] >= 0)
        {
            ulMotorMask |= 0x0001 << m_CSM_CrdSysMotorIndex[iCrdSysIndex][i];
        }
        else
        {
            break;
        }
    }

    return ulMotorMask;
}

//获取电机所在的坐标系索引，返回-1表示电机未分配坐标系
int CSM_GetMotorMapCrdSysIndex(int iMotorNO)
{
    int iCrdSysCount;
    int iAxisCount;
    int i, j, k;

    iCrdSysCount = m_CSM_pSysCtrlParm->nCrdSysCount;

    for (i = 0; i < iCrdSysCount; i++)
    {
        iAxisCount = m_CSM_pCrdSysAxisMapTable[i].nCrdAxisCount;

        for (j = 0; j < iAxisCount; j++)
        {
            for (k = 0; k < AXIS_MAP_MOTOR_MAX_COUNT; k++)
            {
                if (m_CSM_pCrdSysAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] == iMotorNO)
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

