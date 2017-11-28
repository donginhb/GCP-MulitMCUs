/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_CircleDataProcess.c

摘    要：圆弧数据处理

运行环境： LPC43xx 

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年12月03日
*/

#include "math.h"
#include "stdlib.h"
#include "CSM_CircleDataProcess.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"
#include "..\\DataStructDef\\IPR_DataStructDef.h"
#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"


uBit32 CSM_CircleDataProcess_ThreePT(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData)
{
    double64 StartPos[2];    //圆弧起点
    double64 EndPos[2];        //圆弧终点
    double64 MidPos[2];        //圆弧中间点
    double64 CenterPos[2];  //圆心
    double64 dRadius;       //半径
    double64 x1, y1, x2, y2, x3, y3;
    double64 dAngle1, dAngle2;
    double64 dTheta;

    //
    StartPos[0] = pStartPos[pProgData->nAxis1];
    StartPos[1] = pStartPos[pProgData->nAxis2];

    if (pProgData->nFlag & IFLAG_ABS_CRD)
    { //绝对坐标
        EndPos[0] = pProgData->lPinc[pProgData->nAxis1];   
        EndPos[1] = pProgData->lPinc[pProgData->nAxis2];

        MidPos[0] = pProgData->Data.UserCircleData.MidPT[0];
        MidPos[1] = pProgData->Data.UserCircleData.MidPT[1];
    }
    else
    { //相对坐标
        EndPos[0] = pStartPos[pProgData->nAxis1] + pProgData->lPinc[pProgData->nAxis1];   
        EndPos[1] = pStartPos[pProgData->nAxis2] + pProgData->lPinc[pProgData->nAxis2];

        MidPos[0] = pStartPos[pProgData->nAxis1] + pProgData->Data.UserCircleData.MidPT[0];
        MidPos[1] = pStartPos[pProgData->nAxis2] + pProgData->Data.UserCircleData.MidPT[1];
    }

    if( fabs( (MidPos[1] - StartPos[1]) * (EndPos[0] - MidPos[0]) - (MidPos[0] - StartPos[0]) * (EndPos[1] - MidPos[1]) ) <= 0.001)
    {
        return 1;  //三点在一条直线上，无法生成圆弧
    }

    //求圆心和半径
    x1 = StartPos[0];
    y1 = StartPos[1];
    x2 = MidPos[0];
    y2 = MidPos[1];
    x3 = EndPos[0];
    y3 = EndPos[1];

    CenterPos[0] = ((y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1)+(y2-y1)*(y1*y1-y3*y3+x1*x1-x3*x3))/(2*(x2-x1)*(y3-y1)-2*(x3-x1)*(y2-y1));   //绝对坐标
    CenterPos[1] = ((x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1)+(x2-x1)*(x1*x1-x3*x3+y1*y1-y3*y3))/(2*(y2-y1)*(x3-x1)-2*(y3-y1)*(x2-x1));   

    dRadius = sqrt((StartPos[0] - CenterPos[0])*(StartPos[0] - CenterPos[0]) + (StartPos[1] - CenterPos[1]) * (StartPos[1] - CenterPos[1]));

    //求圆弧方向
    dAngle1 = atan2(MidPos[1] - StartPos[1], MidPos[0] - StartPos[0]);
    dAngle2 = atan2(EndPos[1] - MidPos[1], EndPos[0] - MidPos[0]);
    dTheta = dAngle2 - dAngle1;
    while(dTheta > PI) dTheta -= 2 * PI;
    while(dTheta < -PI) dTheta += 2 * PI;

    if(dTheta > 0)  
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CCW;
    }
    else 
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CW;
    }

    //
    //pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.lFeedSpeed / 60000.0;
    pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.dFeedSpeed;

    if (pProgData->nFlag & IFLAG_FINESTOP)
    {
        pMotionData->IPO.dEndSpeed = 0.0;
    }
    else
    {
        pMotionData->IPO.dEndSpeed = ENDSPEED_SMOTH_FLAG;
    }

    //
    pMotionData->IPO.Data.Circle.nAxisIndex[0] = pProgData->nAxis1;
    pMotionData->IPO.Data.Circle.nAxisIndex[1] = pProgData->nAxis2;
    pMotionData->IPO.Data.Circle.dI = CenterPos[0];
    pMotionData->IPO.Data.Circle.dJ = CenterPos[1];
    pMotionData->IPO.Data.Circle.dR = dRadius;

    return 0;
}



