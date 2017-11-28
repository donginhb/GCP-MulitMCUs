
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_UserProgData.c

摘    要：用户程序数据处理模块

运行环境： LPC43xx 

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年12月03日
*/

#include "string.h"
#include "CSM_ParmMan.h"
#include "CSM_Interface.h"
#include "CSM_UserProgData.h"
#include "..\\DataStructDef\\IPR_DataStructDef.h"
#include "..\\ErrorCodeDef\\CSM_ErrorCodeDef.h"


extern CSM_INTERFACE    m_CSMInterface;


/*
函 数 名：uBit32 CSM_ProgDwell(uBit32 ulCrdNO, uBit32 ulDelayMs)
功    能：下载坐标系延时程序段
参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
          ulDelayMs    -- 延时时间，单位ms
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgDwell(uBit32 ulCrdNO, uBit32 ulDelayMs)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nCmd = ICMD_DWELL;
    ProgData.Data.Dwell.lTime = ulDelayMs;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 CSM_ProgMCode(uBit32 ulCrdNO, uBit16 nMCode)
功    能：下载M代码程序段
参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
          nMCode    -- M指令码 如M02时值为2
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgMCode(uBit32 ulCrdNO, uBit16 nMCode)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nMCode = nMCode;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 UIN_MCode(uBit32 ulCrdNO, uBit16 ulSCode)
功    能：下载S代码程序段
参    数：ulCrdNO -- 坐标系编号, 取值从0开始
          ulSCode -- S指令码
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgSCode(uBit32 ulCrdNO, uBit32 ulSCode)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nSCode = ulSCode;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 CSM_ProgTCode(uBit32 ulCrdNO, uBit16 cTCode)
功    能：下载T代码程序段
参    数：ulCrdNO -- 坐标系编号,从0开始
          cTCode  -- T指令码 T1时为1
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgTCode(uBit32 ulCrdNO, uBit8 cTCode)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nTCode = cTCode;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 UIN_BCode(uBit32 ulCrdNo, uBit8 cBCode)
功    能：下载B代码程序段
参    数：ulCrdNO    -- 坐标系编号, 取值从0开始
          cBCode    -- B指令码
返 回 值：0---成功 非0--错误码
注意事项：
*/
uBit32 CSM_ProgBCode(uBit32 ulCrdNO, uBit8 cBCode)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nBCode = cBCode;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：CSM_ProgIOCode(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs)
功    能：下载IO输出指令程序段
参    数：ulCrdNo        -- 坐标系编号,从0开始
          ulDeviceNo    -- IO设备编号
          ulIOMask        -- 输出口屏蔽码
          ulIOState        -- 输出值
          ulDelayMs        -- 延时时间 单位ms
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgIOCode(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs)
{
    IIP_BUF ProgData;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nBCode = M_CODE_IO_SET;
    ProgData.Data.MCodeParm.ulParm1 = ulDeviceNo;
    ProgData.Data.MCodeParm.ulParm2 = ulIOMask;
    ProgData.Data.MCodeParm.ulParm3 = ulIOState;
    ProgData.Data.MCodeParm.ulParm4 = ulDelayMs;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 CSM_ProgRapid(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag)
功    能：单个轴或者多个轴同时执行快速移动(G00)
参    数：ulCrdNO        -- 坐标系编号,从0开始
          ulAxisMask    -- 轴屏蔽字
          pPos            -- 轴目标位置数据地址（依次存放与ulAxisMask中的有效位相对应的轴目标位置）
          bAbsFlag        -- 绝对编程标志    false--相对编程 ture-绝对编程
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgRapid(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag)
{
    IIP_BUF ProgData;
    int iCrdSysAxisCount;
    int i;
    Bit32* pAxisCmdPos;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nCmd = ICMD_RAPID;
    if (bAbsFlag)
    {
        ProgData.nFlag = IFLAG_BLK_BEG | IFLAG_BLK_END | IFLAG_ABS_CRD;
    }
    else
    {
        ProgData.nFlag = IFLAG_BLK_BEG | IFLAG_BLK_END;
    }

    ProgData.ulCmdAxis = ulAxisMask;
    ProgData.ulOutputEnableAxis = ulAxisMask;

    //
    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(ulCrdNO);
    pAxisCmdPos = pPos;
    for (i = 0; i < iCrdSysAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            ProgData.lPinc[i] = *pAxisCmdPos;
            pAxisCmdPos++;
        }
    }

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 CSM_ProgLine(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag)
功    能：多轴直线插补运动
参    数：ulCrdNO    -- 坐标系编号,从0开始
          ulAxisMask    -- 轴屏蔽字
          pPos            -- 轴目标位置数据地址（依次存放与ulAxisMask中的有效位相对应的轴目标位置）
          fSpeed        -- 合成速度，单位：用户计数单位/ms
          bAbsFlag        -- 绝对编程标志    false--相对编程 ture-绝对编程
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgLine(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag)
{
    IIP_BUF ProgData;
    int iCrdSysAxisCount;
    int i;
    Bit32* pAxisCmdPos;

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nCmd = ICMD_LINE;
    if (bAbsFlag)
    {
        ProgData.nFlag = IFLAG_BLK_BEG | IFLAG_BLK_END | IFLAG_ABS_CRD;
    }
    else
    {
        ProgData.nFlag = IFLAG_BLK_BEG | IFLAG_BLK_END;
    }

    ProgData.ulCmdAxis = ulAxisMask;
    ProgData.ulOutputEnableAxis = ulAxisMask;

    //
    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(ulCrdNO);
    pAxisCmdPos = pPos;
    for (i = 0; i < iCrdSysAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            ProgData.lPinc[i] = *pAxisCmdPos;
            pAxisCmdPos++;
        }
    }

    //
    //ProgData.Data.normal.lFeedSpeed = (Bit32)(fSpeed * 100 / 6);  //fSpeed * 1000000 / 60000
    //ProgData.Data.normal.lFeedSpeed = (Bit32)(fSpeed * 1000000);
    ProgData.Data.normal.dFeedSpeed = fSpeed;

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}

