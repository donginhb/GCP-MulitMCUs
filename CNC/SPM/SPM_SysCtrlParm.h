/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：SPM_SysCtrlParm.h

摘    要：参数管理模块

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月15日
*/

//系统参数管理模块
#ifndef SYS_CTRL_PARM_H
#define SYS_CTRL_PARM_H

#include "CNC/DataStructDef/AXIS_DataStructDef.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"



#ifdef __cplusplus
extern "C" {
#endif

    /********************************************************************** 
                               控制参数管理接口
    ***********************************************************************/
    
    /*
    函 数 名：const SYS_CTRL_PARM* SPM_GetSysCtrlParmAddr()
    功    能：获取系统控制参数访问地址（只读）
    参    数：无
    返 回 值：系统控制参数访问地址
    调用位置：
    注意事项：无
    */
    const SYS_CTRL_PARM* SPM_GetSysCtrlParmAddr();


    /*
    函 数 名：const CRDSYS_AXIS_MAP_TABLE* SPM_GetCrdSysAxisMapTableAddr()
    功    能：获取坐标系映射表访问地址（只读）
    参    数：无
    返 回 值：坐标系映射表访问地址
    调用位置：
    注意事项：无
    */
    const CRDSYS_AXIS_MAP_TABLE* SPM_GetCrdSysAxisMapTableAddr();

    /*
    函 数 名：const CRDSYS_PARM* SPM_GetCrdSysParmAddr()
    功    能：获取坐标系参数访问地址（只读）
    参    数：无
    返 回 值：坐标系参数访问地址
    调用位置：
    注意事项：无
    */
    const CRDSYS_PARM* SPM_GetCrdSysParmAddr();

    /*
    函 数 名：const AXIS_PARM* SPM_GetAxisParmAddr()
    功    能：获取轴参数访问地址（只读）
    参    数：无
    返 回 值：轴参数访问地址
    调用位置：
    注意事项：无
    */
    const AXIS_PARM* SPM_GetAxisParmAddr();
    
    /*
    函 数 名：const AXIS_PITCH_CMP_PARM* SPM_GetAxisPitchCmpParmAddr()
    功    能：获取轴螺距补偿地址（只读）
    参    数：无
    返 回 值：轴螺距补偿参数访问地址
    调用位置：
    注意事项：无
    */
    const AXIS_PITCH_CMP_PARM* SPM_GetAxisPitchCmpParmAddr();


    /*
    函 数 名：void SPM_InitSysCtrlData()
    功    能：初始化系统控制参数
    参    数：无
    返 回 值：无
    调用位置：系统上电时调用
    注意事项：无
    */
    void SPM_InitSysCtrlParm();

    /*
    函 数 名：uBit32 SPM_SetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
    功    能：设置系统控制参数
    参    数：*pSysCtrlParm——系统控制参数
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 SPM_SetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm);

    /*
    函 数 名：uBit32 SPM_SetAxisParm(Bit32 iAxisNO, AXIS_PARM* pAxisParm)
    功    能：设置电机轴控制参数
    参    数：iAxisNO --电机编号
              *pAxisParm --电机轴控制参数数据
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 SPM_SetAxisParm(Bit32 iAxisNO, AXIS_PARM* pAxisParm);

    /*
    函 数 名：uBit32 SPM_SetAxisPitchCmpParm(int iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm)
    功    能：设置电机轴螺距补偿参数
    参    数：iAxisNO --电机编号
              *pPitchParm --电机轴螺距补偿参数
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 SPM_SetAxisPitchCmpParm(Bit32 iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm);

    /*
    函 数 名：uBit32 SPM_SetCrdSysAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount)
    功    能：设置坐标系坐标轴映射表
    参    数：pCrdAxisMapTable --坐标系映射表数据起始地址
              iCrdSysCount --坐标系数量
    返 回 值：成功返回0, 否则返回错误码
    注意事项：系统所有坐标系的坐标系映射表必须一次性设置，以便于数据合法性检查
    */
    uBit32 SPM_SetCrdSysAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount);

    /*
    函 数 名：uBit32 SPM_SetCrdParm(Bit32 iCrdSysIndex, CRDSYS_PARM* pCrdParm)
    功    能：设置坐标系参数
    参    数：iCrdSysIndex --坐标系索引
              *pCrdParm --坐标系参数
    返 回 值：成功返回0, 否则返回错误码
    注意事项：
    */
    uBit32 SPM_SetCrdParm(Bit32 iCrdSysIndex,  CRDSYS_PARM* pCrdParm);
    

    /*
    函 数 名：uBit32 SPM_SetLookaheadEnable(Bit32 iCrdSysIndex, Bit32 iEnable)
    功    能：设置坐标系前瞻使能控制参数
    参    数：iCrdSysIndex     --坐标系索引 
              iEnable          --坐标系参数 0-取消前瞻控制 1-使能前瞻控制
    返 回 值：成功返回0, 否则返回错误码
    注意事项：
    */
    uBit32 SPM_SetLookaheadEnable(Bit32 iCrdSysIndex, Bit32 iEnable);

    /*
    函 数 名：uBit16 SPM_GetSysTimeCycleUs()
    功    能：获取系统插补周期
    参    数：无
    返 回 值：插补周期，单位us
    注意事项：
    */
    uBit16 SPM_GetSysTimeCycleUs();

    /*
    函 数 名：void SPM_GetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
    功    能：获取系统控制参数
    参    数：pSysCtrlParm--保存获取的系统控制参数
    返 回 值：无
    注意事项：
    */
    void SPM_GetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm);
    
    /*
    函 数 名：void SPM_GetAxisParm(int iAxisIndex, AXIS_PARM* pAxisParm)
    功    能：获取物理轴控制参数
    参    数：iAxisIndex --轴序号（设备编号）
              pAxisParm  --保存获取的轴控制参数
    返 回 值：无
    注意事项：
    */    
    void SPM_GetAxisParm(int iAxisIndex, AXIS_PARM* pAxisParm);   
    
    /*
    函 数 名：uBit32 SPM_SaveConfAndParm(void)
    功    能：保存系统配置及控制参数到flash
    参    数：无
    返 回 值：成功返回0, 否则返回错误码
    注意事项：
    */    
    uBit32 SPM_SaveConfAndParm(void);    
    
    //从flash中加载硬件配置数据
    uBit32 SPM_LoadConfig();

    /*
    函 数 名：uBit32 SPM_SendSysCtrlParmToDev()
    功    能：更新系统控制参数到设备端
    参    数：无
    返 回 值：成功返回0, 否则返回错误码
    注意事项：插补周期更改
        
    uBit32 SPM_SendSysCtrlParmToDev();*/

    //设置硬件配置信息(设置硬件配置信息后，坐标系映射表需重新建立)
    /*
    函 数 名：uBit32 SPM_SetConfig(DEVICE_CONFIG *pDevConf, uBit32 ulDevCount)
    功    能：设置硬件配置信息
    参    数：pDevConf -- 设备配置数据
             ulDevCount-- 要配置的设备数量
    返 回 值：成功返回0, 否则返回错误码
    注意事项：
    */    
    uBit32 SPM_SetConfig(DEVICE_CONFIG *pDevConf, uBit32 ulDevCount);  
#ifdef __cplusplus
}
#endif




#endif