
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_Interface.h

摘    要：通道管理模块接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月10日

使用说明：
1、通道管理模块初始化顺序
(1) 设置系统控制参数读取地址 CSM_SetSysParmReadAddr 
(2) 设置程序缓冲区地址 CSM_SetProgBufAddr
(3) 设置用户自定义数据缓冲区地址 CSM_SetUserBufAddr
(4) 设置通道依赖的外部接口 CSM_InitInterface
(5) 初始化通道数据CSM_InitData
*/

//插补器接口函数定义
#ifndef CSM_INTERFACE_H
#define CSM_INTERFACE_H

#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"
#include "..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\DataStructDef\\IPR_DataStructDef.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"
#include "..\\DataStructDef\\MST_DataStructDef.h"

#include "..\\..\\Version\\VER_DataStructDef.h"


//
typedef struct _CSM_InterFace {

    //获取轴（物理轴）状态
    uBit32 (*pf_PAX_GetStateData)(uBit32 iAxisNO, AXIS_STATE_DATA* pAxisStateData);
    uBit32 (*pf_PAX_GetRunningStatus)(uBit32 ulAxisNO);
    uBit32 (*pf_PAX_GetRegCount)(void);
    uBit32 (*pf_PAX_Enable)(uBit32 iAxisNO, Bit32 iEnable, Bit32 iWaitExeResult);
    uBit32 (*pf_PAX_Home)(uBit32 iAxisNO, Bit32 iHomeFlag, Bit32 iWaitExeResult);
    uBit32 (*pf_PAX_SetCmdPos)(uBit32 iAxisNO, Bit32 lCmdPos, Bit32 iWaitExeResult);
    uBit32 (*pf_PAX_SetQeiPos)(uBit32 iAxisNO, Bit32 lQeiPos, Bit32 iWaitExeResult);
    
    uBit32 (*pf_DEV_CheckLastCmdExeState)(uBit32 ulDevNO, uBit32* pCmdType, uBit32 *pExeRes);    

    //--------------------------------------------------------------------------------------------
    //插补器状态
    uBit32 (*pf_IPO_Reset)(Bit32 iCrdSysIndex);
    uBit32 (*pf_IPO_SetCtrlMode)(Bit32 iCrdSysIndex, uBit32 ulMode);
    uBit32 (*pf_IPO_GetStateData)(Bit32 iCrdSysIndex, IPO_STATE_DATA* pIPOStateData);
    void   (*pf_IPO_GetBufState)(Bit32 iCrdSysIndex, DATA_BUF_MAN* pIPOBufState);

    //插补器自动模式相关指令
    uBit32 (*pf_IPO_SetCrdSysFeedOverride)(Bit32 iCrdSysIndex, float32 dOverride);
    uBit32 (*pf_IPO_SetCrdSysRapidOverride)(Bit32 iCrdSysIndex, float32 dOverride);
    uBit32 (*pf_IPO_SetVerifyEnable)(Bit32 iCrdSysIndex, Bit32 iEnable);
    uBit32 (*pf_IPO_EnableSingleSegmentMode)(Bit32 iCrdSysIndex, Bit32 iEnable);
    uBit32 (*pf_IPO_ProgRun)(uBit16 nCrdSysMask);
    uBit32 (*pf_IPO_ProgStop)(uBit16 nCrdSysMask);
    uBit32 (*pf_IPO_ProgEStop)(uBit16 nCrdSysMask);
    uBit32 (*pf_IPO_ProgSegmentEndStop)(uBit16 nCrdSysMask);
    uBit32 (*pf_IPO_ProgHold)(uBit16 nCrdSysMask);
    uBit32 (*pf_IPO_AddMotionDataToIPOBuf)(Bit32 iCrdSysIndex, SEGMENT_MOTION_DATA* pMotionData);
    void   (*pf_IPO_CloseAxisOutput)(Bit32 iCrdSysIndex, uBit32 ulAxisMask);

    //插补器手动模式相关指令
    uBit32 (*pf_JOG_SetAxisPosCtrlMotion)(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);
    uBit32 (*pf_JOG_SetAxisSpeedCtrlMotion)(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);
    uBit32 (*pf_JOG_SetMotorPosCtrlMotion)(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);
    uBit32 (*pf_JOG_SetMotorSpeedCtrlMotion)(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);
    uBit32 (*pf_JOG_SetMotionStop)(uBit32 ulMotorMask);
    void   (*pf_JOG_MotorEStop)(uBit32 ulMotorMask);

    //
    uBit32 (*pf_MST_Reset)(Bit32 iCrdSysIndex);
    void   (*pf_MST_Run)(uBit16 nCrdSysMask);
    void   (*pf_MST_Stop)(uBit16 nCrdSysMask);
    void   (*pf_MST_Cancel)(uBit16 nCrdSysMask);
    uBit32 (*pf_MST_ProcessBeforeActionMST)(Bit32 iCrdSysIndex, IIP_BUF* pProgData);
    uBit32 (*pf_MST_ProcessAfterActionMST)(Bit32 iCrdSysIndex, IIP_BUF* pProgData);
    Bit32  (*pf_MST_CheckBeforeActionMSTExist)(IIP_BUF* pProgData);
    Bit32  (*pf_MST_CheckAfterActionMSTExist)(IIP_BUF* pProgData);
    uBit32 (*pf_MST_GetStateData)(Bit32 iCrdSysIndex, MST_STATE_DATA* pMSTStateData);


    //系统状态管理模块相关接口
    uBit32 (*pf_SYS_GetAlarmStatus)(void);


    //------------------------------------------------------------------------------------------------------------------
    //错误管理相关接口
    uBit32 (*pf_ECM_GenerateErrorCode)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);
    uBit32 (*pf_ECM_PutErrorToBuf)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);


}CSM_INTERFACE;




