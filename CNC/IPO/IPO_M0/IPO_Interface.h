
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：IPO_Interface.h

摘    要：插补器接口函数

运行环境：LPC43xx 

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年8月12日

使用说明：
1、IPO初始化顺序（M0）
(1) 等待系统参数管理模块初始化完成(系统启动时务必初始化坐标系映射表),
(2) 设置系统控制参数读取地址IPO_SetSysParmReadAddr 
(3) 设置插补器缓冲区地址 IPO_SetIPOBufAddr
(4) 设置快速速度规划表存储地址 IPO_SetRapidSpeedCtrlTableAddr
(5) 设置插补器AHB数据交换起始地址 IPO_SetAHBDataTransAddr
(6) 设置插补器依赖的外部接口 IPO_InitInterface
(7) 初始化插补器数据 IPO_InitData
(8) 在SDRAM校验通过的情况下，创建加减速规划表IPO_CreateRapidSpeedCtrlTable
(9) 开启插补中断

2、系统参数修改时应注意以下问题：

*/

//插补器接口函数定义
#ifndef INTERPOLATION_INTERFACE_H
#define INTERPOLATION_INTERFACE_H


//
#include "..\\..\\DataStructDef\\IPO_DataStructDefPub.h"
#include "..\\..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\..\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\..\\DataStructDef\\ERR_DataStructDef.h"

#include "..\\..\\..\\Version\\VER_DataStructDef.h"

/////////////////////////
//#include "..\\IPO_Simulator\\IPO_SimConf.h"

#ifdef IPO_SIMULATOR_WIN

#define IPO_API __declspec(dllexport)

#else

#define IPO_API 

#endif
/////////////////////////


typedef struct _IPO_InterFace {
    //------------------------------------------------------------------------------------------------------------------
    uBit32 (*pf_DEV_SynCycleService)(void);

    //设置轴（物理轴）进给的目标位置
    //uBit32 (*pf_PAX_SendFeed)(uBit32 iAxisNO, double64 dCmdPos);
    uBit32 (*pf_PAX_SendFeed)(uBit32 iAxisNO, IPO_AXIS_STATE_DATA IPOAxisState);

    //获取轴（物理轴）状态
    uBit32 (*pf_PAX_GetStateData)(uBit32 iAxisNO, AXIS_STATE_DATA* pAxisStateData);

    //获取已编号的轴（物理轴）数量
    uBit32 (*pf_PAX_GetRegCount)(void);

    //
    uBit32 (*pf_PAX_LockForIPO)(uBit32 iAxisNO, Bit32 iLockFlag, Bit32 iWaitExeResult);

    //
    uBit32 (*pf_PAX_Verify)(uBit32 ulAxisNO, uBit32 iVerifyFlag, Bit32 iWaitExeResult);

    //
    uBit32 (*pf_SYS_GetAlarmStatus)(void);

    //更新插补器插补中断周期
    uBit32 (*pf_SYS_UpdateIPOTimeCycle)(uBit32 ulTimeCycleUs);



    //------------------------------------------------------------------------------------------------------------------
    //错误管理相关接口
    uBit32 (*pf_ECM_GenerateErrorCode)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);
    uBit32 (*pf_ECM_PutErrorToBuf)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);

    /////////////////////////
    //laomao  2016-05-13 添加
    //设置插补器计时开始
    void (*pf_IPOTick_TimingStart)(void);

    //获取计时开始后所经过的时间(单位：微秒)
    uBit32 (*pf_IPOTick_GetTimePassed)(void);
    /////////////////////////

    ///////////////
    //测试用
    void (*pf_SysTick_CountSysTickStart)(Bit32 iIndex);
    void (*pf_SysTick_CountSysTickEnd)(Bit32 iIndex);
    double64 (*pf_SysTick_GetCycleTime)(Bit32 iIndex);

    ///////////////
    
}IPO_INTERFACE;




