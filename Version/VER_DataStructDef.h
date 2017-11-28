
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：VER_DataStructDef.h 

摘    要：定义软件版本信息数据结构

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年08月22日

*/

#ifndef _VER_DATA_STRUCT_DEF_H_
#define _VER_DATA_STRUCT_DEF_H_

#include "DataType/DataType.h"


#pragma pack(push)
#pragma pack(2)        

//系统组件编号
#define SYS_PART_USER_INTERFACE                    (0)            //上位机程序
#define SYS_PART_SLC                            (1)            //逻辑控制（M0）
#define SYS_PART_IPO                            (2)            //插补运算（M4）
#define SYS_PART_AXIS                            (3)            //轴设备
#define SYS_PART_OUT                            (4)            //输出设备
#define SYS_PART_IN                                (5)            //输入设备
#define SYS_PART_ADDA                            (6)            //ADDA设备


#define SYS_PART_CSM                            (20)        //通道管理模块



//软件版本数据结构体
typedef struct _SoftwareVersion {
    Bit8    cType[4];            //产品型号
    Bit8    cSoftVer[2];        //软件版本
    Bit8    cPart;                //组件号
    Bit8    cFlag;                //软件版本特殊标示
}SOFTWARE_VERSION;


#pragma pack(pop)

#endif 

