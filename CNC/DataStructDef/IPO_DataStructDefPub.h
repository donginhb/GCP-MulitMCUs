
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：AXIS_DataStructDef.h 

摘    要：插补器数据结构定义

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：1、规范参数名
          2、去掉控制状态及运行状态位域定义
          3、明确参数的单位

当前版本：2.0

修改作者：毛军

修改日期：2013年11月15日

//-----------------------------------------------------------------------------------------------
修改描述：1、增加花样机运动数据

当前版本：2.1

修改作者：毛军

修改日期：2013年12月25日

//-----------------------------------------------------------------------------------------------
修改描述：1、插补器控制状态字和运行状态字定义统一到坐标系运行状态字和控制状态字

当前版本：2.2

修改作者：毛军

修改日期：2014年1月6日

//-----------------------------------------------------------------------------------------------
修改描述：1、增加比较输出运动段数据

当前版本：2.3

修改作者：毛军

修改日期：2015年8月28日

*/

#ifndef IPO_DATA_STRUCT_DEF_PUB_H
#define IPO_DATA_STRUCT_DEF_PUB_H

#include "DataType.h"
#include "SYS_DataStructDef.h"
#include "SYS_ConfigDef.h"

#pragma pack(push)
#pragma pack(8)        

//----------------------------------------------------------------------------------------------------------
//运动模式定义
#define MOTION_MODE_NULL                (0)            //无运动
#define MOTION_MODE_INTERPOLATION        (1)            //插补运动模式
#define MOTION_MODE_POSCTRL                (2)            //位置控制模式
#define MOTION_MODE_SPEEDCTRL            (3)            //速度控制模式
#define MOTION_MODE_PT                    (4)            //PT模式
#define MOTION_MODE_PVT                    (5)            //PVT模式
#define MOTION_MODE_STITCH                (6)            //缝纫运动控制模式
#define MOTION_MODE_COMPARE_OUTPUT        (7)            //比较输出（该模式不运动，只记录下一运动段要执行比较输出的位置）


//插补模式运动指令定义
#define MOTION_CMD_NULL                    (0)            //
#define MOTION_CMD_RAPID                (1)            //块速定位            (G00)
#define MOTION_CMD_LINE                    (2)            //直线                (G01)
#define MOTION_CMD_CW                    (3)            //顺时钟圆弧        (G02)
#define MOTION_CMD_CCW                    (4)            //逆时针圆弧        (G03)
#define MOTION_CMD_DWELL                (5)            //暂停                (G04)
#define MOTION_CMD_LINE_ACCURATE_STOP    (6)            //当前行准确停止    (G09)        
#define MOTION_CMD_SMOTH_TRANS            (7)            //段间平滑过渡        (G64)
#define MOTION_CMD_ACCURATE_STOP        (8)            //段间准确停止        (G61)
#define MOTION_CMD_CW_CYLSPIRAL            (9)            //顺时钟螺旋形        (G02)
#define MOTION_CMD_CCW_CYLSPIRAL        (10)        //逆时针螺旋形        (G03)

//圆弧数据
typedef struct _CircleData {
    uBit8        nAxisIndex[2];    //参加圆弧插补的轴索引
                                //说明：从轴nAxisIndex[0]到轴nAxisIndex[1]为逆时针方向
    double64    dI;                //（两轴圆弧插补中的）第一轴圆心坐标，单位：用户计数单位
    double64    dJ;                //（两轴圆弧插补中的）第二轴圆心坐标，单位：用户计数单位
    double64    dR;                //半径，单位：用户计数单位
}CIRCLE_DATA;

//圆锥螺旋线 conical spiral
//圆柱螺旋线 cylindrical spiral

