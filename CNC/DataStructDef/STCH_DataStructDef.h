/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：StitchDataStructDefine.h 

摘    要：花样机专用数据结构定义

运行环境：LPC43xx

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2013年12月25日

*/

#ifndef STITCH_DATA_STRUCT_DEFINE_H
#define STITCH_DATA_STRUCT_DEFINE_H

#include "..\\DataStructDef\\DataType.h"


#define STITCH_START_SLOW           //标示是否采用启动慢速
#define STITCH_MOTOR_COUNT          (3)     //电机数量

#define U_AXIS_INDEX                  (0)  //U轴电机索引

#define U_AXIS_CMD_POS_FOR_ONE_REV    (360000)     //单位为0。001度
//#define U_AXIS_PULSE_POS_FOR_ONE_REV  (10000)     //单位脉冲
#define U_AXIS_QEI_POS_FOR_ONE_REV    (10000)     //编码器分辨率

#pragma pack(push)
#pragma pack(8)        



//花样机单针工艺数据结构体
typedef struct _StitchTechData {
    Bit32  lX;                          //X轴位置，单位：脉冲
    Bit32  lY;                          //Y轴位置，单位：脉冲
    Bit32  lZ;                          //Z轴位置，单位：脉冲
    uBit32 ulM;                          //M代码
}STITCH_TECH_DATA;


//------------------------------------------------------------------------

//花样机运动控制参数结构体（需发送到插补器模块）
typedef struct _StitchMotionCtrlParm {
    uBit32  ulUStartSafePosForXY;            //XY可以移动的U轴起始安全位置（送料起始角度），单位：0.001度
    uBit32  ulUEndSafePosForXY;                //XY可以移动的U轴结束安全位置（送料终止角度），单位：0.001度
    uBit16  nXYMoveAdvanceTime;                //XY移动提前时间，单位：ms          (目前设置为9ms)
                                            //说明：该值不能大于主轴以最高速度从ulUEndSafePosForXY到ulUStartSafePosForXY所用的最短时间

    uBit16  nStartSpeed;                    //XY移动起始速度，单位：um/ms       (目前设置为62.5um/ms  = 10pulse/ms)
    uBit16  nDecTime;                        //XY减速时间，单位：ms                (目前设置为3ms)

    uBit16  nLongMoveSpeed;                    //长距离移送速度，单位：um/ms
    uBit16  nShortMoveSpeed;                //短距离移送速度，单位：um/ms
    uBit16  nLongMoveThreshold;                //移送距离阀值(小于该值为短距离移送，否则为长距离移送)，单位：um

    uBit16  nStartTime;                     //XY启动时间，单位：ms
    uBit16    nReserval;

}STITCH_MOTION_CTRL_PARM;  //


//缝纫工艺参数
typedef struct _STITCHTECHPARM {
    
    uBit16    nStartSlowSW;            //bit0: 起始慢针开关，1-开，0-关
                                    //bit1: 起始超慢针开关，1-开，0-关
    uBit16  nStartSlowSpeed;        //（U轴）起始慢针速度，单位：0.001度/ms
    uBit16  nStartSlowPointCount;    //起始慢针针数
    uBit16  nStartSuperSlowSpeed;    //（U轴）起始超慢针速度，单位：0.001度/ms

    uBit16    nLineOperSW;            //bit0: 剪线开关，1-开，0-关
                                    //bit1: 扫线开关，1-开，0-关
    uBit32  ulCutLineStartPos;        //剪线开始位置，单位：0.001度
    uBit32  ulCutLineEndPos;        //剪线结束位置，单位：0.001度
    uBit16  nCutLineSpeed;            //剪线速度，单位：0.001度/ms
    uBit32  ulReleaseLineStartPos;    //松线开始位置，单位：0.001度
    uBit32  ulReleaseLineEndPos;    //松线结束位置，单位：0.001度
    uBit16  nSweepLIneOpenTime;        //扫线启动时间，单位：ms
    uBit16  nSweepLIneCloseTime;    //扫线关闭时间，单位：ms

    uBit16    nUSpeed[4];                //U轴速度设置，单位：0.001度/ms
                                    //nUSpeed[0]: 低速速度
                                    //nUSpeed[1]: 中低速速度
                                    //nUSpeed[2]: 中高速速度
                                    //nUSpeed[3]: 高速速度

    uBit32    ulUUpStopPos;            //U轴上暂停位置，单位：0.001度
    uBit32    ulUDownStopPos;            //U轴上暂停位置，单位：0.001度
    
    uBit16  nExternnalStopCtrl;        //外部暂停控制
                                    //bit0：外部暂停位置，0-下暂停，1-上暂停
                                    //bit1：外部暂停位下暂停时再启动方式，0-当前点启动，1-下一点启动
                                    //bit2: 外部暂停停止后压板是否抬起，0-不抬起，1-抬起

    uBit16  nInternalStopCtrl;        //内部暂停控制
                                    //bit0:  程序运行到次原点时是否停止，0-不停止，1-停止
                                    //bit1:  代码数据暂停后压板是否抬起，0-不抬起，1-抬起
                                    //bit2:  次原点停止后压板是否抬起，0-不抬起，1-抬起
                                    //bit3:  程序运行结束后压板是否抬起，0-不抬起，1-抬起
    
    uBit16  nProgCtrl;                //bit0: 程序中的移动方式，0-点式移动，1-针步移送

    uBit16  nMoveLimitCtrl;            //bit0: 运动范围限制使能，0-不使能，1-使能
    Bit32    lXPosLimitP;            //X轴正软限位，单位：um
    Bit32    lXPosLimitN;            //X轴负软限位，单位：um
    Bit32    lYPosLimitP;            //Y轴正软限位，单位：um
    Bit32    lYPosLimitN;            //Y轴负软限位，单位：um

    Bit32    lPressFootTravel;        //压脚行程，单位：um
    uBit16    nPressFootDownTime;        //压脚压下延时，单位：ms
    uBit16  nPressFootUpTime;        //压脚抬起延时，单位：ms

    uBit16  nLineBreakCtrl;            //bit0:  断线检测使能，0-不使能，1-使能
                                    //bit1:  低转速断线忽略使能，0-不使能，1-使能
    uBit16  nLBStartIgnoreCount;    //断线检测启动时忽略针数
    uBit16  nLBInvalidCount;        //断线检测无效针数
    uBit16  nLBIgnoreSpeed;            //断线检测忽略转速，单位：0.001度/ms



}STITCH_TECH_PARM;  //(sizeof(STITCH_TECH_PARM) = 80)



//缝纫控制参数
typedef struct _StitchCtrlParm {
    STITCH_MOTION_CTRL_PARM        MotionCtrlParm;
    STITCH_TECH_PARM            TechParm;
}STITCH_CTRL_PARM; 




#pragma pack(pop)




#endif