uBit32 CSM_CircleDataProcess_Radius(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData)
{
    double64 StartPos[2];    //圆弧起点
    double64 EndPos[2];        //圆弧终点
    double64 CenterPos[2];  //圆心
    double64 dRadius;       //半径
    double64 dChord;        //弦长
    double64 dDeltaX;
    double64 dDeltaY;
    double64 dTemp;

    //
    StartPos[0] = pStartPos[pProgData->nAxis1];
    StartPos[1] = pStartPos[pProgData->nAxis2];

    if (pProgData->nFlag & IFLAG_ABS_CRD)
    { //绝对坐标
        EndPos[0] = pProgData->lPinc[pProgData->nAxis1];  
        EndPos[1] = pProgData->lPinc[pProgData->nAxis2];
    }
    else
    { //相对坐标
        EndPos[0] = pStartPos[pProgData->nAxis1] + pProgData->lPinc[pProgData->nAxis1];  
        EndPos[1] = pStartPos[pProgData->nAxis1] + pProgData->lPinc[pProgData->nAxis2];
    }

    dRadius = abs(pProgData->Data.UserCircleData.iRadius);

    dDeltaX = EndPos[0] - StartPos[0];
    dDeltaY = EndPos[1] - StartPos[1];

    //
    if (fabs(dDeltaX) < 0.001 && fabs(dDeltaY) < 0.001) 
    {//圆弧起点和终点重合，无法确定的圆弧
        return 1;
    }

    //-------------------------------------------求圆心-----------------------------------
    //
    dChord = sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);

    if(dRadius < dChord/2)  
    {//如果输入圆弧半径小于圆弧起点终点旋长的一半，则报错
        return 1;
    }

    dTemp = sqrt(dRadius * dRadius / (dChord * dChord / 4) - 1);

    if ((pProgData->nCmd == ICMD_CW && pProgData->Data.UserCircleData.iRadius > 0) || (pProgData->nCmd == ICMD_CCW && pProgData->Data.UserCircleData.iRadius < 0))
    {
        dTemp *= -1.0;
    }

    CenterPos[0] = (dDeltaX - dDeltaY * dTemp) / 2;  //增量坐标
    CenterPos[1] = (dDeltaY + dDeltaX * dTemp) / 2;

    //----------------------------------------将程序数据转换为插补器数据-----------------------------------------
    //
    if (pProgData->nCmd == ICMD_CW)
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CW;
    }
    else
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CCW;
    }

    //
    //pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.lFeedSpeed / 60000.0;
    pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.dFeedSpeed;

    if (pProgData->nFlag & IFLAG_FINESTOP)
    {
        pMotionData->IPO.dEndSpeed = 0.0;
    }
    else
    {
        pMotionData->IPO.dEndSpeed = ENDSPEED_SMOTH_FLAG;
    }

    //
    pMotionData->IPO.Data.Circle.nAxisIndex[0] = pProgData->nAxis1;
    pMotionData->IPO.Data.Circle.nAxisIndex[1] = pProgData->nAxis2;
    pMotionData->IPO.Data.Circle.dI = pStartPos[pProgData->nAxis1] + CenterPos[0];
    pMotionData->IPO.Data.Circle.dJ = pStartPos[pProgData->nAxis2] + CenterPos[1];
    pMotionData->IPO.Data.Circle.dR = dRadius;

    return 0;
}

