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

完成日期：2015年08月19日        
*/
#ifndef DEVICE_DATA_STRUCT_DEF_H
#define DEVICE_DATA_STRUCT_DEF_H

#include "DataType.h"
#include "AXIS_DataStructDef.h"
#include "IO_DataStructDef.h"

#pragma pack(push)
#pragma pack(8)        


//IO设备资源信息
typedef struct _IODeviceResource {
    uBit8 nInputCount;            //输入点数量
    uBit8 nOutputCount;            //输出点数量
    uBit8 nPWMOutPutStartPos;    //可作为PWM输出的输出点起始位置
    uBit8 ulPWMOutputMask;        //可作为PWM输出的输出点从nPWMOutPutStartPos开始的掩码
}IO_DEVICE_RESOURCE;

//ADDA设备资源信息
typedef struct _ADDADeviceResource {
    uBit8 nDACount;                //DA数量
    uBit8 nADCount;                //AD数量
    uBit8 nDABitLen;            //DA位长
    uBit8 nADBitLen;            //AD位长
}ADDA_DEVICE_RESOURCE;


//设备信息数据结构体
typedef struct _DeviceInfo {
    uBit32 nDevieID;                                //设备ID
    uBit16 nBusType;                                //总线类型
    uBit16 nBusNO;                                    //总线编号(总线通道序号)
    uBit16 nDeviceType;                                //设备类型
    uBit16 nDeviceNO;                                //设备编号
    uBit16 nDeviceNOLast;                            //上一次的设备编号

    union{
        IO_DEVICE_RESOURCE        IOResource;            //IO资源信息
        ADDA_DEVICE_RESOURCE    ADDAResource;        //ADDA资源信息
    };

}DEVICE_INFO;


//设备配置数据结构体（配置设备编号）
typedef struct _DeviceConfig {
    //uBit16 nBusType;                                //总线类型
    //uBit16 nBusNO;                                //总线编号
    uBit32 nDevieID;                                //设备ID
    uBit16 nDeviceNO;                                //设备编号
}DEVICE_CONFIG;




//设备状态数据结构体
typedef struct _DeviceState {
    uBit16 nDeviceType;                                //设备类型
    uBit16 nDeviceNO;                                //设备编号

    union {
        AXIS_STATE_DATA        AxisStateData;
        IO_STATE_DATA        IOStateData;
        ADDA_STATE_DATA        ADDAStateData;
    };

}DEVICE_STATE;



#pragma pack(pop)


#endif

