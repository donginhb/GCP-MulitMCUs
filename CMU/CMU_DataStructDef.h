/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：CMU_DataStructDef.h 

摘    要：下位机通信模块数据结构定义

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：3.0

修改作者：Alan

修改日期：2015年9月7日
*/

#ifndef _CMU_DATA_STRUCT_DEF_H_
#define _CMU_DATA_STRUCT_DEF_H_


#include "UIN_GlobalDataDef.h"

#include "../CNC/DataStructDef/SYS_DataStructDef.h"
#include "../CNC/DataStructDef/AXIS_DataStructDef.h"
#include "../CNC/DataStructDef/CRD_DataStructDef.h"
#include "../CNC/DataStructDef/IO_DataStructDef.h"
#include "../CNC/DataStructDef/DEV_DataStructDef.h"
#include "../CNC/DataStructDef/IPR_DataStructDef.h"
#include "../CNC/DataStructDef/IPO_DataStructDefPub.h"

#include "Version/Version.h"


//定义通信方式---------------------------------------------------------------------
#ifndef COM_TYPE_NULL
#define COM_TYPE_NULL            (0)   //没有连接
#endif
#ifndef COM_TYPE_CAN
#define COM_TYPE_CAN            (1)   //CAN通信模式
#endif
#ifndef COM_TYPE_ENET
#define COM_TYPE_ENET            (2)   //以太网通信模式
#endif

#ifndef COM_TRANSMIT_BUF_SIZE
#define COM_TRANSMIT_BUF_SIZE                (1280)//接收或发送缓冲区大小
#endif