//
uBit32 CSM_CircleDataProcess_CenterPT(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData)
{
    double64 EndPos[2];        //圆弧终点
    double64 CenterPos[2];  //圆心
    double64 dDeltaX;
    double64 dDeltaY;
    double dTemp1, dTemp2;

    //
    if (pProgData->nCmd == ICMD_CW)
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CW;
    }
    else
    {
        pMotionData->IPO.nCmdType = MOTION_CMD_CCW;
    }

    //
    //pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.lFeedSpeed / 60000.0;
    pMotionData->IPO.dFeedRate = pProgData->Data.UserCircleData.dFeedSpeed;

    if (pProgData->nFlag & IFLAG_FINESTOP)
    {
        pMotionData->IPO.dEndSpeed = 0.0;
    }
    else
    {
        pMotionData->IPO.dEndSpeed = ENDSPEED_SMOTH_FLAG;
    }

    //
    pMotionData->IPO.Data.Circle.nAxisIndex[0] = pProgData->nAxis1;
    pMotionData->IPO.Data.Circle.nAxisIndex[1] = pProgData->nAxis2;
    pMotionData->IPO.Data.Circle.dI = pStartPos[pProgData->nAxis1] + pProgData->Data.UserCircleData.CenterPT[0];
    pMotionData->IPO.Data.Circle.dJ = pStartPos[pProgData->nAxis2] + pProgData->Data.UserCircleData.CenterPT[1];
    dTemp1 = pProgData->Data.UserCircleData.CenterPT[0];
    dTemp2 = pProgData->Data.UserCircleData.CenterPT[1];
    pMotionData->IPO.Data.Circle.dR = sqrt(dTemp1 * dTemp1 + dTemp2 * dTemp2);

    //求圆心到终点的距离，检查圆弧数据是否合法
    CenterPos[0] = pMotionData->IPO.Data.Circle.dI;
    CenterPos[1] = pMotionData->IPO.Data.Circle.dJ;

    if (pProgData->nFlag & IFLAG_ABS_CRD)
    { //绝对坐标
        EndPos[0] = pProgData->lPinc[pProgData->nAxis1];  
        EndPos[1] = pProgData->lPinc[pProgData->nAxis2];
    }
    else
    { //相对坐标
        EndPos[0] = pStartPos[pProgData->nAxis1] + pProgData->lPinc[pProgData->nAxis1];  
        EndPos[1] = pStartPos[pProgData->nAxis1] + pProgData->lPinc[pProgData->nAxis2];
    }

    dDeltaX = EndPos[0] - CenterPos[0];
    dDeltaY = EndPos[1] - CenterPos[1];

    dTemp1 = sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);

    if (fabs(dTemp1 - pMotionData->IPO.Data.Circle.dR) > ZERO)
    {
        return 1;
    }

    return 0;
}

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
uBit32 CSM_CircleDataProcess(double64* pStartPos, IIP_BUF* pProgData, SEGMENT_MOTION_DATA* pMotionData)
{
    uBit32 ulErrorCode;

    ulErrorCode = 1;

    //
    switch (pProgData->Data.UserCircleData.nCircleMode)
    {
        case CIRCLE_MODE_CENTER_PT:
            if (!CSM_CircleDataProcess_CenterPT(pStartPos, pProgData, pMotionData))
            {
                ulErrorCode = 0;
            }

            break;
        case CIRCLE_MODE_RADIUS:
            if (!CSM_CircleDataProcess_Radius(pStartPos, pProgData, pMotionData))
            {
                ulErrorCode = 0;
            }
            break;
        case CIRCLE_MODE_MID_PT:
            if (!CSM_CircleDataProcess_ThreePT(pStartPos, pProgData, pMotionData))
            {
                ulErrorCode = 0;
            }
            
            break;
        default:
            break;
    }

    return ulErrorCode;
}