/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_StateMan.c

摘    要：系统状态管理

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月16日

///////////////////////////////////
修改作者：Alan
         1.添加清除系统报警状态位接口
修改日期：2015年11月28日
///////////////////////////////////
修改作者：Alan
1.添加1us延时接口
2.修改SYS_MainCtrl()接口，添加非轴设备的报警检测(过流、掉线、发送周期性数据失败)，如果有报警则所有坐标系急停
修改日期：2016年4月26日
///////////////////////////////////
使用说明：
*/
#include <string.h>
#include "CNCSYS_Interface.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"
#include "..\\ErrorCodeDef\\SYS_ErrorCodeDef.h"
#include "..\\ErrorCodeMan\\ECM_ErrorCodeMan.h"
#include "..\\Version\\Version.h"
#include "..\\CMU\\CMU_Interface.h"
#include "..\\CSM\\CSM_Interface.h"
#include "..\\DVM\\DEV_Interface.h"
#include "..\\SPM\\SPM_SysCtrlParm.h"

#include "..\\..\\Version\\VER_DataStructDef.h"



#include "..\\IPO\\IPO_M0\\IPO_Interface.h"

#include "..\\..\\SysUpdate\\Bootloader\\BootLoaderDefine.h"
//#include "..\\Bootloader\\BootLoader.h"




SYS_STATE_DATA    m_SysStateData;
double64 m_dSysTickCount = 0.0;
double64 m_dSysTickTime = 1.0;  //(ms)
//static uBit8 m_pVerNo[2];          //版本号


/*
函 数 名：void SYS_InitStateData()
功    能：系统状态数据初始化
参    数：无
返 回 值：无
调用位置：
注意事项：
*/
void SYS_InitStateData()
{
    memset(&m_SysStateData, 0, sizeof(m_SysStateData));

    m_dSysTickTime = 1.0;
    m_dSysTickCount = 0.0;
}


/*
函 数 名：uBit32 SYS_Reset()
功    能：系统复位
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 SYS_Reset()   //本函的功能需重新规划
{
    /*
    uBit32 ulErrorCode;

    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SYS, 0, SYS_ERR_CRDSYS_NOT_FREE, 
            SYS_CMD_RESET, 0, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }    


    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SYS, 0, SYS_ERR_IPO_NOT_FREE, 
            SYS_CMD_RESET, 0, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    SYS_Init();

    //BOOT_SysReset();  //网络会断开
    */
  
    return 0;
}

/*
函 数 名：const SYS_STATE_DATA* SYS_GetStateReadAddr()
功    能：获取系统状态数据读取地址
参    数：无
返 回 值：系统状态数据读取地址
调用位置：
注意事项：
*/
const SYS_STATE_DATA* SYS_GetStateReadAddr()
{
    return &m_SysStateData;
}

/*
函 数 名：Bit32 SYS_CheckResetState()
功    能：检测系统是否正在复位
参    数：无
返 回 值：正在复位返回1，否则返回0
调用位置：
注意事项：
*/
Bit32 SYS_CheckResetState()
{
    return 0;
}

/*
函 数 名：void SYS_SetAlarmBit(uBit32 ulAlarmBit)
功    能：设置系统报警状态位
参    数：ulAlarmBit --系统报警状态位
返 回 值：无
调用位置：
注意事项：
*/
void SYS_SetAlarmBit(uBit32 ulAlarmBit)
{
    m_SysStateData.ulSysExtAlarm |= ulAlarmBit;
}

/*
函 数 名：void SYS_ClrAlarmBit(uBit32 ulAlarmBit)
功    能：清除系统报警状态位
参    数：ulAlarmBit --系统报警状态位
返 回 值：无
调用位置：
注意事项：
*/
void SYS_ClrAlarmBit(uBit32 ulAlarmBit)
{
    m_SysStateData.ulSysExtAlarm &= ~ulAlarmBit;
}

/*
函 数 名：Bit32 SYS_GetAlarmBit(uBit32 ulAlarmBit)
功    能：检测系统是否存在指定的报警状态位
参    数：ulAlarmBit --系统报警状态位
返 回 值：否存在指定的报警状态位返回1，否则返回0
调用位置：
注意事项：
*/
Bit32 SYS_GetAlarmBit(uBit32 ulAlarmBit)
{
    if (m_SysStateData.ulSysExtAlarm & ulAlarmBit)
    {
        return 1;
    }
    
    return 0;
}

uBit32 SYS_GetAlarmStatus()
{
    return m_SysStateData.ulSysExtAlarm;
}

uBit32 SYS_GetSysStatus()
{
    return m_SysStateData.ulSysRunningStatus;
}

/*
函 数 名：void SYS_SetStatusBit(uBit32 ulStatusBit)
功    能：设置系统状态位
参    数：ulStatusBit --系统状态位
返 回 值：无
调用位置：
注意事项：
*/
void SYS_SetStatusBit(uBit32 ulStatusBit)
{
    m_SysStateData.ulSysRunningStatus |= ulStatusBit;
}