/*
函 数 名：uBit32 UIN_ArcC(uBit32 ulCrdNo, uBit32* pAxisNo,  Bit32 *pEndPos, Bit32* pCenter,bool bClockWise, float32 fSpeed,bool bAbsFlag)
功    能：两轴圆弧插补(终点和圆心)
参    数：ulCrdNo                -- 坐标系编号,取值从0开始
          pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                   从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
          pEndPos                -- 圆弧终点坐标数据地址
          pCenter                -- 圆弧圆心坐标数据地址(圆心坐标为相对于圆弧起点的增量值)
          bClockWise            -- 圆弧方向， ture--顺时钟插补 false--逆时针插补
          fSpeed                -- 插补合成速度, 单位：用户计数单位/ms
          bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 CSM_ProgArcC(uBit32 ulCrdNO, uBit32* pAxisNO,  Bit32 *pEndPos, Bit32* pCenter, BooLean bClockWise, float32 fSpeed, BooLean bAbsFlag)
{
    IIP_BUF ProgData;
    int iCrdSysAxisCount;
    int i;
    uBit32 ulErrorCode;

    //
    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(ulCrdNO);
    for (i = 0; i < 2; i++)
    {
        if (pAxisNO[i] >= iCrdSysAxisCount)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ulCrdNO, CSM_ERR_PROG_DATA_ERROR, 
                CSM_CMD_PROG_ARC_CENTERPT, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //
    memset(&ProgData, 0, sizeof(ProgData));

    if (bClockWise)
    {
        ProgData.nCmd = ICMD_CW;
    }
    else
    {
        ProgData.nCmd = ICMD_CCW;
    }

    if (bAbsFlag)
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END | IFLAG_ABS_CRD;
    }
    else
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END;
    }

    ProgData.ulCmdAxis = (0x0001 << pAxisNO[0]) | (0x0001 << pAxisNO[1]);
    ProgData.ulOutputEnableAxis = ProgData.ulCmdAxis;

    //
    ProgData.lPinc[pAxisNO[0]] = pEndPos[0];
    ProgData.lPinc[pAxisNO[1]] = pEndPos[1];

    //
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 100 / 6);  //fSpeed * 1000000 / 60000
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 1000000 );
    ProgData.Data.UserCircleData.dFeedSpeed = fSpeed;

    ProgData.Data.UserCircleData.nCircleMode = CIRCLE_MODE_CENTER_PT;
    ProgData.Data.UserCircleData.CenterPT[0] = pCenter[0];
    ProgData.Data.UserCircleData.CenterPT[1] = pCenter[1];

    //
    ProgData.nAxis1 = pAxisNO[0];
    ProgData.nAxis2 = pAxisNO[1];

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}


/*
函 数 名：uBit32 UIN_ArcR(uBit32 ulCrdNo, uBit32* pAxisNo, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed,BooLean bAbsFlag)
功    能：两轴圆弧插补(终点和半径）
参    数：ulCrdNo                -- 坐标系编号,从0开始
          pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                   从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
          pEndPos                -- 圆弧终点坐标数据地址
          iRadius                -- 半径，半径大于0，表示圆弧小于或等于180度；半径小于0，表示圆弧大于180度
          bClockWise            -- 顺时针插补标志 ture--顺时钟插补 false--逆时针插补
          fSpeed                -- 合成速度, 单位：用户计数单位/ms
          bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
返 回 值：成功返回0，否则返回错误码
注意事项：半径方式不能描述整圆
*/
uBit32 CSM_ProgArcR(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed, BooLean bAbsFlag)
{
    IIP_BUF ProgData;
    int iCrdSysAxisCount;
    int i;
    uBit32 ulErrorCode;

    //
    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(ulCrdNO);
    for (i = 0; i < 2; i++)
    {
        if (pAxisNO[i] >= iCrdSysAxisCount)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ulCrdNO, CSM_ERR_PROG_DATA_ERROR, 
                CSM_CMD_PROG_ARC_CENTERPT, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //
    memset(&ProgData, 0, sizeof(ProgData));

    if (bClockWise)
    {
        ProgData.nCmd = ICMD_CW;
    }
    else
    {
        ProgData.nCmd = ICMD_CCW;
    }

    if (bAbsFlag)
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END | IFLAG_ABS_CRD;
    }
    else
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END;
    }

    ProgData.ulCmdAxis = (0x0001 << pAxisNO[0]) | (0x0001 << pAxisNO[1]);
    ProgData.ulOutputEnableAxis = ProgData.ulCmdAxis;

    //
    ProgData.lPinc[pAxisNO[0]] = pEndPos[0];
    ProgData.lPinc[pAxisNO[1]] = pEndPos[1];

    //
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 100 / 6);  //fSpeed * 1000000 / 60000
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 1000000 );
    ProgData.Data.UserCircleData.dFeedSpeed = fSpeed;

    ProgData.Data.UserCircleData.nCircleMode = CIRCLE_MODE_RADIUS;
    ProgData.Data.UserCircleData.iRadius = iRadius;

    //
    ProgData.nAxis1 = pAxisNO[0];
    ProgData.nAxis2 = pAxisNO[1];

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}


