
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：ECM_ErrorCodeMan.c

摘    要：错误码管理模块

运行环境： LPC43xx 

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年10月15日
*/

#include <string.h>

#include "ECM_ErrorCodeMan.h"
#include "ECM_DataStructDef.h"
#include "..\\DataStructDef\\DataType.h"


static ERROR_CODE_MAN_DATA    m_ErrorCodeManData;


/*
函 数 名：void ECM_InitErrorCodeData()
功    能：初始化错误码管理数据
参    数：无
返 回 值：无
调用位置：
注意事项：无
*/
void ECM_InitErrorCodeData()
{
    memset(&m_ErrorCodeManData, 0, sizeof(ERROR_CODE_MAN_DATA));
}

/*
函 数 名：ERROR_CODE ECM_GenerateErrorCode(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
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
ERROR_CODE ECM_GenerateErrorCodeStruct(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
{
    ERROR_CODE ErrorCode;
    
    ErrorCode.ulModuleNO = ulModuleNO;
    ErrorCode.ulCrdNO = ulCrdNO;
    ErrorCode.ulCodeNO = ulErrCodeNO;
    ErrorCode.ulCmdType = ulCmdType;
    ErrorCode.ulDeviceNO = ulDeviceNO;
    ErrorCode.ulErrorLevel = ulErrorLevel;

    return ErrorCode;
}


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
uBit32 ECM_GenerateErrorCode(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
{
    ERROR_CODE ErrorCode;
    uBit32 ulErrorCode;
    
    ErrorCode.ulModuleNO = ulModuleNO;
    ErrorCode.ulCrdNO = ulCrdNO;
    ErrorCode.ulCodeNO = ulErrCodeNO;
    ErrorCode.ulCmdType = ulCmdType;
    ErrorCode.ulDeviceNO = ulDeviceNO;
    ErrorCode.ulErrorLevel = ulErrorLevel;

    memcpy(&ulErrorCode, &ErrorCode, sizeof(ulErrorCode));

    return ulErrorCode;
}


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
uBit32 ECM_PutErrorToBuf(uBit32 ulModuleNO, uBit32 ulCrdNO, uBit32 ulErrCodeNO, uBit32 ulCmdType, uBit32 ulDeviceNO, uBit32 ulErrorLevel)
{
    ERROR_CODE ErrorCode;

    ErrorCode.ulModuleNO = ulModuleNO;
    ErrorCode.ulCrdNO = ulCrdNO;
    ErrorCode.ulCodeNO = ulErrCodeNO;
    ErrorCode.ulCmdType = ulCmdType;
    ErrorCode.ulDeviceNO = ulDeviceNO;
    ErrorCode.ulErrorLevel = ulErrorLevel;

    return ECM_PutErrorCodeStructToBuf(ErrorCode);
}

/*
函 数 名：uBit32 ECM_PutErrorCodeToBuf(uBit32 ulErrorCode)
功    能：将错误码存入缓冲区
参    数：ulErrorCode --错误码 
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：无
*/
uBit32 ECM_PutErrorCodeToBuf(uBit32 ulErrorCode)
{
    ERROR_CODE ErrorCode;

    memcpy(&ErrorCode, &ulErrorCode, sizeof(ErrorCode));

    return ECM_PutErrorCodeStructToBuf(ErrorCode);
}

/*
函 数 名：uBit32 ECM_PutErrorCodeToBuf(ERROR_CODE ErrorCode)
功    能：将错误码存入缓冲区
参    数：ErrorCode --错误码 
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：无
*/
uBit32 ECM_PutErrorCodeStructToBuf(ERROR_CODE ErrorCode)
{
    //
    m_ErrorCodeManData.ErrorCode[m_ErrorCodeManData.nWritePos] = ErrorCode;
    m_ErrorCodeManData.nWritePos++;
    m_ErrorCodeManData.nErrorCount++;
    
    if(m_ErrorCodeManData.nWritePos >= ERROR_CODE_MAX_COUNT)
    {
        m_ErrorCodeManData.nWritePos = 0;
    }

    //
    if (m_ErrorCodeManData.nErrorCount > ERROR_CODE_MAX_COUNT)
    {
        m_ErrorCodeManData.nErrorCount = ERROR_CODE_MAX_COUNT;
        m_ErrorCodeManData.nReadPos++;
        if(m_ErrorCodeManData.nReadPos >= ERROR_CODE_MAX_COUNT)
        {
            m_ErrorCodeManData.nReadPos = 0;
        }

        return 1;
    }

    return 0;
}

/*
函 数 名：Bit32 ECM_GetErrorCodeStruct(ERROR_CODE* pErrorCode)
功    能：获取一条错误码
参    数：*pErrorCode --错误码（出参）
返 回 值：获取成功（缓冲区中存在错误码）返回0，否则返回1
调用位置：
注意事项：无
*/
Bit32 ECM_GetErrorCodeStruct(ERROR_CODE* pErrorCode)
{
    if(m_ErrorCodeManData.nErrorCount == 0)
    {
        return 1;
    }
    
    *pErrorCode = m_ErrorCodeManData.ErrorCode[m_ErrorCodeManData.nReadPos];
    m_ErrorCodeManData.nErrorCount--;
    m_ErrorCodeManData.nReadPos++;
   
    if(m_ErrorCodeManData.nReadPos >= ERROR_CODE_MAX_COUNT)
    {
        m_ErrorCodeManData.nReadPos = 0;
    }
    
    return 0; 
}

/*
函 数 名：Bit32 ECM_GetErrorCode(uBit32* pulErrorCode)
功    能：获取一条错误码
参    数：*pulErrorCode --错误码（出参）
返 回 值：获取成功（缓冲区中存在错误码）返回0，否则返回1
调用位置：
注意事项：无
*/
uBit32 ECM_GetErrorCode(uBit32* pulErrorCode)
{
    ERROR_CODE ErrorCode;

    if (ECM_GetErrorCodeStruct(&ErrorCode))
    {
        return 1;
    }
    else
    {
        memcpy(pulErrorCode, &ErrorCode, sizeof(uBit32));
        return 0; 
    }
}

/*
函 数 名：uBit16 ECM_GetErrorCodeNum()
功    能：获取当前缓冲区中的错误码条数
参    数：无
返 回 值：错误码条数
调用位置：
注意事项：无
*/
uBit16 ECM_GetErrorCount()
{
    return m_ErrorCodeManData.nErrorCount;
}