#ifdef __cplusplus
extern "C" {
#endif


    //------------------------------------------供系统管理层使用的接口函数----------------------------------------------

    /*
    函 数 名：void IPO_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
                                            const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm)
    功    能：初始化系统参数读取地址
    参    数：pSysCtrlParm --系统控制参数读取地址
              pCrdSysAxisMapTab --坐标系映射表数据读取地址
              pCrdParm --坐标系参数读取地址
              pAxisParm --轴参数读取地址
    返 回 值：无
    调用位置：插补器初始化时，在系统参数管理模块初始化之后调用
    注意事项：无
    */
    IPO_API void IPO_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
                                    const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm);

    /*
    函 数 名：void IPO_SetIPOBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen)
    功    能：设置插补器插补缓冲区地址
    参    数：ulStartAddr --插补缓冲区起始地址
              ulBufLen --插补缓冲区长度
    返 回 值：无
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API void IPO_SetIPOBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen);

    /*
    函 数 名：void IPO_SetRapidSpeedCtrlTableAddr(uBit32 ulStartAddr, uBit32 ulLen)
    功    能：设置快移速度规划表地址
    参    数：ulStartAddr --快移速度规划表数据区起始地址
              ulLen --快移速度规划表数据区长度
    返 回 值：无
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API void IPO_SetRapidSpeedCtrlTableAddr(uBit32 ulStartAddr, uBit32 ulLen);
    
    /*
    函 数 名：void IPO_SetJogPosCtrlTableAddr(uBit32 ulStartAddr, uBit32 ulLen)
    功    能：设置手动位置控制速度规划表地址
    参    数：ulStartAddr --手动位置控制速度规划表数据区起始地址
              ulLen --手动位置控制速度规划表数据区长度
    返 回 值：无
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API void IPO_SetJogPosCtrlTableAddr(uBit32 ulStartAddr, uBit32 ulLen);


    /*
    函 数 名：void IPO_SetAHBDataTransAddr(uBit32 ulAddr)
    功    能：设置AHB数据交换起始地址
    参    数：ulAddr --AHB数据交换起始地址
    返 回 值：无
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API void IPO_SetAHBDataTransAddr(uBit32 ulAddr);

    /*
    函 数 名：uBit32 IPO_InitInterface(IPO_INTERFACE* pIPOInterface)
    功    能：设置插补器依赖的外部接口函数
    参    数：pIPOInterface --外部接口数据地址
    返 回 值：成功返回0，否则返回非0值
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API uBit32 IPO_InitInterface(IPO_INTERFACE* pIPOInterface);

    //
    IPO_API uBit32 IPO_GetDefaultInterface(IPO_INTERFACE* pIPOInterface);


    /*
    函 数 名：void IPO_InitData()
    功    能：初始化插补器数据
    参    数：无
    返 回 值：无
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API void IPO_InitData();


    /*
    函 数 名：int IPO_CreateRapidSpeedCtrlTable()
    功    能：创建各电机轴快移模式速度规划表
    参    数：无
    返 回 值：成功返回0，否则返回非0值
    调用位置：插补器初始化时调用
    注意事项：无
    */
    IPO_API uBit32 IPO_CreateRapidSpeedCtrlTable();

    /*
    函 数 名：uBit32 IPO_CreateAxisRapidSpeedCtrlTable(Bit32 iAxisNO)
    功    能：生成指定轴的快移模式速度规划表
    参    数：iAxisNO ——轴编号，取值从0开始
    返 回 值：成功返回0，否则返回错误码
    调用位置：插补器内部调用，轴参数改变时调用
    注意事项：无
    */
    IPO_API uBit32 IPO_CreateAxisRapidSpeedCtrlTable(Bit32 iAxisNO);
    
    /*
    函 数 名：int IPO_CreateJogPosCtrlTable()
    功    能：创建各电机轴手动位置控制速度规划表
    参    数：无
    返 回 值：成功返回0，否则返回非0值
    调用位置：系统初始化时调用
    注意事项：无
    */
    IPO_API uBit32 IPO_CreateJogPosCtrlTable();
    
    /*
    函 数 名：uBit32 IPO_CreateMotorJogPosCtrlTable(Bit32 iMotorNO)
    功    能：生成指定轴的手动位置控制速度规划表
    参    数：iMotorNO  --轴编号，取值从0开始
    返 回 值：成功返回0，否则返回错误码
    调用位置：插补器内部调用，轴参数改变时调用
    注意事项：无
    */
    IPO_API uBit32 IPO_CreateMotorJogPosCtrlTable(Bit32 iMotorNO);
    

    /*
    函 数 名：uBit32 IPO_UpdateCrdSysAxisMapTable()
    功    能：更新插补器坐标系映射表表数据
    参    数：无
    返 回 值：成功返回0，否则返回错误码
    调用位置：参数管理模块调用
    注意事项：坐标系映射表改变时必须调用本函数
    */
    IPO_API uBit32 IPO_UpdateCrdSysAxisMapTable();

    /*
    函 数 名：uBit32 IPO_UpdateSycCtrlParm()
    功    能：更新插补器系统控制参数（当插补周期改变时，M4需要修改插补中断定时器配置）
    参    数：无
    返 回 值：成功返回0，否则返回错误码
    调用位置：参数管理模块调用
    注意事项：插补周期改变时必须调用本函数
    */
    IPO_API uBit32 IPO_UpdateSycCtrlParm();


    /*
    函 数 名：void IPO_InitIPOBuf()
    功    能：初始化插补缓冲区
    参    数：无
    返 回 值：无
    调用位置：1、插补器内部调用（插补器初始化时调用）,
              2、参数管理模块（当系统配置参数中系统坐标系数量改变时调用）
    注意事项：无
    */
    IPO_API void IPO_InitIPOBuf();


    /*
    函 数 名：void IPO_MainCtrl()
    功    能：插补器主控函数
    参    数：无
    返 回 值：无
    调用位置：在中断进程中调用
    注意事项：
    */
    IPO_API void IPO_MainCtrl();

    /*
    函 数 名：void LKH_LookaheadMainCtrl(void)
    功    能：插补前瞻主控函数
    参    数：无
    返 回 值：无
    调用位置：系统大循环中调用
    注意事项：无
    */
    IPO_API void LKH_LookaheadMainCtrl(void);

    //------------------------------------------外部应用层使用的接口函数----------------------------------------------

    /*
    函 数 名：uBit32 IPO_GetVersion(SOFTWARE_VERSION* pVersion)
    功    能：获取插补器版本信息
    参    数：pVersion --插补器版本信息存储地址（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层
    注意事项：无
    */
    IPO_API uBit32 IPO_GetVersion(SOFTWARE_VERSION* pVersion);

    /*
    函 数 名：uBit32 IPO_Reset(Bit32 iCrdSysIndex);
    功    能：复位插补器
    参    数：iCrdSysIndex --坐标系索引
    返 回 值：成功接收复位指令返回0，否则返回错误码
    调用位置：
    注意事项：本函数调用成功，不表示复位完成
    */
    IPO_API uBit32 IPO_Reset(Bit32 iCrdSysIndex);

    /*
    函 数 名：Bit32 IPO_CheckAxisStop(Bit32 iCrdSysIndex, Bit32 iAxisIndex)
    功    能：检测坐标轴是否运动停止
    参    数：iCrdSysIndex --坐标系索引
              iAxisIndex --坐标轴索引
    返 回 值：运动停止返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API Bit32 IPO_CheckAxisStop(Bit32 iCrdSysIndex, Bit32 iAxisIndex);
    
    /*
    函 数 名：int IPO_CheckSysFree();
    功    能：检测系统是否空闲（运动停止且无程序或其他运动指令在执行）
    参    数：无
    返 回 值：非空闲返回1，空闲返回0
    调用位置：
    注意事项：无
    */
    IPO_API Bit32 IPO_CheckSysFree();

    /*
    函 数 名：Bit32 IPO_CheckCrdSysFree(Bit32 iCrdSysIndex)
    功    能：检测坐标系是否空闲（运动停止且无程序或其他运动指令在执行）
    参    数：iCrdSysIndex --坐标系索引
    返 回 值：空闲返回1，否则返回0
    调用位置：
    注意事项：无
    */
    IPO_API Bit32 IPO_CheckCrdSysFree(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 IPO_GetStateData(Bit32 iCrdSysIndex, IPO_STATE_DATA* pIPOStateData)
    功    能：获取坐标系状态数据
    参    数：iCrdSysIndex --坐标系索引
              *pCrdSysStateData --坐标系状态数据（出参）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API uBit32 IPO_GetStateData(Bit32 iCrdSysIndex, IPO_STATE_DATA* pIPOStateData);

    /*
    函 数 名：uBit32 JOG_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);
    功    能：设置多个坐标轴按位置控制方式运动
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --坐标轴掩码
              pPosCtrlMotion --位置控制运动数据起始地址（有多少个轴，就需要有多少个运动数据）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：
    */
    IPO_API uBit32 JOG_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);


    /*
    函 数 名：uBit32 JOG_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);
    功    能：设置多个坐标轴按速度控制方式运动
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --坐标轴掩码
              pSpeedCtrlMotion --速度控制运动数据起始地址（有多少个轴，就需要有多少个运动数据）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：
    */
    IPO_API uBit32 JOG_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);

    /*
    函 数 名：uBit32 JOG_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion)
    功    能：设置电机按位置控制方式运动
    参    数：iMotorNO --电机编号
              *pPosCtrlMotion --位置控制运动参数
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：
    */
    IPO_API uBit32 JOG_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);

    /*
    函 数 名：uBit32 JOG_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
    功    能：设置电机按速度控制方式运动
    参    数：iMotorNO --电机编号
              *pPosCtrlMotion --位置控制运动参数
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：
    */
    IPO_API uBit32 JOG_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);

    /*
    函 数 名：uBit32 JOG_SetMotionStop(uBit32 ulMotorMask)
    功    能：设置电机手动运动停止
    参    数：ulMotorMask --要执行手动运动停止的电机掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API uBit32 JOG_SetMotionStop(uBit32 ulMotorMask);

    /*
    函 数 名：void JOG_MotorEStop(uBit32 ulMotorMask)
    功    能：电机轴手动运动急停
    参    数：ulMotorMask --要急停的电机掩码
    返 回 值：无
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API void JOG_MotorEStop(uBit32 ulMotorMask);
        
    /*
    函 数 名：void JOG_AxisEStop(Bit32 iCrdSysIndex,  uBit32 ulAxisMask)
    功    能：坐标轴手动运动急停
    参    数：iCrdSysIndex --坐标系索引
              ulAxisMask --执行手动急停的坐标轴掩码
    返 回 值：无
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API void JOG_AxisEStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask);

    /*
    函 数 名：uBit32 IPO_SetCtrlMode(Bit32 iCrdSysIndex, uBit32 ulMode)
    功    能：设置插补器控制模式
    参    数：iCrdSysIndex --坐标系索引
              ulMode --控制模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：设置模式时不在运动状态，且没有控制指令在执行
    */
    IPO_API uBit32 IPO_SetCtrlMode(Bit32 iCrdSysIndex, uBit32 ulMode);

    /*
    函 数 名：uBit32 IPO_SetVerifyEnable(Bit32 iCrdSysIndex, Bit32 iEnable)
    功    能：打开或关闭校验运行模式
    参    数：iCrdSysIndex --坐标系索引
              iEnable --校验模式开关标记，取值：1-打开校验模式，0-关闭校验模式
    返 回 值：成功返回0，否则返回非0值
    调用位置：
    注意事项：坐标系程序未停止或手动运行未停止时，不能设置该模式
    */
    IPO_API uBit32 IPO_SetVerifyEnable(Bit32 iCrdSysIndex, Bit32 iEnable);

    //
    IPO_API uBit32 IPO_EnableSingleSegmentMode(Bit32 iCrdSysIndex, Bit32 iEnable);


    /*
    函 数 名：uBit32 IPO_AddMotionDataToIPOBuf(Bit32 iCrdSysIndex, SEGMENT_MOTION_DATA* pMotionData)
    功    能：添加运动段数据到插补缓冲区
    参    数：iCrdSysIndex --坐标系索引
              *pMotionData --运动段数据
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层
    注意事项：
    */
    IPO_API uBit32 IPO_AddMotionDataToIPOBuf(Bit32 iCrdSysIndex, SEGMENT_MOTION_DATA* pMotionData);

    /*
    函 数 名：void IPO_GetBufState(Bit32 iCrdSysIndex, DATA_BUF_MAN* pIPOBufState)
    功    能：获取插补缓冲区状态数据
    参    数：iCrdSysIndex --坐标系索引
              *pIPOBufState --插补缓冲区状态数据（出参）
    返 回 值：无
    调用位置：应用层
    注意事项：无
    */
    IPO_API void IPO_GetBufState(Bit32 iCrdSysIndex, DATA_BUF_MAN* pIPOBufState);


    /*
    函 数 名：uBit32 IPO_ProgRun(uBit16 nCrdSysMask)
    功    能：设置插补器循环启动（运动程序开始执行）
    参    数：nCrdSysMask --循环启动的坐标系掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    IPO_API uBit32 IPO_ProgRun(uBit16 nCrdSysMask);

    /*
    函 数 名：int IPO_ProgStop(int nCrdSysMask);
    功    能：设置插补器循环停止
    参    数：nCrdSysMask --执行循环停止的坐标系掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    IPO_API uBit32 IPO_ProgStop(uBit16 nCrdSysMask);

    /*
    函 数 名：void IPO_ProgEStop(unsigned short nCrdSysMask)
    功    能：设置插补器程序急停
    参    数：nCrdSysMask --执行程序急停的坐标系掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    IPO_API uBit32 IPO_ProgEStop(uBit16 nCrdSysMask);
      
    /*
    函 数 名：int IPO_ProgSegmentEndStop(int nCrdSysMask);
    功    能：设置当前段末尾结束执行停止
    参    数：nCrdSysMask --执行程序段末尾停止的坐标系掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    IPO_API uBit32 IPO_ProgSegmentEndStop(uBit16 nCrdSysMask);

    /*
    函 数 名：uBit32 IPO_ProgHold(uBit16 nCrdSysMask)
    功    能：设置插补器程序暂停
    参    数：nCrdSysMask --执行程序暂停的
    返 回 值：成功返回0，否则返回非0值
    调用位置：
    注意事项：无
    */
    IPO_API uBit32    IPO_ProgHold(uBit16 nCrdSysMask);

    /*
    函 数 名：uBit32 IPO_SetCrdSysFeedOverride(Bit32 iCrdSysIndex, float32 dOverride)
    功    能：设置坐标系进给修调
    参    数：iCrdSysIndex --坐标系索引
              dOverride --进给修调
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API uBit32 IPO_SetCrdSysFeedOverride(Bit32 iCrdSysIndex, float32 dOverride);

    /*
    函 数 名：uBit32 IPO_SetCrdSysFeedOverride(Bit32 iCrdSysIndex, float32 dOverride)
    功    能：设置坐标系进给修调
    参    数：iCrdSysIndex --坐标系索引
              dOverride --进给修调
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    IPO_API uBit32 IPO_SetCrdSysRapidOverride(Bit32 iCrdSysIndex, float32 dOverride);

    /*
    函 数 名：void IPO_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask);
    功    能：设置坐标系的指定轴停止发脉冲
    参    数：iCrdSysIndex --坐标系索引
              ulAxisMask --需要关闭的坐标轴掩码，bit0-bit7分别对应坐标轴1-8
    返 回 值：无
    调用位置：应用层调用
    注意事项：
    */
    IPO_API void IPO_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask);


    
#ifdef __cplusplus
}
#endif



#endif
