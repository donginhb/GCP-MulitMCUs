/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：ERR_DataStructDef.h

摘    要：定义错误管理数据结构体

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年8月27日
*/

#ifndef ERR_DATA_STRUCT_DEF_H
#define ERR_DATA_STRUCT_DEF_H

#include "DataType.h"

#pragma pack(push)
#pragma pack(2)    


//定义系统组成模块编号
#define SYS_SOFT_MODULE_USER_INTERFACE            (0)        //上位机用户接口模块

#define SYS_SOFT_MODULE_COM_WITH_UPCPU            (1)        //与上位机的通信控制模块
#define SYS_SOFT_MODULE_SYS                        (2)        //系统管理模块
#define SYS_SOFT_MODULE_SPM                        (3)        //参数管理模块
#define SYS_SOFT_MODULE_CSM                        (4)        //通道管理模块
#define SYS_SOFT_MODULE_IPR                        (5)        //解释器
#define SYS_SOFT_MODULE_IPO                        (6)        //插补器
#define SYS_SOFT_MODULE_DEV_MAN                    (7)        //设备管理模块
#define SYS_SOFT_MODULE_MST                        (8)        //MST模块
#define SYS_SOFT_MODULE_USER_REG_MAN            (9)        //用户寄存器管理模块
#define SYS_SOFT_MODULE_PLC                        (10)    //PLC模块

#define SYS_SOFT_MODULE_AXIS                    (11)    //轴设备
#define SYS_SOFT_MODULE_IO                        (12)    //IO设备
#define SYS_SOFT_MODULE_ADDA                    (13)    //ADDA设备


//定义错误等级
#define ERR_LEVEL_NORMAL                        (0)        //普通错误（操作性错误）
#define ERR_LEVEL_SYS                            (1)        //系统错误（系统不稳定产生的错误）
#define ERR_LEVEL_SYS_FATAL                        (2)        //系统致命错误（设计性错误：例如模块间调用不合理、系统程序配置不正确等）


//
#define ERR_CRD_NO_INVALID                        (0x0F)
#define ERR_DEVICE_NO_INVALID                    (0x3F)


//错误码数据定义
typedef struct _ErrorCode {
    uBit32 ulCodeNO:            8;    //错误编号
    uBit32 ulCmdType:           8;    //指令类型
    uBit32 ulDeviceNO:          6;    //设备编号或坐标轴编号，取值从0开始，取值为0x3F时表示数据无效
                                      //当模块编号为通道管理模块时，表示坐标轴编号
                                      //当模块编号为设备管理模块时，表示设备编号
                                      //
    uBit32 ulErrorLevel:        2;    //错误等级
    uBit32 ulCrdNO:             4;    //通道编号，取值从0开始，取值为0xF时表示数据无效
    uBit32 ulModuleNO:            4;    //模块编号
}ERROR_CODE;















#pragma pack(pop)


#endif

