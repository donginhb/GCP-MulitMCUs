
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：IO_DataStructDef.h 

摘    要：定义IO数据结构

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年8月20日

*/

#ifndef _IO_DATA_STRUCT_DEF_H_
#define _IO_DATA_STRUCT_DEF_H_

#include "DataType.h"


#pragma pack(push)
#pragma pack(8)        


//IO配置数据定义待补充


//定义IO模块系统状态
#define IO_STATUS_OVER_CURRENT                (0x01)    //IO模块过流报警
#define IO_STATUS_OFF_LINE                    (0X02)    //IO脱离总线
#define IO_STATUS_SEND_CYCLE_DATA_FAIL      (0x04)    //发送周期数据失败

#define IO_STATUS_UPDATE_BOOT_PERMIT        (0x10)    //升级Boot允许状态位
#define IO_STATUS_UPDATE_BOOT_SUCESS        (0x20)    //升级Boot成功状态位
#define IO_STATUS_UPDATE_PERMIT                (0x40)    //升级允许状态位
#define IO_STATUS_UPDATE_SUCESS                (0x80)    //升级成功状态位

//IO状态数据结构体
typedef struct _IOStateData {
    uBit8       cSysStatus;                        //系统状态
    uBit32        nInputStatus;                    //输入状态
    uBit32        nOutputStatus;                    //输出状态
    uBit32        nOutputCmdStatus;                //输出指令状态
}IO_STATE_DATA;


//定义每个ADDA模块中AD输入的通道数量
#define AD_CHANNEL_COUNT        (6)
#define DA_CHANNEL_COUNT        (4)


typedef struct _ADDAStateData {
    uBit8       cSysStatus;                        //系统状态
    uBit16        nADStatus[AD_CHANNEL_COUNT];    //AD状态
    uBit16        nDAStatus[DA_CHANNEL_COUNT];    //DA状态
    uBit16        nDACmdStatus[DA_CHANNEL_COUNT];    //DA指令状态
}ADDA_STATE_DATA;








#pragma pack(pop)


#endif //

