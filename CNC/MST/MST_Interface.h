
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_Interface.h

摘    要：mst模块接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月13日

*/

#ifndef MST_INTERFACE_H
#define MST_INTERFACE_H


#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"
#include "..\\DataStructDef\\IPR_DataStructDef.h"
#include "..\\DataStructDef\\MST_DataStructDef.h"

//
typedef struct _MST_InterFace {
    //通道管理模块操作接口函数
    uBit32 (*pf_CSM_ProgStop)(uBit16 nCrdSysMask);

    //设备管理模块操作接口函数
    uBit32 (*pf_IO_SetOutputStatus)(uBit32 ulIONO, uBit32 ulStatus, uBit32 ulMask);


    //------------------------------------------------------------------------------------------------------------------
    //错误管理相关接口
    uBit32 (*pf_ECM_GenerateErrorCode)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);
    uBit32 (*pf_ECM_PutErrorToBuf)(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);

    //
    double64 (*pf_SYS_GetSysTickCount)(void);

}MST_INTERFACE;




#ifdef __cplusplus
extern "C" {
#endif

    int MST_InitInterface(MST_INTERFACE* pMSTInterface);

    void MST_Init();

    uBit32 MST_Reset(Bit32 iCrdSysIndex);

    void MST_Run(uBit16 nCrdSysMask);

    void MST_Stop(uBit16 nCrdSysMask);

    void MST_Cancel(uBit16 nCrdSysMask);

    /*
    函 数 名：int MST_CheckBeforeActionMSTExist(IIP_BUF* pProgData)
    功    能：检测程序段是否存在前作用MST代码
    参    数：pProgData --程序段数据地址
    返 回 值：存在前作用MST代码返回1，否则返回0
    调用位置：
    注意事项：无
    */
    Bit32 MST_CheckBeforeActionMSTExist(IIP_BUF* pProgData);

    /*
    函 数 名：int CSM_CheckAfterActionMSTExist(IIP_BUF* pProgData)
    功    能：检测程序段是否存在后作用MST代码
    参    数：pProgData --程序段数据地址
    返 回 值：存在后作用MST代码返回1，否则返回0
    调用位置：
    注意事项：无
    */
    Bit32 MST_CheckAfterActionMSTExist(IIP_BUF* pProgData);


    uBit32 MST_ProcessBeforeActionMST(Bit32 iCrdSysIndex, IIP_BUF* pProgData);

    uBit32 MST_ProcessAfterActionMST(Bit32 iCrdSysIndex, IIP_BUF* pProgData);

    void MST_MainCtrl();

    //
    uBit32 MST_GetStateData(Bit32 iCrdSysIndex, MST_STATE_DATA* pMSTStateData);



#ifdef __cplusplus
}
#endif



#endif