//圆柱螺旋线数据
typedef struct _CylSpiralData {
    uBit8        nAxisIndex[4];    //参加圆弧插补的轴索引，取值0-7（坐标系坐标轴映射表中的轴索引）
                                //说明：轴nAxisIndex[0]、nAxisIndex[1]控制圆弧，且轴nAxisIndex[0]到轴nAxisIndex[1]为逆时针方向
                                //      轴nAxisIndex[2]控制导程
                                //      nAxisIndex[3]空闲，只是为了使结构体大小为偶数个字节
    double64    dI;                //（两轴圆弧插补中的）第一轴圆心坐标(绝对坐标)，单位：用户计数单位
    double64    dJ;                //（两轴圆弧插补中的）第二轴圆心坐标(绝对坐标)，单位：用户计数单位
    double64    dR;                //半径，单位：用户计数单位
    double64    dK;                //导程，单位：用户计数单位

    double64    dAngle;            //螺旋线的倾角，单位：用户计数单位（弧度）
}CYLSPIRAL_DATA;

//暂停数据
typedef struct _DwellData {
    uBit32        ulTime;            //暂时时间，单位：ms
}DWELL_DATA;

#define ENDSPEED_SMOTH_FLAG            (999999.0)            //(前瞻模式下)段末速度平滑过度标识值

//插补运动模式数据结构体
typedef struct _IPOMotionData {
    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];    //当前段终点坐标（轴绝对坐标），单位：用户计数单位
                                                //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
                                                //说明：数组中坐标值存放顺序按照坐标系坐标轴映射表中各坐标轴出现的先后顺序存放
    uBit16        nCmdType;
    uBit16      nReserval;

    float32        dFeedRate;                        //进给速度，单位：用户计数单位 / ms
    float32        dEndSpeed;                        //插补段终点速度， 当取值为ENDSPEED_SMOTH_FLAG时，表示(前瞻使能)段末速度平滑过度
                                                //如果系统设置了段末速度平滑过度，则运动程序中设置的终点速度无效

    uBit32     ulOutputDisableAxisMask;            //不输出（不运动）的轴掩码

    union
    {
        CIRCLE_DATA         Circle;
        DWELL_DATA         Dwell;
        CYLSPIRAL_DATA   CylSpiral;
    }Data;

}IPO_MOTION_DATA;


//缝纫运动控制模式运动数据结构体
typedef struct _STITCHMotionData {
    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];    //当前段终点坐标（轴绝对坐标），单位：um
                                                //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
                                                //说明：数组中坐标值存放顺序按照坐标系坐标轴映射表中各坐标轴出现的先后顺序存放

    uBit16        nCmdType;                        //1-缝纫运动段，0-空送运动段
    uBit16      nReserval;

    float32        dMaxSpeed;                        //最大速度，单位：用户计数单位 / ms

    Bit32       lUQeiPos;                        //当前U轴编码器参考位置，单位：内部脉冲； 当值为0xFFFFFFFF时，表示该值无效
    float32     dUQeiSpeed;                     //当前U轴编码器参考速度，单位：内部脉冲/ms  
    float32     dFeedEndPos;                    //送料终止位置
}STITCH_MOTION_DATA;


//比较输出运动模式数据结构体
typedef struct _CmpOutputData {
    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];            //下一运动段执行比较输出的坐标（轴绝对坐标），单位：用户计数单位
                                                //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
                                                //说明：数组中坐标值存放顺序按照坐标系坐标轴映射表中各坐标轴出现的先后顺序存放

    uBit16        nCmdType;                        //预留

    uBit16     nIONO;                            //IO设备编号
    uBit32     ulOutputMask;                    //IO输出点掩码
    uBit32     ulOutputState;                    //IO输出点状态
}CMP_OUTPUT_DATA;



//PT运动段类型
#define PT_SEG_TYPE_NORMAL                (0)            //普通段
#define PT_SEG_TYPE_CONSTANT_SPEED        (1)            //匀速段
#define PT_SEG_TYPE_SLOW_TO_ZERO        (2)            //减速到0段

//PT运动模式数据结构体
typedef struct _PTMotionData {
    uBit16        nType;                    //运动段类型
    double64    dTime;                    //时间，单位：毫秒
    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];    //轴绝对坐标
}PT_MOTION_DATA;