typedef struct _cmu_external_fun_table
{
      //--------------------------------------------系统管理模块------------------------------------------------
    double64 (*pf_SYS_GetSysTickCount)(void);        //系统时钟计数
    void (*pf_SYS_SetSysStatusBit)(uBit32 ulStatusBit);
    uBit32 (*pf_SYS_PreUpdateDevProc)(Bit8* pUpdateData, uBit32 ulDataLen);         //
    uBit32 (*pf_SYS_PostUpdateDevProc)(Bit8* pUpdateData, uBit32 ulDataLen);        //
    uBit32 (*pf_SYS_UpdateSLC)(Bit8* pUpdateData, uBit32 ulDataLen);                //更新M0程序(进入Bootloader)
    uBit32 (*pf_SYS_UpdateIPO)(Bit8* pUpdateData, uBit32 ulDataLen);                //更新M4程序(进入Bootloader)
    uBit32 (*pf_SYS_UpdateBootloader)(Bit8* pUpdateData, uBit32 ulDataLen);    
    uBit32 (*pf_SYS_GetSLCVersion)(SOFTWARE_VERSION* pVersion);
    uBit32 (*pf_SYS_GetIPOVersion)(SOFTWARE_VERSION* pVersion);
    uBit32 (*pf_SYS_SetComType)(uBit32 ulComType);
    const SYS_STATE_DATA* (*pf_SYS_GetStateReadAddr)(void);
    //将固定循环程序代码程序写入缓冲区 iBlockNo--块序号 从0开始
    uBit32 (*pf_SYS_UpdateCCFileData)(uBit32 ulBlockNo, uBit8 *pBuff, uBit32 ulSize,uBit8 uEndFlag);
    void   (*pf_SYS_Reset)(void);
    
    //高速接口
    uBit32 (*pf_GPIO_GetHSpdOutputStatus)(void);
    uBit32 (*pf_GPIO_GetHSpdInputStatus)(void);
    uBit32 (*pf_GPIO_SetHSpdOutputState)(uBit32 ulMask, uBit32 ulState);
    uBit32 (*pf_MDAC_SetHSpdDaData)(uBit32 ulChIndex, uBit32 ulDaData);
    uBit32 (*pf_MDAC_GetHSpdDaData)(uBit32 ulChIndex, uBit32 *pDaData);
     Bit32 (*pf_QEI_GetHSpdQeiPos)(void);
     Bit32 (*pf_QEI_GetHSpdQeiSpeed)(void);
    uBit32 (*pf_SYS_IPREnable)(Bit32 iEnable);  //使能解释器


    //--------------------------------------------参数管理模块------------------------------------------------
    uBit32 (*pf_SPM_SetSysCtrlParm)(SYS_CTRL_PARM* pSysCtrlParm);
    uBit32 (*pf_SPM_SetCrdSysAxisMapTable)(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount);
    uBit32 (*pf_SPM_SetCrdParm)(Bit32 iCrdSysIndex,  CRDSYS_PARM* pCrdParm);
    uBit32 (*pf_SPM_SetAxisParm)(Bit32 iAxisNO, AXIS_PARM* pAxisParm);//设置电机控制参数        
    uBit32 (*pf_SPM_SetAxisPitchCmpParm)(Bit32 iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm);//设置电机螺距补偿        
    uBit32 (*pf_SPM_SetLookaheadEnable)(Bit32 iCrdSysIndex, Bit32 iEnable);
    const SYS_CTRL_PARM* (*pf_SPM_GetSysCtrlParmAddr)(void);
    const CRDSYS_AXIS_MAP_TABLE* (*pf_SPM_GetCrdSysAxisMapTableAddr)(void);//????Alan 是否缺少坐标系索引参数
    const CRDSYS_PARM* (*pf_SPM_GetCrdSysParmAddr)(void);
    const AXIS_PARM* (*pf_SPM_GetAxisParmAddr)(void);
    const AXIS_PITCH_CMP_PARM* (*pf_SPM_GetAxisPitchCmpParmAddr)(void);
    uBit32 (*pf_SPM_SaveParm)(void);                                //保存控制参数
    uBit32 (*pf_SPM_SaveConfig)(void);                                //保存配置信息

    //--------------------------------------------设备管理模块------------------------------------------------

    uBit32 (*pf_DEV_BlinkLED)(uBit32 ulDeviceID);
    uBit32 (*pf_DEV_SetConfig)(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount); 
    uBit32 (*pf_DEV_GetVersion)(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion);
    uBit32 (*pf_DEV_GetBusDriverVersion)(uBit32 ulBusType, SOFTWARE_VERSION* pVersion);
    uBit32 (*pf_DEV_GetBusRsMsData)(uBit32 ulBusType, BUS_RS_MS_DATA *pBusRsMsData);
    uBit32 (*pf_DEV_GetCount)(void);
    uBit32 (*pf_DEV_GetInfo)(uBit32 ulDevNO, DEVICE_INFO* pDeviceInfo);
    uBit32 (*pf_DEV_Scan)(void);                                        //设备扫描
    const DEVICE_STATE* (*pf_DEV_GetStateReadAddr)(uBit32 ulDevNO); //需要在设备管理模块添加该接口
    uBit32 (*pf_DEV_SendUpdateAppData)(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 ulDataLen);
    uBit32 (*pf_DEV_SendUpdateBootData)(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 ulDataLen);//???是否需要添加设置升级设备BOOTLOADER接口
    uBit32 (*pf_DEV_CheckLastCmdExeState)(uBit32 ulDevNO, uBit32* pCmdType, uBit32 *pExeRes);    //获取上次执行结果
    

    uBit32 (*pf_PAX_Home)(uBit32 ulAxisNO, Bit32 iHomeFlag, Bit32 iWaitExeResult);//电机回零                                                
    uBit32 (*pf_PAX_SetCmdPos)(uBit32 ulAxisNO, Bit32 lCmdPos, Bit32 iWaitExeResult);    //设置电机当前置单位：um                
    uBit32 (*pf_PAX_SetQeiPos)(uBit32 ulAxisNO, Bit32 lQeiPos, Bit32 iWaitExeResult);        //设置电机当前位置单位：um        
    uBit32 (*pf_PAX_Enable)(uBit32 ulAxisNO, Bit32 iEnable, Bit32 iWaitExeResult);            //设置电机使能-断使能-加使能
    uBit32 (*pf_PAX_Reset)(uBit32 ulAxisNO);                                                                //电机复位
    uBit32 (*pf_PAX_SendSvParm)(uBit32 ulAxisNO, uBit8 *pParmData);                            //设置伺服参数
    uBit32 (*pf_PAX_GetSvParm)(uBit32 ulAxisNO, uBit16 nParmNo, uBit8 *pParmData);        //


    uBit32 (*pf_IO_SetOutputStatus)(uBit32 ulIONO, uBit32 ulMask, uBit32 ulStatus);        //设置IO板状态.需要修改模块接口                
    uBit32 (*pf_IO_SetOutPutPWMMode)(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iMode);        //使能IOPWM功能            
    uBit32 (*pf_IO_SetOutPutPWMDuty)(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iDuty);            //设置占空比-100                    
    uBit32 (*pf_IO_SetOutPutPWMFreq)(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iFreq);            //设置输出口PWM频率
    uBit32 (*pf_ADDA_SetDAStatus)(uBit32 ulADDANO, uBit32 iDAChNO, uBit32 iDAVaule);//设置DA板DA输出

    //--------------------------------------------通道管理模块------------------------------------------------

    uBit32 (*pf_LAX_Home)(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iHomeFlag, Bit32 iWaitExeResult);//轴回零                        
    uBit32 (*pf_LAX_SetCmdPos)(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lCmdPos, Bit32 iWaitExeResult);//设置轴当前位置单位：um        
    uBit32 (*pf_LAX_SetQeiPos)(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lQeiPos, Bit32 iWaitExeResult);//设置轴当前位置单位：um        
    uBit32 (*pf_LAX_Enable)(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iEnable, Bit32 iWaitExeResult);
    uBit32 (*pf_LAX_CheckLastCmdExeState)(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType, uBit32 *pExeRes);    //获取上次执行结果

    uBit32 (*pf_CSM_GetVersion)(SOFTWARE_VERSION* pVersion);

    uBit32 (*pf_CSM_Reset)(Bit32 iCrdSysIndex);
    uBit32 (*pf_CSM_SetCtrlMode)(Bit32 iCrdSysIndex, uBit32 ulMode);
    //uBit32 (*pf_CSM_EnableLookahead)(Bit32 iCrdSysIndex, Bit32 iEnable);
    uBit32 (*pf_CSM_EnableVerify)(Bit32 iCrdSysIndex, Bit32 iEnable);
    uBit32 (*pf_CSM_EnableSingleSegmentMode)(Bit32 iCrdSysIndex, Bit32 iEnable);
    uBit32 (*pf_CSM_ProgEStop)(uBit16 nCrdSysMask);
    uBit32 (*pf_CSM_ProgRun)(uBit16 nCrdSysMask);
    uBit32 (*pf_CSM_ProgHold)(uBit16 nCrdSysMask);
    uBit32 (*pf_CSM_ProgStop)(uBit16 nCrdSysMask);    //解释器未使能时调用本函数停止程序
    uBit32 (*pf_CSM_ProgCancel)(uBit16 nCrdSysMask);  //解释器未使能时调用本函数取消程序
    uBit32    (*pf_CSM_ProgSegmentEndStop)(uBit16 nCrdSysMask);

    uBit32 (*pf_CSN_SetRapidOverride)(Bit32 iCrdSysIndex, float32 dOverride);
    uBit32 (*pf_CSM_SetFeedOverride)(Bit32 iCrdSysIndex, float32 dOverride);
    uBit32 (*pf_CSM_SetSpindleOverride)(Bit32 iCrdSysIndex, float32 dOverride);
    uBit32 (*pf_CSM_SpindleRun)(Bit32 iCrdSysIndex, Bit32 iDir, uBit32 ulSpeed);
    uBit32 (*pf_CSM_SpindleStop)(Bit32 iCrdSysIndex);

    const CRDSYS_STATE_DATA* (*pf_CSM_GetCrdSysStateReadAddr)(Bit32 iCrdSysIndex);

    uBit32 (*pf_CSM_SetMotorPosCtrlMotion)(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion);//设置位置控制运动数据    
    uBit32 (*pf_CSM_SetMotorSpeedCtrlMotion)(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);//设置速度控制运动数据                        
    uBit32 (*pf_CSM_SetMotorJogStop)(Bit32 iMotorNO);//电机停止    
    uBit32 (*pf_CSM_MotorJogEStop)(Bit32 iMotorNO);  //电机急停
    uBit32 (*pf_CSM_SetAxisPosCtrlMotion)(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion);    
    uBit32 (*pf_CSM_SetAxisSpeedCtrlMotion)(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);        
    uBit32 (*pf_CSM_SetAxisJogStop)(Bit32 iCrdSysIndex, uBit32 ulAxisMask);//轴停止            
    uBit32 (*pf_CSM_AxisJogEStop)(Bit32 iCrdSysIndex, uBit32 ulAxisMask);  //轴急停


    uBit32 (*pf_CSM_SetProgBufMode)(Bit32 iCrdSysIndex, Bit32 iMode);    
    uBit32 (*pf_CSM_ClearProgData)(Bit32 iCrdSysIndex, Bit32 iClearMode); //上位机接口仍然保持单一的全部清除方式
    uBit32 (*pf_CSM_AddProgDataToBuf)(Bit32 iCrdSysIndex, IIP_BUF* pIIPData);//延时、MST、快移、插补等指令    
    const DATA_BUF_MAN* (*pf_CSM_GetProgBufStateReadAddr)(Bit32 iCrdSysIndex);
    uBit32 (*pf_CSM_UploadProgData)(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen);

    uBit32 (*pf_CSM_SetUserBufMode)(Bit32 iCrdSysIndex, Bit32 iMode);                                            
    uBit32 (*pf_CSM_ClearUserData)(Bit32 iCrdSysIndex);//清除用户自定义数据区                
    uBit32 (*pf_CSM_AddUserDataToBuf)(Bit32 iCrdSysIndex, Bit8* pUserData, Bit32 iDataLen);//下载用户数据
    const DATA_BUF_MAN* (*pf_CSM_GetUserBufStateReadAddr)(Bit32 iCrdSysIndex);
    uBit32 (*pf_CSM_UploadUserData)(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen);

    uBit32 (*pf_CSM_ProgDwell)(uBit32 ulCrdNO, uBit32 ulDelayMs);
    uBit32 (*pf_CSM_ProgMCode)(uBit32 ulCrdNO, uBit16 nMCode);
    uBit32 (*pf_CSM_ProgSCode)(uBit32 ulCrdNO, uBit32 ulSCode);
    uBit32 (*pf_CSM_ProgTCode)(uBit32 ulCrdNO, uBit8 cTCode);
    uBit32 (*pf_CSM_ProgBCode)(uBit32 ulCrdNO, uBit8 cBCode);
    uBit32 (*pf_CSM_ProgIOCode)(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs);
    uBit32 (*pf_CSM_ProgRapid)(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag);
    uBit32 (*pf_CSM_ProgLine)(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag);
    uBit32 (*pf_CSM_ProgArcC)(uBit32 ulCrdNO, uBit32* pAxisNO,  Bit32 *pEndPos, Bit32* pCenter, BooLean bClockWise, float32 fSpeed, BooLean bAbsFlag);
    uBit32 (*pf_CSM_ProgArcR)(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed, BooLean bAbsFlag);
    uBit32 (*pf_CSM_ProgArcPT)(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag);


    //--------------------------------------------解释器模块------------------------------------------------
    //启动(挂起)所有解释线程，获取(释放)10M用户缓冲区控制权限，下载G代码前先检查使能状态，只有使能状态下才运行下载G代码程序
    Bit32 (*pf_IPR_SetPpiEnable)(Bit32 iEnable);

    //获取解释器使能状态    0--未使能 1--使能，使能后不能下载运动段数据，下载G代码文件前先查询
    Bit32 (*pf_IPR_GetPpiEnable)(void);    

    //初始化解释器模块,为解释器分配内存后调用(系统调用) 
    //Bit32 (*pf_IPR_CncModuleInit)(void); 

    //取消某通道的解释进程
    Bit32 (*pf_IPR_Cancel)(uBit32 ulCh);

    //暂停解释器nCh通道当前工作, 等待循环启动后再继续解释工作
    Bit32 (*pf_IPR_Pause)(uBit32 ulCh);

    //循环启动
    Bit32 (*pf_IPR_Cycle)(uBit32 ulCh);

    //将当前通道程序行的读写复位，复位到程序开始处,下次循环启动从头开始执行
    Bit32 (*pf_IPR_ProgReset)(uBit32 ulCh);

    //将指定长度的G代码程序储存到缓冲区 ulBlockNo--块序号 从0开始
    Bit32 (*pf_IPR_PutGCodeToIprBuf)(uBit32 ulCh, uBit32 ulBlockNo, uBit8 *pBuff, uBit32 ulSize, uBit8 uEndFlag);

    /*
    函数名： IPR_GetState
    取解释器状态
            返回值<0表示该通道解释器没有创建或程序已执行完
            返回值=0表示该通道解释器暂停运行
            返回值>0表示该通道解释器正在运行中(不能下载程序)
    */
    Bit32 (*pf_IPR_GetState)(uBit32 ulCh);

    //任意行启动
    Bit32 (*pf_IPR_RndStart)(uBit32 ulCh, Bit32 nLineNo); 

    //重新设置坐标零点 
    Bit32 (*pf_IPR_SetCrdZero)(uBit32 ulCh,Bit32 ax_mask,Bit32 *zero);
    
    //获取坐标系零点
    Bit32 (*pf_IPR_GetCrdZero)(uBit32 ulCh,Bit32 ax_mask,Bit32 *zero);   

    //设置刀具数据
    Bit32 (*pf_IPR_SetToolData)(uBit32 ulCh, Bit32 nToolNo,CUTTER_T *pToolData);

    //获取刀具数据
    Bit32 (*pf_IPR_GetToolData)(uBit32 ulCh, Bit32 nToolNo,CUTTER_T *pToolData);

    //配置flash
    //Bit32 (*pf_IPR_SetFlashConf)(uBit32 ulBaseAddr, uBit32 ulSize);    

    //配置解释器的全局变量缓存区首地址，及其大小，2M；
    //Bit32 (*pf_IPR_SetIprVarBufBase)(Bit8* pt_base, uBit32 len);                    

    //获取断点数据信息长度
    Bit32 (*pf_IPR_GetBpDataLen)(uBit32 ulCh);    

    //获取断点数据
    Bit32 (*pf_IPR_GetBpData)(uBit32 ulCh, uBit32 ulStartIndex, uBit8 *pBuf, uBit32 ulBufLen);    

    //装在断点数据 uEndFlag=1 表示最后一个数据块
    Bit32 (*pf_IPR_SetBpData)(uBit32 ulCh, uBit32 ulStartIndex, uBit8 *pBuf, uBit32 ulBufLen, uBit8 uEndFlag);

    //设置当前刀具号
    Bit32 (*pf_IPR_SetTool)(uBit32 ulCh, Bit32 nToolNo);

    //设置基准刀具
    Bit32 (*pf_IPR_SetBaseTool)(uBit32 ulCh, Bit32 nToolNo);

    //刷新更新解释器通道参数（重新配置坐标系轴映射表时调用）
    Bit32 (*pf_IPR_UpdateCrdData)(void);

    //获取通道当前解释行号
    Bit32 (*pf_IPR_GetCurLineNo)(uBit32 ulCh, Bit32 *pLineNo);

    //获取通道当前基准刀具号
    Bit32 (*pf_IPR_GetBaseToolNo)(uBit32 ulCh, Bit32 *pToolNo);

    //后期完善----
    //设置当前编程位置
    Bit32 (*pf_IPR_SetProgPos)(uBit32 ulCh, Bit32 iAxisMask, Bit32 *pPos);

    //取当前程编位置（工件坐标系值）, 返回轴屏蔽字
    Bit32 (*pf_IPR_GetProgPos)(uBit32 ulCh, Bit32 *pPos);


    //--------------------------------------------寄存器模块------------------------------------------------
    uBit32 (*pf_UREG_SetRegData)(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount);
    uBit32 (*pf_UREG_GetRegData)(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount);

    //--------------------------------------------错误管理模块-----------------------------------------------
    //错误管理模块接口
    uBit32 (*pf_ECM_GetErrorCode)(uBit32 *pulErrorCode);
}CMU_EXTERNAL_FUN_TEBLE;

