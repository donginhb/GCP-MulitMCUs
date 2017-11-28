/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： 

修改了内容：

修改日期： 

原 作 者：毛军

完成日期：2015年08月22日        
*/
#ifndef DEV_ABSTRACT_H
#define DEV_ABSTRACT_H


#include "DEV_DriveStructDef.h"
#include "..\\DataStructDef\\DataType.h"


#define DEV_BUS_MAX_COUNT    (3)            //系统支持的设备总线种类最大数量


//总线信息数据结构体
typedef struct _BusInfo {
    uBit16 nBusType;
    uBit16 nBusChannelMask;
    BUS_DEVICE_DRIVE BusDevDrv;
}BUS_INFO; 

#ifdef __cplusplus
extern "C" {
#endif







#ifdef __cplusplus
}
#endif

#endif