//PVT运动模式数据结构体
typedef struct _PVTMotionData {
    uBit16        nType;                    //PVT描述方式

    double64    dPos;                    //位置
    float32        dVel;                    //速度    
    float32        dTime;                    //时间，单位：毫秒
    float32        dPercent;                //百分比
    float32        dVelMax;                //最大速度
    float32        dAcc;                    //加速度
    float32        dDec;                    //减速度
}PVT_MOTION_DATA;    

//运动段数据结构体(说明：该数据结构的长度需保证是偶数字节，以满足字节对齐要求)
typedef struct _SegmentMotionData {
    uBit32      ulUserLineNO;            //用户自定义行号
    uBit32        ulProgLineNO;            //程序行号（解释器解析出来的指令单元行号，包含了G代码、M-S-T指令等， 该行号从1开始，连续无限累加，不可重复）
    uBit16        nMotionMode;            //运动模式

    union
    {
        IPO_MOTION_DATA        IPO;        //插补模式运动数据
        PT_MOTION_DATA        PT;            //PT模式运动数据
        STITCH_MOTION_DATA  Stitch;        //缝纫运动数据
        //PVT_MOTION_DATA    PVT[CRDSYS_AXIS_MAX_COUNT];

        CMP_OUTPUT_DATA     CmpOutput;    //比较输出数据结构：坐标、IO点
    };

}SEGMENT_MOTION_DATA;

//位置控制运动模式数据结构体
typedef    struct _PosCtrlMotionData {
    double64    dPos;            //目标位置， 单位：用户计数单位
    float32        dSpeed;            //目标速度,  单位：用户计数单位/ms，取值大于0
}POSCTRL_MOTION_DATA;


//速度控制运动模式数据结构体
typedef    struct _SpeedCtrlMotionData {
    float32    dAcc;            //加速度，单位：用户计数单位/ms2
    float32    dDec;            //减速度，单位：用户计数单位/ms2
    float32    dJerk;            //捷度，单位：用户计数单位/ms3
    float32    dSpeed;            //目标速度，单位：用户计数单位/ms
}SPEEDCTRL_MOTION_DATA;

//------------------------------------------------------------------------------------------------

//插补器运行状态数据结构体
typedef struct _IPOStateData {
    double64    dTargetCrd[CRDSYS_AXIS_MAX_COUNT];    //当前段目标位置
    double64    dTotalFeedDis;                //总进给距离，单位：用户计数单位

    uBit32        ulCtrlStatus;            //控制状态
    uBit32        ulRunningStatus;        //运行状态
    uBit32        ulAlarm;                //报警状态

    uBit32        ulCurProgLineNO;        //当前运行程序行号（解释器解析出来的指令单元行号，包含了G代码、M-S-T指令等， 该行号从1开始，连续无限累加，不可重复）
    float32        dRapidOverride;            //快移速度修调，取值范围0.0-1.0
    float32        dFeedOverride;            //进给速度修调，取值范围0.0-1.0
    float32        dProgFeedRate;            //程序设定进给速度，单位：用户计数单位/毫秒
    float32        dFeedRate;                //程序当前进给速度，单位：用户计数单位/毫秒
    float32        dAcc;                    //程序当前加速度，单位：用户计数单位/平方毫秒

    uBit16      nAxisCloseMask;            //关闭坐标轴脉冲输出，bit0-bit7分别对应坐标系的坐标轴1-坐标轴7，对应位为1表示关闭该轴的输出

    uBit16        ulReserval1;
}IPO_STATE_DATA;



//插补计算的轴状态
typedef struct _IPOAxisStateData {
    double64                dCmdPos;            //轴指令位置，单位：用户计数单位
    double64                dPulsePos;            //轴脉冲位置，单位；脉冲数     
    float32                    dCmdSpeed;            //轴当前指令速度，单位：用户计数单位/毫秒
    float32                    dAcc;                //轴当前加速度，单位：用户计数单位/平方毫秒
    Bit32                    lPinc;                //一个周期的进给脉冲数
}IPO_AXIS_STATE_DATA;



#pragma pack(pop)






#endif