/*
函 数 名：uBit32 UIN_ArcPT(uBit32 ulCrdNo, uBit32* pAxisNo, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag)
功    能：两轴圆弧插补(终点和中间点）
参    数：ulCrdNo                -- 坐标系编号,从0开始
          pAxisNo                -- pAxisNo指向2元数组首地址，存放执行圆弧插补的坐标轴在坐标系映射表中的索引， pAxisNo[0]-执行圆弧插补的一个坐标轴索引，pAxisNo[1]-执行圆弧插补的第二个坐标轴索引， 取值从0开始
                                   从轴pAxisNo[0]旋转到轴pAxisNo[1]的方向为逆时针方向
          pEndPos                -- 圆弧终点坐标数据地址
          pMidPos                -- 圆弧中间点坐标数据地址
          fSpeed                -- 合成速度, 单位：用户计数单位/ms
          bAbsFlag                -- 绝对编程标志    false--相对编程 ture-绝对编程
返 回 值：成功返回0，否则返回错误码
注意事项：三点方式不能描述整圆
*/
uBit32 CSM_ProgArcPT(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag)
{
    IIP_BUF ProgData;
    int iCrdSysAxisCount;
    int i;
    uBit32 ulErrorCode;

    //
    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(ulCrdNO);
    for (i = 0; i < 2; i++)
    {
        if (pAxisNO[i] >= iCrdSysAxisCount)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ulCrdNO, CSM_ERR_PROG_DATA_ERROR, 
                CSM_CMD_PROG_ARC_CENTERPT, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //
    memset(&ProgData, 0, sizeof(ProgData));

    ProgData.nCmd = ICMD_CW;        //说明：三点确定圆弧时，圆弧的方向在计算圆弧时重新修正

    if (bAbsFlag)
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END | IFLAG_ABS_CRD;
    }
    else
    {
        ProgData.nFlag = IFLAG_CIRCLE_USER | IFLAG_BLK_BEG | IFLAG_BLK_END;
    }

    ProgData.ulCmdAxis = (0x0001 << pAxisNO[0]) | (0x0001 << pAxisNO[1]);
    ProgData.ulOutputEnableAxis = ProgData.ulCmdAxis;

    //
    ProgData.lPinc[pAxisNO[0]] = pEndPos[0];
    ProgData.lPinc[pAxisNO[1]] = pEndPos[1];

    //
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 100 / 6);  //fSpeed * 1000000 / 60000
    //ProgData.Data.UserCircleData.lFeedSpeed = (Bit32)(fSpeed * 1000000 );
    ProgData.Data.UserCircleData.dFeedSpeed = fSpeed;

    ProgData.Data.UserCircleData.nCircleMode = CIRCLE_MODE_MID_PT;
    ProgData.Data.UserCircleData.MidPT[0] = pMidPos[0];
    ProgData.Data.UserCircleData.MidPT[1] = pMidPos[1];

    //
    ProgData.nAxis1 = pAxisNO[0];
    ProgData.nAxis2 = pAxisNO[1];

    return CSM_AddProgDataToBuf(ulCrdNO, &ProgData);
}