/*
函 数 名：void SYS_ClrStatusBit(uBit32 ulStatusBit)
功    能：清除系统状态位
参    数：ulStatusBit --系统状态位
返 回 值：无
调用位置：
注意事项：
*/
void SYS_ClrStatusBit(uBit32 ulStatusBit)
{
    m_SysStateData.ulSysRunningStatus &= ~ulStatusBit;
}



/*
函 数 名：void SYS_SetSysTickCount(void)
功    能：设置定时中断值加1
参    数：无
返 回 值：无
调用位置：定时中断调用
注意事项：无
*/
void SYS_SetSysTickCount(void)
{
    m_dSysTickCount += m_dSysTickTime;
}

//
void SYS_SetSysTickTimeCylce(uBit32 ulTimeCycle)
{
    m_dSysTickTime = ulTimeCycle * 1.0 / 1000;
}

/*
函 数 名：double64 SYS_GetSysTickCount(void)
功    能：获取定时中断值
参    数：无
返 回 值：定时中断值
调用位置：应用层调用
注意事项：无
*/
double64 SYS_GetSysTickCount(void)
{
#ifndef WIN32_VER
    return m_dSysTickCount;
#else
    return GetTickCount();
#endif
}


/*
函 数 名：void SYS_SetSLCVersion(uBit8 uMainNo, uBit8 uSubNo)
功    能：设置M0程序版本信息
参    数：uMainNo -- 主版本号
          uSubNo -- 子版本号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：

void SYS_SetSLCVersion(uBit8 uMainNo, uBit8 uSubNo)
{
   m_pVerNo[0] = uMainNo;
   m_pVerNo[1] = uSubNo;   
}*/ 

/*
函 数 名：uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion)
功    能：获取M0程序版本信息
参    数：*pVersion --程序版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion)
{
    memcpy(pVersion, APP_VER_STR, sizeof(SOFTWARE_VERSION));
    pVersion->cSoftVer[0] = APP_VER_NO0;
    pVersion->cSoftVer[1] = APP_VER_NO1;
    pVersion->cPart = APP_VER_PART;
    return 0;
}





/*
函 数 名：uBit32 SYS_SimpleDelayUs(uBit32 ulUs)
功    能：延时us(100000循环约为36us, 1us约为2777循环)
参    数：ulUs
返 回 值：无
调用位置：
注意事项：
*/
void SYS_SimpleDelayUs(uBit32 ulUs)
{
    uBit32 i;
    uBit32 ulCycle;

    ulCycle = ulUs*2770;
    for (i=ulCycle;  i>0;  i--);
}


/*
void SYS_MainCtrl()
{
    uBit8 ulNonAxisAlarm=0;
    const SYS_CTRL_PARM *pSysCtrlParm;

    //切换通信方式
    if (m_uComSetDelayCycles!=0)
    {
        m_uComSetDelayCycles--;

        if (m_uComSetDelayCycles==0)
        {
            //切换通信方式
            //uBit32 ulComType, ulNode;
            //BOOT_GetComType(&ulComType, &ulNode);
            //CMU_Init(ulComType);
            CMU_Init(COM_TYPE_ENET);
        }
    }  

    //非轴设备报警检测，如果有报警则停止坐标系运动(急停)
    ulNonAxisAlarm = DEV_CheckNonAxisDevAlarm();

    if (ulNonAxisAlarm==0)
    {
        SYS_ClrAlarmBit(SYS_ALARM_IO_OVER_CURRENT|SYS_ALARM_DEV_OFF_LINE|SYS_ALARM_DEV_OFF_LINE);
    }else
    {
        if (ulNonAxisAlarm&IO_STATUS_OVER_CURRENT)//IO模块过流报警
            SYS_SetAlarmBit(SYS_ALARM_IO_OVER_CURRENT);
        else
            SYS_ClrAlarmBit(SYS_ALARM_IO_OVER_CURRENT);

        if (ulNonAxisAlarm&IO_STATUS_OFF_LINE)//IO脱离总线
            SYS_SetAlarmBit(SYS_ALARM_DEV_OFF_LINE);
        else
            SYS_ClrAlarmBit(SYS_ALARM_DEV_OFF_LINE);

        if (ulNonAxisAlarm&IO_STATUS_SEND_CYCLE_DATA_FAIL) //发送周期数据失败
            SYS_SetAlarmBit(SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL);
        else
            SYS_ClrAlarmBit(SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL);

        //
        pSysCtrlParm = SPM_GetSysCtrlParmAddr();
        
        if(pSysCtrlParm->nCrdSysCount)
        {
              if(CSM_CheckEStop(0)==0)
                    IPO_ProgEStop((1<<pSysCtrlParm->nCrdSysCount)-1);
        }
    }
}*/
