
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_CircleDataProcess.h

摘    要：圆弧数据处理

运行环境： LPC43xx 

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年12月03日
*/

#ifndef CSM_CIRCLE_DATA_PROCESS_H
#define CSM_CIRCLE_DATA_PROCESS_H


#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"
#include "CNC/DataStructDef/DataType.h"


#ifdef __cplusplus
extern "C" {
#endif



    /*
    函 数 名：uBit32 CSM_CircleDataProcess(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData)
    功    能：将用户设置的圆弧数据转换为插补要求的圆弧数据
    参    数：pStartPos --当前起点坐标数据起始地址（指向长度为CRDSYS_AXIS_MAX_COUNT的数组）
              pProgData --程序段数据地址
              pMotionData --插补器定义的运动数据地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：通道管理模块内部
    注意事项：无
    */
    uBit32 CSM_CircleDataProcess(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData);




#ifdef __cplusplus
}
#endif



#endif
