
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：UREG_Interface.h

摘    要：用户寄存器数据管理

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月21日

使用说明：

*/

#ifndef UREG_INTERFACE_H
#define UREG_INTERFACE_H


#ifdef __cplusplus
extern "C" {
#endif

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
    uBit32 UREG_SetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount);


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
    uBit32 UREG_GetRegData(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount);

    /*
    函 数 名：UREG_GetReg_f(uBit32 ulRegNo)
    功    能：获取用户寄存器的据值（浮点类型）
    参    数：ulRegNo --寄存器编号(0-3999)
    返 回 值：寄存器值
    调用位置：PLC模块
    注意事项：
    */
    float UREG_GetReg_f(uBit32 ulRegNo);


    /*
    函 数 名：UREG_GetReg_l(uBit32 ulRegNo)
    功    能：获取用户寄存器的据值（长整数类型）
    参    数：ulRegNo --寄存器编号(0-3999)
    返 回 值：寄存器值
    调用位置：PLC模块
    注意事项：
    */
    long  UREG_GetReg_l(uBit32 ulRegNo);
    /*
    函 数 名：UREG_SetReg_f(uBit32 ulRegNo, float fVal)
    功    能：设置用户寄存器的据值（浮点类型）
    参    数：ulRegNo   --寄存器编号(0-3999)
            fVal    --设置值
    返 回 值：寄存器值
    调用位置：PLC模块
    注意事项：
    */
    void UREG_SetReg_f(uBit32 ulRegNo, float fVal);


#ifdef __cplusplus
}
#endif



#endif
