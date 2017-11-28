
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_Interface.h

摘    要：系统管理模块接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月17日

使用说明：
                1、升级数据格式说明如下：
                    (1) 升级数据块包括：块头+数据
                    (2) 块头包含的信息有：当前块序号，当前块数据长度
                    (3) 第一块升级数据块具有两个作用：请求升级；发送升级文件头
                    (4) 最后一块升级数据块进行数据校验
*/


#ifndef SYS_INTERFACE_H
#define SYS_INTERFACE_H


#include "..\\DataType\\DataType.h"
#include "..\\Version\\VER_DataStructDef.h"




#ifdef __cplusplus
extern "C" {
#endif


uBit32 SYS_Init();
void SYS_MainCtrl(void);








    /*
    函 数 名：void SYS_SetSLCVersion(uBit8 uMainNo, uBit8 uSubNo)
    功    能：设置M0程序版本号
    参    数：uMainNo -- 主版本号
              uSubNo -- 子版本号
    返 回 值：无
    调用位置：
    注意事项：
    
    void SYS_SetSLCVersion(uBit8 uMainNo, uBit8 uSubNo);*/ 
    
    /*
    函 数 名：uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion)
    功    能：获取M0程序版本信息
    参    数：*pVersion --程序版本信息（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion);
    


    
    /*
    函 数 名：uBit32 SYS_SimpleDelayUs(uBit32 ulUs)
    功    能：延时us
    参    数：ulUs
    返 回 值：无
    调用位置：
    注意事项：*/
    void SYS_SimpleDelayUs(uBit32 ulUs);        




#ifdef __cplusplus
}
#endif



#endif
