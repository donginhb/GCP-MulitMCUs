/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：UREG_DataMan.c

摘    要：用户寄存器数据管理

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月21日

使用说明：
/////////////////////////////////////////////////////////////////////////////////////////
修改描述：

1.修改UREG_SetRegData和UREG_SetRegData，寄存器数据由之前的2位修改为4位，兼容浮点数据
2.添加UREG_GetReg_f、UREG_GetReg_l、UREG_SetReg_f和UREG_SetReg_l接口操作单个寄存器数据

修改作者：Alan

修改日期：2016-4-15
*/
#include "string.h"
#include "SDRAM_AddrDef.h"
#include "CNC_AddrDef.h"
#include "CNC/DataStructDef/DataType.h"

#include "../../CMU/CMU_ErrorCodeDef.h"

#define MAX_REG_NO  (UREG_BUF_TOTAL_LEN/4 -1)

#pragma diag_suppress=Pa084 //屏蔽对MAX_REG_NO比较恒为假的警告


/*
函 数 名：uBit32 UREG_SetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount)
功    能：设置用户寄存器的值
参    数：ulStartRegIndex --寄存器写起始位置，表示从第几个寄存器开始设置值
pData --寄存器数据缓冲区
ulCount --数据长度
返 回 值：成功返回0，否则返回错误编码
调用位置：通信管理模块
注意事项：
*/
uBit32 UREG_SetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount)
{
    uBit32 *pAddr;
    
    if((ulStartRegIndex+ulCount)>MAX_REG_NO)
    {
        return CMU_ERR_UREGNO_RANGOUT;
    }
    
    pAddr = (uBit32*)(UREG_BUF_START_ADDR+4*ulStartRegIndex);
    memcpy(pAddr, pData, ulCount*sizeof(uBit32));
    return 0;
}


/*
函 数 名：uBit32 UREG_GetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount)
功    能：获取用户寄存器的值
参    数：ulStartRegIndex --寄存器读起始位置
pData --寄存器数据缓冲区
ulCount --数据长度
返 回 值：成功返回0，否则返回错误编码
调用位置：通信管理模块
注意事项：
*/
uBit32 UREG_GetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount)
{
    uBit32 *pAddr;
    
    if((ulStartRegIndex+ulCount) > MAX_REG_NO)
    {
        return CMU_ERR_UREGNO_RANGOUT;
    }
    
    pAddr = (uBit32*)(UREG_BUF_START_ADDR + 4*ulStartRegIndex);
    memcpy(pData, pAddr, ulCount*sizeof(uBit32));
    return 0;
}


/*
函 数 名：UREG_GetReg_f(uBit32 ulRegNo)
功    能：获取用户寄存器的据值（浮点类型）
参    数：ulRegNo --寄存器编号(0-3999)
返 回 值：寄存器值
调用位置：PLC模块
注意事项：
*/
float UREG_GetReg_f(uBit32 ulRegNo)
{
    return *(float*)(UREG_BUF_START_ADDR+4*ulRegNo);
}


/*
函 数 名：UREG_GetReg_l(uBit32 ulRegNo)
功    能：获取用户寄存器的据值（长整数类型）
参    数：ulRegNo --寄存器编号(0-3999)
返 回 值：寄存器值
调用位置：PLC模块
注意事项：
*/
long  UREG_GetReg_l(uBit32 ulRegNo)
{
    return *(long*)(UREG_BUF_START_ADDR+4*ulRegNo);
}

/*
函 数 名：UREG_SetReg_f(uBit32 ulRegNo, float fVal)
功    能：设置用户寄存器的据值（浮点类型）
参    数：ulRegNo   --寄存器编号(0-3999)
fVal    --设置值
返 回 值：寄存器值
调用位置：PLC模块
注意事项：
*/
void UREG_SetReg_f(uBit32 ulRegNo, float fVal)
{
    float *pAddr;
    
    pAddr = (float*)UREG_BUF_START_ADDR+4*ulRegNo;
    *pAddr = fVal;
}


/*
函 数 名：UREG_SetReg_l(uBit32 ulRegNo, long lVal)
功    能：获取用户寄存器的据值（长整数类型）
参    数：ulRegNo --寄存器编号(0-3999)
lVal    --设置值    
返 回 值：PLC模块
调用位置：
注意事项：
*/
void  UREG_SetReg_l(uBit32 ulRegNo, long lVal)
{
    long *pAddr;
    
    pAddr = (long*)UREG_BUF_START_ADDR+4*ulRegNo;
    *pAddr = lVal;
}