typedef struct _ipr_out_api
{
    //启动(挂起)所有解释线程，获取(释放)10M用户缓冲区控制权限，下载G代码前先检查使能状态，只有使能状态下才运行下载G代码程序
    Bit32 (*pf_IPR_SetPpiEnable)(Bit32 iEnable);

    //获取解释器使能状态    0--未使能 1--使能，使能后不能下载运动段数据，下载G代码文件前先查询
    Bit32 (*pf_IPR_GetPpiEnable)(void);    

    //初始化解释器模块,为解释器分配内存后调用(系统调用) 
    Bit32 (*pf_IPR_CncModuleInit)(void); 

    //取消某通道的解释进程
    Bit32 (*pf_IPR_Cancel)(uBit32 ulCh);

    //暂停解释器nCh通道当前工作, 等待循环启动后再继续解释工作
    Bit32 (*pf_IPR_Pause)(uBit32 ulCh);

    //循环启动
    Bit32 (*pf_IPR_Cycle)(uBit32 ulCh);

    //将当前通道程序行的读写复位，复位到程序开始处,下次循环启动从头开始执行
    Bit32 (*pf_IPR_ProgReset)(uBit32 ulCh);

    //将指定长度的G代码程序储存到缓冲区 ulBlockNo--块序号 从0开始
    Bit32 (*pf_IPR_PutGCodeToIprBuf)(uBit32 ulCh, uBit32 ulBlockNo, uBit8 *pBuff, uBit32 ulSize, uBit8 uEndFlag);

    /*
    函数名： IPR_GetState
    取解释器状态
            返回值<0表示该通道解释器没有创建或程序已执行完
            返回值=0表示该通道解释器暂停运行
            返回值>0表示该通道解释器正在运行中(不能下载程序)
    */
    Bit32  (*pf_IPR_GetState)(uBit32 ulCh);

    //任意行启动
    Bit32 (*pf_IPR_RndStart)(uBit32 ulCh, Bit32 nLineNo); 

    //重新设置坐标零点 
    Bit32 (*pf_IPR_SetCrdZero)(uBit32 ulCh, Bit32 iCrdZeroType, Bit32* pZero);
    
    //获取坐标系零点
    Bit32 (*pf_IPR_GetCrdZero)(uBit32 ulCh, Bit32 iCrdZeroType, Bit32* pZero);    

    //设置刀具数据
    Bit32 (*pf_IPR_SetToolData)(uBit32 ulCh, Bit32 nToolNo, CUTTER_T *pToolData);

    //获取刀具数据
    Bit32 (*pf_IPR_GetToolData)(uBit32 ulCh, Bit32 nToolNo, CUTTER_T *pToolData);

    //配置flash
    Bit32 (*pf_IPR_SetFlashConf)(uBit32 ulSize);    

    //设置用户缓冲区地址
    Bit32 (*pf_IPR_SetIprUserBufBase)(Bit8* pt_base, uBit32 len);                    

    //配置解释器的全局变量缓存区首地址，及其大小，2M；
    Bit32 (*pf_IPR_SetIprVarBufBase)(Bit8* pt_base, uBit32 len);                    

    //获取断点数据信息长度
    Bit32 (*pf_IPR_GetBpDataLen)(uBit32 ulCh);    

    //获取断点数据
    Bit32 (*pf_IPR_GetBpData)(uBit32 ulCh, uBit32 ulStartIndex, uBit8 *pBuf, uBit32 ulBufLen);    

    //装载断点数据 uEndFlag=1 表示最后一个数据块
    Bit32 (*pf_IPR_SetBpData)(uBit32 ulCh, uBit32 ulStartIndex, uBit8 *pBuf, uBit32 ulBufLen, uBit8 uEndFlag);

    //设置当前刀具号
    Bit32 (*pf_IPR_SetTool)(uBit32 ulCh, Bit32 nToolNo);

    //设置基准刀具
    Bit32 (*pf_IPR_SetBaseTool)(uBit32 ulCh, Bit32 nToolNo);

    //刷新更新解释器通道参数（重新配置坐标系轴映射表时调用）
    Bit32 (*pf_IPR_UpdateCrdData)(void);

    //获取通道当前解释行号
    Bit32 (*pf_IPR_GetCurLineNo)(uBit32 ulCh, Bit32 *pLineNo);

    //获取通道当前基准刀具号
    Bit32 (*pf_IPR_GetBaseToolNo)(uBit32 ulCh, Bit32 *pToolNo);
}ipr_out_api;
#endif