#ifdef __cplusplus
extern "C" {
#endif


    //------------------------------------------供系统管理层使用的接口函数----------------------------------------------
    /*
    函 数 名：void CSM_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
                                                const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm);
    功    能：设置系统参数访问地址
    参    数：pSysCtrlParm --系统控制参数访问地址
              pCrdSysAxisMapTab --坐标系映射表访问地址
              pCrdParm --坐标系参数访问地址
              pAxisParm --轴参数访问地址
    返 回 值：无
    调用位置：通道管理模块初始化时调用
    注意事项：无
    */
    void CSM_SetSysParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm, const CRDSYS_AXIS_MAP_TABLE* pCrdSysAxisMapTab,
        const CRDSYS_PARM* pCrdParm, const AXIS_PARM* pAxisParm);

    /*
    函 数 名：void CSM_SetProgBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen);
    功    能：设置程序缓冲区地址
    参    数：ulStartAddr --程序缓冲区起始地址
              ulBufLen --程序缓冲区长度，单位：字节
    返 回 值：无
    调用位置：通道管理模块初始化时调用
    注意事项：无
    */
    void CSM_SetProgBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen);

    /*
    函 数 名：void CSM_SetUserBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen);
    功    能：设置用户自定义数据缓冲区地址
    参    数：ulStartAddr --用户自定义数据缓冲区起始地址
              ulBufLen --用户自定义数据缓冲区长度，单位：字节
    返 回 值：无
    调用位置：通道管理模块初始化时调用
    注意事项：无
    */
    void CSM_SetUserBufAddr(uBit32 ulStartAddr, uBit32 ulBufLen);

    /*
    函 数 名：uBit32 CSM_InitInterface(CSM_INTERFACE* pCSMInterface)
    功    能：初始化通道管理模块依赖的外部接口
    参    数：pCSMInterface --通道管理模块依赖的外部接口信息
    返 回 值：成功返回0，否则返回错误码
    调用位置：通道管理模块初始化时调用
    注意事项：无
    */
    uBit32 CSM_InitInterface(CSM_INTERFACE* pCSMInterface);

    /*
    函 数 名：void CSM_InitData()
    功    能：初始化通道数据
    参    数：无
    返 回 值：无
    调用位置：通道管理模块初始化时调用
    注意事项：无
    */
    void CSM_InitData();

    /*
    函 数 名：void CSM_MainCtrl()
    功    能：通道管理主控函数
    参    数：无
    返 回 值：无
    调用位置：在大循环中调用
    注意事项：
    */
    void CSM_MainCtrl();

    /*
    函 数 名：uBit32 CSM_GetVersion(SOFTWARE_VERSION* pVersion)
    功    能：获取通道管理模块版本信息
    参    数：无
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 CSM_GetVersion(SOFTWARE_VERSION* pVersion);


    //---------------------------------------供系统参数管理模块使用的接口函数---------------------------------------------
    /*
    函 数 名：void CSM_InitProgBuf()
    功    能：初始化程序缓冲区
    参    数：无
    返 回 值：无
    调用位置：1、通道管理模块内部调用（通道管理模块初始化时调用）,
              2、参数管理模块（当系统配置参数中系统坐标系数量改变时调用）
    注意事项：无
    */
    void CSM_InitProgBuf();

    /*
    函 数 名：void CSM_InitUserBuf()
    功    能：初始化用户自定义数据缓冲区
    参    数：无
    返 回 值：无
    调用位置：1、通道管理模块内部调用（通道管理模块初始化时调用）,
              2、参数管理模块（当系统配置参数中系统坐标系数量改变时调用）
    注意事项：无
    */
    void CSM_InitUserBuf();

    uBit32 CSM_UpdateCrdSysAxisMapTable();


    //-------------------------------------------手动运动指令接口函数-------------------------------------------------
    /*
    函 数 名：uBit32 CSM_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);
    功    能：设置多个坐标轴按位置控制方式运动
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --坐标轴掩码
              pPosCtrlMotion --位置控制运动数据起始地址（有多少个轴，就需要有多少个运动数据）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：
    */
    uBit32 CSM_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);

    /*
    函 数 名：uBit32 CSM_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);
    功    能：设置多个坐标轴按速度控制方式运动
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --坐标轴掩码
              pSpeedCtrlMotion --速度控制运动数据起始地址（有多少个轴，就需要有多少个运动数据）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：
    */
    uBit32 CSM_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);

    /*
    函 数 名：uBit32 CSM_SetAxisJogStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
    功    能：设置坐标轴手动运动停止
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --执行手动运动停止的坐标轴掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_SetAxisJogStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask);


    /*
    函 数 名：uBit32 CSM_AxisJogEStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
    功    能：坐标轴手动运动急停
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --执行手动急停的坐标轴掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置： 
    注意事项：无
    */
    uBit32 CSM_AxisJogEStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask);

    /*
    函 数 名：void CSM_CrdSysJogEStop(Bit32 iCrdSysIndex)
    功    能：通道手动运动急停
    参    数：无
    返 回 值：无
    调用位置： 
    注意事项：无
    */
    void CSM_CrdSysJogEStop(Bit32 iCrdSysIndex);
    

     /*
    函 数 名：uBit32 CSM_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);
    功    能：设置电机按位置控制方式运动
    参    数：iMotorNO --电机编号
              *pPosCtrlMotion --位置控制运动数据
    返 回 值：成功返回0，否则返回错误码
    调用位置： 
    注意事项：无
    */
    uBit32 CSM_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);

    /*
    函 数 名：uBit32 CSM_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pPosCtrlMotion);
    功    能：设置电机按速度控制方式运动
    参    数：iMotorNO --电机编号
              *pSpeedCtrlMotion --速度控制运动数据
    返 回 值：成功返回0，否则返回错误码
    调用位置： 
    注意事项：无
    */
    uBit32 CSM_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);

    /*
    函 数 名：uBit32 CSM_SetMotorJogStop(Bit32 iMotorNO)
    功    能：设置电机手动运动停止
    参    数：iMotorNO --电机编号
    返 回 值：成功返回0，否则返回错误码
    调用位置： 
    注意事项：无
    */
    uBit32 CSM_SetMotorJogStop(Bit32 iMotorNO);


    /*
    函 数 名：uBit32 CSM_MotorJogEStop(Bit32 iMotorNO)
    功    能：设置电机手动运动急停
    参    数：iMotorNO --电机编号
    返 回 值：成功返回0，否则返回错误码
    调用位置： 
    注意事项：无
    */
    uBit32 CSM_MotorJogEStop(Bit32 iMotorNO);


    //-------------------------------------------自动运动指令接口函数-------------------------------------------------
    /*
    函 数 名：uBit32 CSM_ProgRun(uBit16 nCrdSysMask);
    功    能：设置通道程序开始运行（循环启动）
    参    数：nCrdSysMask --循环启动的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_ProgRun(uBit16 nCrdSysMask);

    uBit32 CSM_ProgRun_IPR(Bit32 iCrdSysIndex);


    /*
    函 数 名：uBit32 CSN_ProgStop(uBit16 nCrdSysMask)
    功    能：设置通道程序运行停止（循环停止）
    参    数：nCrdSysMask --循环停止的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_ProgStop(uBit16 nCrdSysMask);

    uBit32 CSM_ProgStop_IPR(Bit32 iCrdSysIndex);


    /*
    函 数 名：uBit32 CSM_ProgEStop(uBit16 nCrdSysMask);
    功    能：设置通道程序急停
    参    数：nCrdSysMask --执行程序急停的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_ProgEStop(uBit16 nCrdSysMask);
      
    /*
    函 数 名：uBit32    CSM_ProgSegmentEndStop(uBit16 nCrdSysMask)
    功    能：设置当前段末尾结束执行停止
    参    数：nCrdSysMask --执行程序段末尾停止的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32    CSM_ProgSegmentEndStop(uBit16 nCrdSysMask);

    /*
    函 数 名：uBit32 CSM_ProgHold(uBit16 nCrdSysMask);
    功    能：设置通道程序暂停
    参    数：nCrdSysMask --执行程序暂停的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32    CSM_ProgHold(uBit16 nCrdSysMask);
    uBit32    CSM_ProgHold_IPR(uBit16 iCrdSysIndex);
    
    /*
    函 数 名：uBit32 CSM_ProgCancel(uBit16 nCrdSysMask);
    功    能：设置通道程序取消
    参    数：nCrdSysMask --执行程序取消的通道掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_ProgCancel(uBit16 nCrdSysMask);

    /*
    函 数 名：uBit32 CSM_SetFeedOverride(Bit32 iCrdSysIndex, float32 dOverride)
    功    能：设置通道进给修调
    参    数：iCrdSysIndex --通道索引
              dOverride --进给修调，取值0.0-1.0
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_SetFeedOverride(Bit32 iCrdSysIndex, float32 dOverride);

    /*
    函 数 名：uBit32 CSN_SetRapidOverride(Bit32 iCrdSysIndex, float32 dOverride)
    功    能：设置通道快移修调
    参    数：iCrdSysIndex --通道索引
              dOverride --快移修调，取值0.0-1.0
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_SetRapidOverride(Bit32 iCrdSysIndex, float32 dOverride);

    /*
    函 数 名：uBit32 CSM_SetSpindleOverride(Bit32 iCrdSysIndex, float32 dOverride)
    功    能：设置通道主轴修调
    参    数：iCrdSysIndex --通道索引
              dOverride --主轴修调，取值0.0-1.0
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：无
    */
    uBit32 CSM_SetSpindleOverride(Bit32 iCrdSysIndex, float32 dOverride);


    /*
    函 数 名：void CSM_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
    功    能：关闭通道中指定坐标轴的脉冲输出
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --关闭脉冲输出的坐标轴掩码
    返 回 值：无
    调用位置：
    注意事项：无
    */
    void CSM_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask);


    /*
    函 数 名：uBit32 CSM_MoveAxis(Bit32 iCrdSysIndex, Bit32* pAxisPos, Bit32 iAxisCount, Bit32 iSpeed);
    功    能：将坐标轴移动到指定位置
    参    数：iCrdSysIndex --通道索引
              pAxisPos --坐标轴目标位置起始地址
              iAxisCount --轴数量
              iSpeed --运动速度，单位：um/ms
    返 回 值：成功返回0，否则返回错误码
    调用位置：供解释器调用
    注意事项：该函数必须在自动模式下调用，函数内部以G01的方式完成运动
    */
    uBit32 CSM_MoveAxis(Bit32 iCrdSysIndex, Bit32* pAxisPos, Bit32 iAxisCount, Bit32 iSpeed);


    //
    uBit32 CSM_SpindleRun(Bit32 iCrdSysIndex, Bit32 iDir, uBit32 ulSpeed);
    uBit32 CSM_SpindleStop(Bit32 iCrdSysIndex);
    
    //-------------------------------------------通道状态控制指令接口函数-------------------------------------------------

    /*
    函 数 名：uBit32 CSM_Reset(Bit32 iCrdSysIndex)
    功    能：通道复位
    参    数：iCrdSysIndex --通道索引
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_Reset(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSM_SetCtrlMode(Bit32 iCrdSysIndex, uBit32 ulMode);
    功    能：设置通道控制模式
    参    数：iCrdSysIndex --通道索引
              ulMode --控制模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_SetCtrlMode(Bit32 iCrdSysIndex, uBit32 ulMode);


    /*
    函 数 名：uBit32 CSM_EnableVerify(Bit32 iCrdSysIndex, Bit32 iEnable)
    功    能：打开或关闭校验运行模式
    参    数：iCrdSysIndex --通道索引
              iEnable --1-打开校验模式，0-关闭校验模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：坐标系程序未停止或手动运行未停止时，不能设置该模式
    */
    uBit32 CSM_EnableVerify(Bit32 iCrdSysIndex, Bit32 iEnable);

    //打开或关闭程序单段执行模式
    uBit32 CSM_EnableSingleSegmentMode(Bit32 iCrdSysIndex, Bit32 iEnable);

    //uBit32 CSM_EnableLookahead(Bit32 iCrdSysIndex, Bit32 iEnable);

    //-------------------------------------------通道状态查询指令接口函数-------------------------------------------------
    /*
    函 数 名：uBit32 CSM_GetCrdSysStateData(Bit32 iCrdSysIndex, CRDSYS_STATE_DATA* pCrdSysStateData)
    功    能：获取通道状态数据
    参    数：iCrdSysIndex --通道索引
              *pCrdSysStateData --通道状态数据（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_GetCrdSysStateData(Bit32 iCrdSysIndex, CRDSYS_STATE_DATA* pCrdSysStateData);

    const CRDSYS_STATE_DATA* CSM_GetCrdSysStateReadAddr(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSM_GetCrdSysStatus(Bit32 iCrdSysIndex)
    功    能：获取通道运行状态字
    参    数：iCrdSysIndex --通道索引
    返 回 值：通道运行状态字
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_GetCrdSysStatus(Bit32 iCrdSysIndex);

    /*
    函 数 名：void CSM_GetCurLineTargetPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount)
    功    能：获取当前运动段的目标位置
    参    数：iCrdSysIndex --通道索引
              pdPos --目标位置存放起始地址
              iAxisCount --轴数量（表示pdPos地址可存放多少个轴的目标位置）
    返 回 值：无
    调用位置：应用层调用
    注意事项：无
    */
    void CSM_GetCurLineTargetPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount);

    /*
    函 数 名：void CSM_GetCmdPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount)
    功    能：获取通道当前指令位置
    参    数：iCrdSysIndex --通道索引
              pdPos --指令位置存放起始地址
              iAxisCount --轴数量（表示pdPos地址可存放多少个轴的指令位置）
    返 回 值：无
    调用位置：应用层调用
    注意事项：无
    */
    void CSM_GetCmdPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount);
  
    /*
    函 数 名：int CSM_CheckSysFree(int iCrdSysIndex);
    功    能：检测通道是否空闲（运动停止且无程序或其他运动指令在执行）
    参    数：iCrdSysIndex --通道索引
    返 回 值：空闲返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckCrdSysFree(Bit32 iCrdSysIndex);


    /*
    函 数 名：Bit32 CSM_CheckSysFree()
    功    能：检测所有通道是否空闲
    参    数：无 
    返 回 值：空闲返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckSysFree();

    //检查是否存在通道报警，存在报警返回1，否则返回0
    Bit32 CSM_CheckSysAlarm();

    /*
    函 数 名：Bit32 CSM_CheckEStop(Bit32 iCrdSysIndex)
    功    能：检测通道是否急停（存在不可恢复的错误）
    参    数：iCrdSysIndex --通道索引
    返 回 值：通道急停返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckEStop(Bit32 iCrdSysIndex);

    /*
    函 数 名：Bit32 CSM_CheckResetState(Bit32 iCrdSysIndex)
    功    能：检测通道是否处于复位状态
    参    数：iCrdSysIndex --通道索引
    返 回 值：正在复位返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckResetState(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSM_CheckAxisMoveState(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
    功    能：检测通道中指定的轴是否在运动
    参    数：iCrdSysIndex --通道索引
              ulAxisMask --待检测是否存在运动的坐标轴掩码
    返 回 值：返回值中对应位为0表示没有轴在移动，对应位非零表示还在运动
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_CheckAxisMoveState(Bit32 iCrdSysIndex, uBit32 ulAxisMask);

    /*
    函 数 名：Bit32 CSM_CheckProgCancel(Bit32 iCrdSysIndex)
    功    能：检测程序是否取消
    参    数：iCrdSysIndex --通道索引
    返 回 值：程序取消返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckProgCancel(Bit32 iCrdSysIndex);

    /*
    函 数 名：Bit32 CSM_CheckProgRun(Bit32 iCrdSysIndex)
    功    能：检测通道是否循环启动
    参    数：iCrdSysIndex --通道索引
    返 回 值：通道已循环启动返回1，否则返回0
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_CheckProgRun(Bit32 iCrdSysIndex);

    /*
    函 数 名：Bit32 CSM_GetSkipSetting(Bit32 iCrdSysIndex)
    功    能：获取当前跳段设置
    参    数：iCrdSysIndex --通道索引
    返 回 值：跳段设置
    调用位置：应用层调用
    注意事项：无
    */
    Bit32 CSM_GetSkipSetting(Bit32 iCrdSysIndex);

    /*
    函 数 名：Bit32 CSM_GetStartLineCount(Bit32 iCrdSysIndex)
    功    能：获取程序运行开始的行数量
    参    数：iCrdSysIndex --通道索引
    返 回 值：开始的行数量
    调用位置：解释器
    注意事项：无
    
    Bit32 CSM_GetStartLineCount(Bit32 iCrdSysIndex);*/

    /*
    函 数 名：Bit32 CSM_GetFinishLineCount(Bit32 iCrdSysIndex)
    功    能：获取程序运行结束的行数量
    参    数：iCrdSysIndex --通道索引
    返 回 值：结束的行数量
    调用位置：解释器
    注意事项：无
    
    Bit32 CSM_GetFinishLineCount(Bit32 iCrdSysIndex);*/
    
    /*
    函 数 名：Bit32 CSM_GetExeLineCount(Bit32 iCrdSysIndex)
    功    能：获取已经执行的程序段起始及结束行号
    参    数：iCrdSysIndex        --通道索引
              pStartLineCount    --起始行
              pFinishLineCount    --结束行
    返 回 值：0--成功 非0-错误码
    调用位置：解释器
    注意事项：无
    */
    Bit32 CSM_GetExeLineCount(Bit32 iCrdSysIndex, Bit32 *pStartLineCount, Bit32 *pFinishLineCount);

    /*
    函 数 名：Bit32 CSM_GetSpindleQeiSense(Bit32 iCrdSysIndex)
    功    能：获取主轴编码器分辨率
    参    数：iCrdSysIndex --通道索引
    返 回 值：主轴编码器分辨率
    调用位置：解释器
    注意事项：无
    */
    Bit32 CSM_GetSpindleQeiSense(Bit32 iCrdSysIndex);

    //(供解释器使用)
    Bit32 CSM_GetEnable(Bit32 iCrdSysIndex);
    uBit32* CSM_GetNcAxisAddr(Bit32 iCrdSysIndex);
    uBit32* CSM_GetUserinAddr(Bit32 iCrdSysIndex);
    uBit32* CSM_GetUserOutAddr(Bit32 iCrdSysIndex);

    uBit32* CSM_GetNcAxisAddr(Bit32 iCrdSysIndex);

    //----------------------------------------------程序段数据操作接口----------------------------------------------

    /*
    函 数 名：uBit32 CSM_ProgDwell(uBit32 ulCrdNO, uBit32 ulDelayMs)
    功    能：下载坐标系延时程序段
    参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
              ulDelayMs    -- 延时时间，单位ms
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgDwell(uBit32 ulCrdNO, uBit32 ulDelayMs);

    /*
    函 数 名：uBit32 CSM_ProgMCode(uBit32 ulCrdNO, uBit16 nMCode)
    功    能：下载M代码程序段
    参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
              nMCode    -- M指令码 如M02时值为2
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgMCode(uBit32 ulCrdNO, uBit16 nMCode);

    /*
    函 数 名：uBit32 UIN_MCode(uBit32 ulCrdNO, uBit16 ulSCode)
    功    能：下载S代码程序段
    参    数：ulCrdNO -- 坐标系编号, 取值从0开始
              ulSCode -- S指令码
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgSCode(uBit32 ulCrdNO, uBit32 ulSCode);

    /*
    函 数 名：uBit32 CSM_ProgTCode(uBit32 ulCrdNO, uBit16 cTCode)
    功    能：下载T代码程序段
    参    数：ulCrdNO -- 坐标系编号,从0开始
              cTCode  -- T指令码 T1时为1
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgTCode(uBit32 ulCrdNO, uBit8 cTCode);

    /*
    函 数 名：uBit32 UIN_BCode(uBit32 ulCrdNo, uBit8 cBCode)
    功    能：下载B代码程序段
    参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
              cBCode    -- B指令码
    返 回 值：0---成功 非0--错误码
    注意事项：
    */
    uBit32 CSM_ProgBCode(uBit32 ulCrdNO, uBit8 cBCode);

    /*
    函 数 名：CSM_ProgIOCode(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs)
    功    能：下载IO输出指令程序段
    参    数：ulCrdNo        -- 坐标系编号,从0开始
              ulDeviceNo    -- IO设备编号
              ulIOMask        -- 输出口屏蔽码
              ulIOState        -- 输出值
              ulDelayMs        -- 延时时间 单位ms
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgIOCode(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs);

    /*
    函 数 名：uBit32 CSM_ProgRapid(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag)
    功    能：单个轴或者多个轴同时执行快速移动(G00)
    参    数：ulCrdNO        -- 坐标系编号,从0开始
              ulAxisMask    -- 轴屏蔽字
              pPos            -- 轴目标位置数据地址（依次存放与ulAxisMask中的有效位相对应的轴目标位置）
              bAbsFlag        -- 绝对编程标志    false--相对编程 ture-绝对编程
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgRapid(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag);

    /*
    函 数 名：uBit32 CSM_ProgLine(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag)
    功    能：多轴直线插补运动
    参    数：ulCrdNO    -- 坐标系编号,从0开始
              ulAxisMask    -- 轴屏蔽字
              pPos            -- 轴目标位置数据地址（依次存放与ulAxisMask中的有效位相对应的轴目标位置）
              fSpeed        -- 合成速度，单位：用户计数单位/ms
              bAbsFlag        -- 绝对编程标志    false--相对编程 ture-绝对编程
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgLine(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag);

    /*
    函 数 名：uBit32 UIN_ArcC(uBit32 ulCrdNo, uBit32* pAxisNo,  Bit32 *pEndPos, Bit32* pCenter,bool bClockWise, float32 fSpeed,bool bAbsFlag)
    功    能：两轴圆弧插补(终点和圆心)
    参    数：ulCrdNo                -- 坐标系编号,取值从0开始
              pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                       从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
              pEndPos                -- 圆弧终点坐标数据地址
              pCenter                -- 圆弧圆心坐标数据地址(圆心坐标为相对于圆弧起点的增量值)
              bClockWise            -- 圆弧方向， ture--顺时钟插补 false--逆时针插补
              fSpeed                -- 插补合成速度, 单位：用户计数单位/ms
              bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
    返 回 值：成功返回0，否则返回错误码
    注意事项：
    */
    uBit32 CSM_ProgArcC(uBit32 ulCrdNO, uBit32* pAxisNO,  Bit32 *pEndPos, Bit32* pCenter, BooLean bClockWise, float32 fSpeed, BooLean bAbsFlag);

    /*
    函 数 名：uBit32 UIN_ArcR(uBit32 ulCrdNo, uBit32* pAxisNo, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed,BooLean bAbsFlag)
    功    能：两轴圆弧插补(终点和半径）
    参    数：ulCrdNo                -- 坐标系编号,从0开始
              pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                       从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
              pEndPos                -- 圆弧终点坐标数据地址
              iRadius                -- 半径，半径大于0，表示圆弧小于或等于180度；半径小于0，表示圆弧大于180度
              bClockWise            -- 顺时针插补标志 ture--顺时钟插补 false--逆时针插补
              fSpeed                -- 合成速度, 单位：用户计数单位/ms
              bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
    返 回 值：成功返回0，否则返回错误码
    注意事项：半径方式不能描述整圆
    */
    uBit32 CSM_ProgArcR(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed, BooLean bAbsFlag);

    /*
    函 数 名：uBit32 UIN_ArcPT(uBit32 ulCrdNo, uBit32* pAxisNo, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag)
    功    能：两轴圆弧插补(终点和中间点）
    参    数：ulCrdNo                -- 坐标系编号,从0开始
              pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                       从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
              pEndPos                -- 圆弧终点坐标数据地址
              pMidPos                -- 圆弧中间点坐标数据地址
              fSpeed                -- 合成速度, 单位：用户计数单位/ms
              bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
    返 回 值：成功返回0，否则返回错误码
    注意事项：三点方式不能描述整圆
    */
    uBit32 CSM_ProgArcPT(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag);



    //----------------------------------------------程序缓冲区操作接口----------------------------------------------

    /*
    函 数 名：uBit32 CSM_SetProgBufMode(Bit32 iCrdSysIndex, Bit32 iMode)
    功    能：设置程序缓冲区工作模式
    参    数：iCrdSysIndex --通道索引
              iMode --工作模式，取值为: 0-动态模式，1-静态模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 CSM_SetProgBufMode(Bit32 iCrdSysIndex, Bit32 iMode);

    uBit32 CSM_OpenProgBuf(Bit32 iCrdSysIndex);

    uBit32 CSM_CloseProgBuf(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSN_AddProgDataToBuf(Bit32 iCrdSysIndex, IIP_BUF* pIIPData)
    功    能：添加程序段数据到插补缓冲区
    参    数：iCrdSysIndex --通道索引
              pIIPData --程序段数据地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层调用
    注意事项：
    */
    uBit32 CSM_AddProgDataToBuf(Bit32 iCrdSysIndex, IIP_BUF* pIIPData);

    /*
    函 数 名：Bit32 CSM_GetProgBufFreeCount(Bit32 iCrdSysIndex)
    功    能：获取程序缓冲区空闲段数
    参    数：iCrdSysIndex --通道索引
    返 回 值：程序缓冲区空闲段数0
    调用位置：应用层调用
    注意事项：
    */
    Bit32 CSM_GetProgBufFreeCount(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSM_ClearProgData(Bit32 iCrdSysIndex, Bit32 iClearMode)
    功    能：清除程序数据
    参    数：iCrdSysIndex --坐标系索引
              iClearMode --清除模式，取值为：
                            0-清除没有运行的数据段
                            1-清除所有的运动段，但不包括正在执行的运动段
                            2-清除所有的运动段，包括正在执行的运动段
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层（解释器）
    注意事项：
    */
    uBit32 CSM_ClearProgData(Bit32 iCrdSysIndex, Bit32 iClearMode);

    /*
    函 数 名：Bit32 CSM_CheckProgBufEmpty(Bit32 iCrdSysIndex)
    功    能：检测通道程序缓冲区是否为空
    参    数：iCrdSysIndex --通道索引
    返 回 值：通道程序缓冲区为空返回1，否则返回0
    调用位置：应用层调用
    注意事项：
    */
    Bit32 CSM_CheckProgBufEmpty(Bit32 iCrdSysIndex);


    const DATA_BUF_MAN* CSM_GetProgBufStateReadAddr(Bit32 iCrdSysIndex);


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
    uBit32 CSM_UploadProgData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen);

    
    //----------------------------------------------用户自定义数据缓冲区操作接口----------------------------------------------

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
    Bit32 CSM_SetUserDataBufCtrlMode(Bit32 iCrdSysIndex, Bit32 iMode, DATA_BUF_MAN** ppDataBufMan);

    //iMode=0插补器控制用户缓冲区，iMode=1解释器控制用户缓冲区
    Bit32 CSM_SetUserDataBufCtrlMode_IPR(Bit32 iMode);//, uBit32* pulStartAddr, uBit32* pulSize

    /*
    函 数 名：uBit32 CSM_SetUserBufMode(Bit32 iCrdSysIndex, Bit32 iMode)
    功    能：设置用户自定义数据缓冲区工作模式
    参    数：iCrdSysIndex --通道索引
              iMode --工作模式，取值为：1-静态模式，0-动态模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层
    注意事项：
    */
    uBit32 CSM_SetUserBufMode(Bit32 iCrdSysIndex, Bit32 iMode);

    /*
    函 数 名：void CSM_OpenUserBuf(Bit32 iCrdSysIndex)
    功    能：打开用户自定义数据缓冲区
    参    数：iCrdSysIndex --通道索引
    返 回 值：无
    调用位置：应用层
    注意事项：
    */
    void CSM_OpenUserBuf(Bit32 iCrdSysIndex);

    /*
    函 数 名：void CSM_CloseUserBuf(Bit32 iCrdSysIndex)
    功    能：关闭用户自定义数据缓冲区
    参    数：iCrdSysIndex --通道索引
    返 回 值：无
    调用位置：应用层
    注意事项：
    */
    void CSM_CloseUserBuf(Bit32 iCrdSysIndex);

    /*
    函 数 名：void CSM_ClearUserData(Bit32 iCrdSysIndex)
    功    能：清除用户自定义数据
    参    数：iCrdSysIndex --通道索引
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 CSM_ClearUserData(Bit32 iCrdSysIndex);

    /*
    函 数 名：uBit32 CSM_AddUserDataToBuf(Bit32 iCrdSysIndex, Bit8* pUserData, Bit32 iDataLen)
    功    能：添加用户自定义数据到缓冲区
    参    数：iCrdSysIndex --通道索引
              pUserData --用户数据起始地址
              iDataLen --用户数据长度
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层
    注意事项：
    */
    uBit32 CSM_AddUserDataToBuf(Bit32 iCrdSysIndex, Bit8* pUserData, Bit32 iDataLen);

    /*
    函 数 名：uBit32 CSM_SetUserBufReadPos(Bit32 iCrdSysIndex, uBit32 ulReadPos)
    功    能：设置用户自定义数据缓冲区读位置
    参    数：iCrdSysIndex --通道索引
              ulReadPos --读位置
    返 回 值：成功返回0，否则返回错误码
    调用位置：应用层
    注意事项：
    */
    uBit32 CSM_SetUserBufReadPos(Bit32 iCrdSysIndex, uBit32 ulReadPos);

    /*
    函 数 名：Bit32 CSM_GetUserData(Bit32 iCrdSysIndex, Bit8* pUserData, uBit32 ulBufLen)
    功    能：读取用户自定义数据
    参    数：iCrdSysIndex --通道索引
              pUserData --读取的数据存放缓冲区地址
              ulBufLen --读取的数据存放缓冲区长度
    返 回 值：实际读取的用户数据长度
    调用位置：应用层
    注意事项：1、读取数据后，读取位置会自动向后偏移；2、动态模式下，数据读取后，缓冲区中的对应数据会自动清除
    */
    Bit32 CSM_GetUserData(Bit32 iCrdSysIndex, Bit8* pUserData, uBit32 ulBufLen);

    /*
    函 数 名：Bit32 CSM_GetUserDataCountInBuf(int iCrdSysIndex)
    功    能：获取用户自定义数据缓冲区中的有效数据长度
    参    数：iCrdSysIndex --通道索引
    返 回 值：用户自定义数据缓冲区中的有效数据长度
    调用位置：应用层
    注意事项：无
    */
    int CSM_GetUserDataCountInBuf(int iCrdSysIndex);

    /*
    函 数 名：Bit32 CSM_GetUserBufFreeSize(int iCrdSysIndex)
    功    能：获取用户自定义数据缓冲区的空闲长度
    参    数：iCrdSysIndex --通道索引
    返 回 值：用户自定义数据缓冲区的空闲长度
    调用位置：应用层
    注意事项：无
    */
    Bit32 CSM_GetUserBufFreeSize(Bit32 iCrdSysIndex);

    //获取用户自定义数据缓冲区总长度
    Bit32 CSM_GetUserBufTotalSize(Bit32 iCrdSysIndex);

    /*
    函 数 名：void CSM_GetUserBufStateData(Bit32 iCrdSysIndex, DATA_BUF_MAN* pUserBufState)
    功    能：获取用户自定义数据缓冲区状态数据
    参    数：iCrdSysIndex --通道索引
              *pUserBufState --用户缓冲区状态数据（出参）
    返 回 值：无
    调用位置：应用层
    注意事项：无
    */
    void CSM_GetUserBufStateData(Bit32 iCrdSysIndex, DATA_BUF_MAN* pUserBufState);


    const DATA_BUF_MAN* CSM_GetUserBufStateReadAddr(Bit32 iCrdSysIndex);


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
    uBit32 CSM_UploadUserData(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen);

    //----------------------------------------------逻辑轴相关操作接口----------------------------------------------

    /*
    函 数 名：LAX_Enable
    功    能：设置轴使能
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              iEnable --使能标记，0-断使能，1-加使能
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_Enable(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iEnable, Bit32 iWaitExeResult);

    /*
    函 数 名：LAX_Home
    功    能：设置轴回零
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              iHomeFlag --回零标记，0-回零停止，1-回零开始
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：本函数回零停止执行成功表示回零停止指令成功发送，不表示回零动作已经停止，从回零停止指令成功发送到回零动作完全停止有一个过程；
    */
    uBit32 LAX_Home(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iHomeFlag, Bit32 iWaitExeResult);

    /*
    函 数 名：LAX_SetCmdPos
    功    能：设置轴指令位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              lCmdPos --指令位置，单位：um（旋转轴为0.001度）
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_SetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lCmdPos, Bit32 iWaitExeResult);

    //added by alan at 2016-7-14
    uBit32 CSM_SetCmdPos(Bit32 iCrdSysIndex,Bit32 *lCmdPos, Bit32 iAxisCount);
    Bit32* CSM_GetAxisRfPosAddr(Bit32 nCh,Bit32 iAxisNo);                    //07获取机床参考点位置参数的缓存地址，G28用 (YHZ 疑问回零时这个参数起何作用?)

    /*
    函 数 名：LAX_SetQeiPos
    功    能：设置编码器位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              lQeiPos --编码器位置
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_SetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lQeiPos, Bit32 iWaitExeResult);
    
    /*
    函 数 名：uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType)
    功    能：检测坐标轴上一次的指令执行状态
    参    数：iCrdSysIndex  --通道索引
              iAxisIndex --坐标轴索引
              *pCmdType --指令类型（出参）
              *pExeRes --指令执行结果（出参）
    返 回 值：执行完成返回0，正在执行返回1
    调用位置：
    注意事项：
    */
    uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType, uBit32 *pExeRes);

    /*
    函 数 名：uBit32 LAX_GetAxisStateData(Bit32 iCrdSysIndex, Bit32 iAxisIndex, AXIS_STATE_DATA* pAxisStateData, Bit32 iStateDataLen)
    功    能：获取电机轴状态数据
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pAxisStateData --坐标轴状态数据缓冲区起始地址
              iStateDataLen --坐标轴状态数据缓冲区长度（表示可以存放多少个轴状态）
    返 回 值：成功返回0，否则返回非0值
    调用位置：应用层调用
    注意事项：无
    */
    uBit32 LAX_GetAxisStateData(Bit32 iCrdSysIndex, Bit32 iAxisIndex, AXIS_STATE_DATA* pAxisStateData, Bit32 iStateDataLen);

    /*
    函 数 名：LAX_GetCtrlStatus
    功    能：获取轴控制状态字
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pulCtrlStatus --坐标轴控制状态字缓冲区起始地址
              iDataLen --坐标轴控制状态字缓冲区长度（表示可以存放多少个轴状态）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetCtrlStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulCtrlStatus, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetCtrlStatus
    功    能：获取轴运行状态字
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pulRunningStatus --坐标轴运行状态字缓冲区起始地址
              iDataLen --坐标轴运行状态字缓冲区长度（表示可以存放多少个轴状态）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetRunningStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulRunningStatus, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetCmdPos
    功    能：获取轴指令位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pdCmdPos --坐标轴指令位置缓冲区起始地址
              iDataLen --坐标轴指令位置缓冲区长度（表示可以存放多少个轴的指令位置）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdCmdPos, Bit32 iDataLen);

    /*
    函 数 名：AXIS_GetPlusPos
    功    能：获取轴脉冲位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pdPulsePos --坐标轴脉冲位置缓冲区起始地址
              iDataLen --坐标轴脉冲位置缓冲区长度（表示可以存放多少个轴的脉冲位置）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetPulsePos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdPulsePos, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetQeiPos
    功    能：获取轴编码器位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              plQeiPos --坐标轴编码器位置缓冲区起始地址
              iDataLen --坐标轴编码器位置缓冲区长度（表示可以存放多少个轴的编码器位置）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32* plQeiPos, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetCmdSpeed
    功    能：获取轴编码器位置
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pdCmdSpeed --坐标轴指令速度缓冲区起始地址
              iDataLen --坐标轴指令速度缓冲区长度（表示可以存放多少个轴的指令速度）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetCmdSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdCmdSpeed, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetQeiSpeed
    功    能：获取轴编码器速度
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pdQeiSpeed --坐标轴编码器速度缓冲区起始地址
              iDataLen --坐标轴编码器速度缓冲区长度（表示可以存放多少个轴的编码器速度）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetQeiSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdQeiSpeed, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetAccSpeed
    功    能：获取轴加速度
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pdAccSpeed --坐标轴加速度缓冲区起始地址
              iDataLen --坐标轴加速度缓冲区长度（表示可以存放多少个轴的加速度）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetAccSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdAccSpeed, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetSvStatus
    功    能：获取伺服系统状态
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pulSvStatus --坐标轴伺服状态缓冲区起始地址
              iDataLen --坐标轴伺服状态缓冲区长度（表示可以存放多少个轴的伺服状态）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetSvStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulSvStatus, Bit32 iDataLen);

    /*
    函 数 名：LAX_GetSvAlarm
    功    能：获取伺服报警状态
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
              pulSvAlarm --坐标轴伺服报警缓冲区起始地址
              iDataLen --坐标轴伺服报警缓冲区长度（表示可以存放多少个轴的伺服报警）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 LAX_GetSvAlarm(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulSvAlarm, Bit32 iDataLen);

    /*
    函 数 名：Bit32 LAX_CheckAxisStop(Bit32 iCrdSysIndex, Bit32 iAxisIndex)
    功    能：检测轴是否运动停止
    参    数：iCrdSysIndex --通道索引
              iAxisIndex --坐标轴索引
    返 回 值：运动停止返回1，否则返回0
    调用位置：
    注意事项：无
    */
    Bit32 LAX_CheckAxisStop(Bit32 iCrdSysIndex, Bit32 iAxisIndex);


    //Alan Deleted at 2016-8-23
    //由模拟器实现
    //Bit32 CSM_SaveProgData(Bit32 iCrdSysIndex ,Bit8 *buf);
    //Bit32 CSM_LoadProgData(Bit32 iCrdSysIndex ,Bit8 *buf);
    //------------
    
    
   uBit32 CSM_AllChProgEStop();
 
    
#ifdef __cplusplus
}
#endif



#endif
