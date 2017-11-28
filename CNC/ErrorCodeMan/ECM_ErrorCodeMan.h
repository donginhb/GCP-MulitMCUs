
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：ECM_ErrorCodeMan.h

摘    要：错误码管理模块

运行环境： LPC43xx 

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年9月10日
*/

#ifndef ERROR_CODE_MAN_H
#define ERROR_CODE_MAN_H

#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"



#ifdef __cplusplus
extern "C" {
#endif

    /*
    函 数 名：void ECM_Init()
    功    能：初始化错误码管理数据
    参    数：无
    返 回 值：无
    调用位置：系统启动时调用
    注意事项：无
    */
    void ECM_Init();

    /*
    函 数 名：ERROR_CODE ECM_GenerateErrorCodeStruct(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
    功    能：生成错误码
    参    数：ulModuleNO --模块编号
              ulCrdNO --坐标系编号
              ulErrCodeNO --错误编号
              ulCmdType --指令类型
              ulDeviceNO --设备编号
              ulErrorLevel --错误等级
    返 回 值：错误码
    调用位置：
    注意事项：无
    */
    ERROR_CODE ECM_GenerateErrorCodeStruct(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);

    /*
    函 数 名：uBit32 ECM_GenerateErrorCode(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
    功    能：生成错误码
    参    数：ulModuleNO --模块编号
              ulCrdNO --坐标系编号
              ulErrCodeNO --错误编号
              ulCmdType --指令类型
              ulDeviceNO --设备编号
              ulErrorLevel --错误等级
    返 回 值：错误码
    调用位置：
    注意事项：无
    */
    uBit32 ECM_GenerateErrorCode(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);

    /*
    函 数 名：uBit32 ECM_PutErrorToBuf(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
    功    能：将错误码存入缓冲区
    参    数：ulModuleNO --模块编号
              ulCrdNO --坐标系编号
              ulErrCodeNO --错误编号
              ulCmdType --指令类型
              ulDeviceNO --设备编号
              ulErrorLevel --错误等级
    返 回 值：成功返回0，否则返回非0值
    调用位置：
    注意事项：无
    */
    uBit32 ECM_PutErrorToBuf(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel);


    /*
    函 数 名：uBit32 ECM_PutErrorCodeToBuf(uBit32 ulErrorCode)
    功    能：将错误码存入缓冲区
    参    数：ulErrorCode --错误码 
    返 回 值：成功返回0，否则返回非0值
    调用位置：
    注意事项：无
    */
    uBit32 ECM_PutErrorCodeToBuf(uBit32 ulErrorCode);

    /*
    函 数 名：uBit32 ECM_PutErrorCodeStructToBuf(ERROR_CODE ErrorCode)
    功    能：将错误码存入缓冲区
    参    数：ErrorCode --错误码 
    返 回 值：成功返回0，否则返回非0值
    调用位置：
    注意事项：无
    */
    uBit32 ECM_PutErrorCodeStructToBuf(ERROR_CODE ErrorCode);


    /*
    函 数 名：Bit32 ECM_GetErrorCodeStruct(ERROR_CODE* pErrorCode)
    功    能：获取一条错误码
    参    数：*pErrorCode --错误码（出参）
    返 回 值：获取成功（缓冲区中存在错误码）返回0，否则返回1
    调用位置：
    注意事项：无
    */
    Bit32 ECM_GetErrorCodeStruct(ERROR_CODE* pErrorCode);

    /*
    函 数 名：Bit32 ECM_GetErrorCode(uBit32* pulErrorCode)
    功    能：获取一条错误码
    参    数：*pulErrorCode --错误码（出参）
    返 回 值：获取成功（缓冲区中存在错误码）返回0，否则返回1
    调用位置：
    注意事项：无
    */
    uBit32 ECM_GetErrorCode(uBit32* pulErrorCode);

    /*
    函 数 名：uBit16 ECM_GetErrorCount()
    功    能：获取当前缓冲区中的错误码条数
    参    数：无
    返 回 值：错误码条数
    调用位置：
    注意事项：无
    */
    uBit16 ECM_GetErrorCount(void);


#ifdef __cplusplus
}
#endif


#endif
