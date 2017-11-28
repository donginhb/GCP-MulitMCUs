
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：AXIS_DataStructDef.h 

摘    要：定义轴数据结构

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：1、规范参数名
          2、去掉控制状态及运行状态位域定义
          3、明确数据意义，去掉意义不明的数据

当前版本：2.0

修改作者：毛军

修改日期：2013年11月15日

//-----------------------------------------------------------------------------------------------
修改描述：1、轴补偿参数数据结构体定义拆分为反向间隙补偿和螺距补偿
          2、AXIS_SIGNAL_CONFIG中增加编码器使能位，如果未使能，系统将不检测编码器反馈
          3、增加回零信号电平设置、伺服信号电平设置及有效性设置

当前版本：2.2

修改作者：毛军

修改日期：2014年2月11日

//-----------------------------------------------------------------------------------------------
修改描述：1、轴状态数据结构体中轴脉冲位置由ubit32该为double64，以适应任意角度旋转轴的插补运算

当前版本：2.3

修改作者：毛军

修改日期：2014年5月19日

//-----------------------------------------------------------------------------------------------
修改描述：1、轴状态数据中增加伺服状态和伺服报警

当前版本：3.0

修改作者：毛军

修改日期：2015年8月22日

//-----------------------------------------------------------------------------------------------
修改描述：1、修改回零方式

当前版本：4.0

修改作者：毛军

修改日期：2016年1月6日

//-----------------------------------------------------------------------------------------------

修改描述：1、修改了轴运行状态定义
          2、增加了编码器方向和电机运动方向反转设置信号量
          3、明确了参数取值范围

当前版本：5.0

修改作者：毛军、朱润平

修改日期：2016年1月16日
*/

#ifndef    AXIS_DATA_STRUCT_DEF_H
#define AXIS_DATA_STRUCT_DEF_H

#include "DataType.h"
#include "SYS_ConfigDef.h"

#pragma pack(push)
#pragma pack(8)        




//------------------------------------------------------------------------------
//轴控制字位定义
#define AXIS_CTRL_ENABLE                 (0X00000001) //轴加使能
#define AXIS_CTRL_DISABLE                 (0X00000002) //轴断使能
#define AXIS_CTRL_CLRALM                (0X00000004) //轴报警清除
#define AXIS_CTRL_HOME_CW                 (0X00000008) //轴正向回零
#define AXIS_CTRL_HOME_CCW              (0X00000010) //轴负向回零
#define AXIS_CTRL_HOME_STOP                (0X00000020) //回零停止
#define AXIS_CTRL_REQUIRE_ID             (0X00000040) //请求芯片ID
#define AXIS_CTRL_LED_BLINK                (0X00000080) //设置LED灯常亮5秒
#define    AXIS_CTRL_LOCK                    (0x00000100) //轴锁定
#define    AXIS_CTRL_RESET                    (0x00000200) //轴复位
#define AXIS_CTRL_ENABLE_VERIFY            (0x00000400) //使能校验模式
#define AXIS_CTRL_DISABLE_VERIFY        (0x00000800) //使能校验模式
#define AXIS_CTRL_UPDATE                (0x00001000) //更新应用程序
#define AXIS_CTRL_UPDATE_BOOT             (0x00002000) //更新BootLoader

//总线伺服专用控制状态位
#define AXIS_CTRL_REQUIRE_INDEX         (0x00010000) //请求捕捉INDEX信号
#define AXIS_CTRL_CCW_DISABLE            (0x00020000) //CCW驱动禁止
#define AXIS_CTRL_CW_DISABLE             (0x00040000) //CW驱动禁止
#define AXIS_CTRL_OFFSET_RESET          (0x00080000) //位置偏差清零/速度选择1/零速箝位
#define AXIS_CTRL_PULSE_DISABLE            (0x00100000) //指令脉冲禁止
#define AXIS_CTRL_CCW_TORQUE             (0x00200000) //CCW(反转)转矩限制
#define AXIS_CTRL_CW_TORQUE                (0x00400000) //CW(正转)转矩限制

#define AXIS_CTRL_REC_SYS_ERROR            (0x80000000)    //记录系统运行错误（系统设计问题，致命错误）

//------------------------------------------------------------------------------
//轴状态位定义
#define AXIS_STATUS_ENABLE                        (0x00000001)    //轴使能（1-伺服已使能，0-伺服未使能）                                （设备端处理）
#define AXIS_STATUS_SV_ALARM                    (0x00000002)    //伺服报警（伺服输入IO, 1-伺服有报警，0-伺服无报警）（致命错误）    （设备端处理）
#define AXIS_STATUS_ARRIVE                        (0x00000004)    //伺服到位（伺服输入IO, 1-伺服到位，0-伺服未到位）                    （设备端处理）
#define AXIS_STATUS_INDEX                        (0x00000008)    //轴Index信号（伺服输入IO, 1-有索引信号，0-无索引信号）                （设备端处理）
#define AXIS_STATUS_READY                        (0x00000010)    //伺服准备好（伺服输入IO, 1-伺服准备好，0-伺服未准备好）            （设备端处理）
#define AXIS_STATUS_LEFT_LIMIT                    (0x00000020)    //左硬限位信号                                                        （设备端处理）
#define AXIS_STATUS_RIGHT_LIMIT                    (0x00000040)    //右硬限位信号                                                        （设备端处理）
#define AXIS_STATUS_HOME_SWITCH                    (0x00000080)    //零位信号                                                            （设备端处理）
#define AXIS_STATUS_USER_DEF_INPUT1                (0x00000100)    //用户自定义输入点1（回零减速信号）                                    （设备端处理）
#define AXIS_STATUS_USER_DEF_INPUT2                (0x00000200)    //用户自定义输入点2                                                    （设备端处理）
#define AXIS_STATUS_HOME_SUCESS                    (0x00000400)    //回零成功                                                            （设备端处理）
#define    AXIS_STATUS_HOME_RUNNING                (0x00000800)    //轴正在回零                                                        （设备端处理）
#define AXIS_STATUS_ZERO_SPEED                    (0x00001000)    //轴零速（连续一定时间满足以下条件：1-无位置指令，2-电机运动停止）   （设备端处理）
#define AXIS_STATUS_DEV_PROCESS_MASK            (0x00001FFF)    //设备端负责处理的轴状态位

#define    AXIS_STATUS_FSTOP                        (0x00002000)    //轴已精确定位停止（连续一定时间满足以下条件：1-轴零速，2-指令位置与编码器位置之间的差值小于定位误差）  （设备管理层处理）
#define    AXIS_STATUS_LOCATION_ERR                (0x00004000)    //轴定位误差过大                                                     （设备管理层处理）
#define    AXIS_STATUS_FOLLOW_ERR                    (0x00008000)    //轴跟随误差过大                                                     （设备管理层处理）
#define AXIS_STATUS_LEFT_SOFT_LIMIT                (0x00010000)    //左软限位报警                                                        （设备管理层处理）
#define AXIS_STATUS_RIGHT_SOFT_LIMIT            (0x00020000)    //右软限位报警                                                        （设备管理层处理）
#define    AXIS_STATUS_VERIFY                        (0x00040000)    //校验模式（不发脉冲）                                                （设备管理层处理）
#define AXIS_STATUS_OFFLINE                        (0x00080000)    //轴脱离总线(致命错误）                                                （设备管理层处理）
#define AXIS_STATUS_CMDPOS_ERR                    (0x00100000)    //指令位置错误（上位机和下位机命令位置不一致）                        （设备管理层处理）
#define AXIS_STATUS_SEND_CYCLE_DATA_FAIL        (0x00200000)    //发送周期数据失败                                                    （设备管理层处理）
#define AXIS_STATUS_FEED_OUT_OF_RANGE            (0x00400000)    //轴单周期内插补进给量超限                                            （设备管理层处理）
#define    AXIS_STATUS_ALARM                        (0x00800000)    //轴模块报警（表示轴模块存在可可恢复型报警）                         （设备管理层处理）
#define    AXIS_STATUS_FAIL                        (0x01000000)    //致命错误（表示轴模块存在不可恢复型报警）                             （设备管理层处理）
#define AXIS_STATUS_SYS_ERROR                    (0x02000000)    //系统运行出错（系统设计问题，致命错误）

//
#define AXIS_STATUS_BRAKE                        (0x10000000)    //伺服抱闸
#define AXIS_STATUS_SPD_REACH                    (0x20000000)    //速度到达
#define AXIS_STATUS_CCW_TORQUE                     (0x40000000)    //CCW(反转)转矩限制
#define AXIS_STATUS_CW_TORQUE                    (0x80000000)    //CW(正转)转矩限制


//轴可恢复的报警状态位-------------------
/*
#define AXIS_STATUS_SV_ALARM                    (0x00000002)    //伺服报警（伺服输入IO, 1-伺服有报警，0-伺服无报警）（致命错误）    （设备端处理）
#define AXIS_STATUS_LEFT_LIMIT                    (0x00000020)    //左硬限位信号                                                        （设备端处理）
#define AXIS_STATUS_RIGHT_LIMIT                    (0x00000040)    //右硬限位信号                                                        （设备端处理）
#define    AXIS_STATUS_LOCATION_ERR                (0x00004000)    //轴定位误差过大                                                     （设备管理层处理）
#define    AXIS_STATUS_FOLLOW_ERR                    (0x00008000)    //轴跟随误差过大                                                     （设备管理层处理）
#define AXIS_STATUS_LEFT_SOFT_LIMIT                (0x00010000)    //左软限位报警                                                        （设备管理层处理）
#define AXIS_STATUS_RIGHT_SOFT_LIMIT            (0x00020000)    //右软限位报警                                                        （设备管理层处理）
#define AXIS_STATUS_FEED_OUT_OF_RANGE            (0x00400000)  //轴单周期内插补进给量超限                                            （设备管理层处理）
*/
#define AXIS_ALARM_STATUS_BIT_MASK                (0x0043C062)    

//轴模块致命错误状态位-------------------
/*
#define AXIS_STATUS_OFFLINE                        (0x00080000)    //轴脱离总线(致命错误）                                                （设备管理层处理）
#define AXIS_STATUS_CMDPOS_ERR                    (0x00100000)    //指令位置错误（上位机和下位机命令位置不一致）                        （设备管理层处理）
#define AXIS_STATUS_SEND_CYCLE_DATA_FAIL        (0x00200000)  //发送周期数据失败                                                    （设备管理层处理）
#define AXIS_STATUS_SYS_ERROR                    (0x02000000)    //系统运行出错（系统设计问题，致命错误）
*/
#define AXIS_ERROR_STATUS_BIT_MASK                (0x02380000)                        

//说明：用户计数单位一般采用微米或0.001度

//电机轴运行状态数据结构体
typedef struct _AxisStateData {
    uBit32                    ulCtrlStatus;        //轴控制字
    uBit32                    ulRunningStatus;    //轴状态字

    double64                dCmdPos;            //轴指令位置，单位：用户计数单位
    double64                dPulsePos;            //轴脉冲位置，单位；脉冲数     
    float32                    dCmdSpeed;            //轴当前指令速度，单位：用户计数单位/毫秒
    float32                    dQeiSpeed;            //轴当前编码器反馈速度，单位：用户计数单位/毫秒
    float32                    dAcc;                //轴当前加速度，单位：用户计数单位/平方毫秒
    Bit32                    lQeiPos;            //轴编码器位置，单位；脉冲数
    Bit32                    lPinc;                //一个周期的进给脉冲数

    uBit32                    ulSvStatus;         //伺服状态
    uBit32                    ulSvAlarm;            //伺服报警
}AXIS_STATE_DATA;


//------------------------------------------------------------------------------

//轴运动参数（主控M4用）
typedef struct _AxisMoveParm{
    float32        dMaxJogSpeed;                //最大手动速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dMaxJogAcc;                    //最大手动加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
    float32        dJogJerk;                    //手动捷度,单位：用户计数单位/立方毫秒，取值范围：大于0
    float32        dMaxFeedSpeed;                //最大加工速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dMaxFeedAcc;                //最大加工加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
    float32        dFeedJerk;                    //加工捷度,单位：用户计数单位/立方毫秒，取值范围：大于0
    float32        dRapidSpeed;                //快移速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dRapidAcc;                    //快移加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
    float32        dRapidJerk;                    //快移捷度,单位：用户计数单位/立方毫秒，取值范围：大于0
}AXIS_MOVE_PARM;



//轴模块信号量配置数据
typedef struct _AxisSignalConfig {                
    uBit32 SvAlarmSignal:            1;    //伺服报警信号电平，1-高电平对应伺服报警，0-低电平对应伺服报警
    uBit32 SvArrivSignal:            1;    //伺服到位信号电平，1-高电平对应伺服到位，0-低电平对应伺服到位
    uBit32 QeiIndexSignal:            1;    //编码器INDEX信号电平，1-高电平对应INDEX信号，0-低电平对应INDEX信号
    uBit32 SvReadySignal:            1;    //伺服准备好信号电平，1-高电平对应伺服准备好，0-低电平对应伺服准备好
    uBit32 SvEnableSignal:            1;    //伺服使能输出电平，1-高电平使能，0-低电平使能
    uBit32 HardLimPSignal:            1;    //轴正硬限位信号电平，1-高电平对应轴正硬限位信号，0-低电平对应轴正硬限位信号
    uBit32 HardLimNSignal:            1;    //轴负硬限位信号电平，1-高电平对应轴负硬限位信号，0-低电平对应轴负硬限位信号
    uBit32 HomeSignal:                1;    //零位信号电平，1-高电平对应零位信号，0-低电平对应零位信号
    uBit32 QeiEnable:                1;    //编码器使能
    uBit32 SvAlarmEnable:            1;  //伺服报警信号使能，1-伺服报警信号有效，0-伺服报警信号无效
    uBit32 SvArriveEnable:            1;  //伺服到位信号使能，1-伺服到位信号有效，0-伺服到位信号无效
    uBit32 SvReadyEnable:            1;  //伺服准备好信号使能，1-伺服准备好信号有效，0-伺服准备好信号无效
    uBit32 QeiReverse:              1;  //反转编码器方向，1-反转编码器方向，0-不反转编码器方向
    uBit32 MotorDirReverse:         1;  //反转电机运动方向，1-反转电机运动方向，0-不反转电机运动方向
    uBit32 UserDef1:                1;    //用户自定义信号1电平，1-高电平信号有效，0-低电平信号有效
    uBit32 UserDef2:                1;    //用户自定义信号2电平，1-高电平信号有效，0-低电平信号有效
    uBit32 Reserval:                16;
}AXIS_SIGNAL_CONFIG;


//轴安全参数
typedef struct _AxisSafeParm{
    uBit32        ulInPosBand;                //电机定位误差，单位：用户计数单位，取值范围：大于等于0；该值等于0时表示不判断电机定位误差
    uBit32        ulFollowErrorLim;            //跟随误差极限，单位：用户计数单位, 取值范围：大于等于0；该值等于0时表示不检测跟随误差
    
    Bit32        lSoftPosLimP;                //正软限位， 单位：用户计数单位，该值为0时，表示不检测正软限位
    Bit32        lSoftPosLimN;                //负软限位， 单位：用户计数单位，该值为0时，表示不检测负软限位
    
    Bit32        iReserve;                    //预留 
}AXIS_SAFE_PARM;


#define AXIS_HOME_MODE_COUNT    (4)

//轴回零参数
typedef struct _AxisHomeParm{
    Bit16        nHomeMode;                    //回零方式：
                                            //    0--接收回零开始指令后，按以下步骤回零：
                                            //       （1）电机按照nHomeDir指定的方向、以dHomeApproachSpeed指定的速度找零位信号上升沿，
                                            //       （2）检测零位信号上升沿后，以dHomeLocatSpeed指定的速度，移动lHomeOffset指定的距离，然后设置位置为lHomePos，完成回零。
                                            //    1--接收回零开始指令后，按以下步骤回零：
                                            //       （1）电机按照nHomeDir指定的方向、以dHomeRapidSpeed指定的速度找减速信号 
                                            //     （2）检测到减速信号后，以dHomeApproachSpeed指定的速度找零位信号上升沿，
                                            //       （3）检测零位信号上升沿后，以dHomeLocatSpeed指定的速度，移动lHomeOffset指定的距离，然后设置位置为lHomePos，完成回零。
                                            //    2--接收回零开始指令后，按以下步骤回零：
                                            //     （1）电机按照nHomeDir指定的方向、以dHomeRapidSpeed指定的速度找零位信号，
                                            //     （2）检查到零位信号后，以dHomeApproachSpeed指定的速度反向离开零位开关，
                                            //     （3）以dHomeLocatSpeed指定的速度找零位信号上升沿，
                                            //       （4）检测零位信号上升沿后，以dHomeLocatSpeed指定的速度，移动lHomeOffset指定的距离，然后设置位置为lHomePos，完成回零。
                                            //  3--接收回零开始指令后，按以下步骤回零：
                                            //       （1）电机按照nHomeDir指定的方向，以dHomeIndexSpeed指定的速度找INDEX信号，
                                            //     （2）找到INDEX信号后，以dHomeLocatSpeed指定的速度，移动lHomeOffset指定的距离，然后设置位置为lHomePos，完成回零
    Bit16        nHomeDir;                    //回参考点方向，1-正向回零，0-负向回零
    Bit32        lHomePos;                    //机床回参考点后，参考点在机床坐标中的位置。单位：内部脉冲当量。
    Bit32        lHomeOffset;                //电机回参考时，找到Z脉冲后并不是参考点，
                                            //再继续走一个参考点偏差值（单位：内部脉冲当量），才是参考点。

    Bit32        lLocationDis;                //单向定位偏移值，单位：内部脉冲当量
                                            //    G60单向定位时，在接近定位点从快移速度转换为定位速度时，
                                            //    坐标轴与定位点间的距离（即减速移动的位移值）。
                                            //    单向定位偏移值>0：正向定位
                                            //    单向定位偏移值<0：负向定位

    float32        dHomeRapidSpeed;            //回参考点快移速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dHomeApproachSpeed;            //回参考点接近速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dHomeLocatSpeed;            //回参考点定位速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32        dHomeIndexSpeed;            //电机(寻找INDEX)复位速度及方向，正值表示正向运动复位，负值表示负向运动复位，单位：用户计数单位/毫秒，取值范围：大于0  

}AXIS_HOME_PARM;


//轴反向间隙补偿参数
typedef struct _AxisBackLashCmpParm{
    uBit32        ulBacklashTakeupRate;        //反向间隙补偿速率，单位：用户计数单位/毫秒, 如果该值为0，则在方向时一次补偿到位，取值范围：大于等于0
    uBit32        ulBacklashSize;                //反向间隙大小，单位：用户计数单位，取值范围：大于等于0
}AXIS_BACK_LASH_CMP_PARM;

//轴螺距补偿参数
typedef struct _AxisPitchCmpParm{
    uBit16        nCmpType;                    //补偿类型  0-无  1--单向补偿  2--双向补偿
    uBit16        nCmpCount;                    //补偿段数，取值范围：大于等于0，小于等于AXIS_CMP_MAX_POINT
    Bit32        lCmpStartPos;                //补偿起始位置，单位：用户计数单位
    Bit32        iCmpEndPos;                    //补偿终止位置，单位：用户计数单位
    Bit32        iCmpGap[AXIS_CMP_DIR_COUNT][AXIS_CMP_MAX_POINT];     //补偿表
}AXIS_PITCH_CMP_PARM;


//轴比例参数
typedef struct _AxisScaleParm{
    uBit16        nType;                    //轴类型，取值为：
                                        //AXIS_TYPE_NULL                //未安装
                                        //AXIS_TYPE_LINE                //直线轴
                                        //AXIS_TYPE_ROTATE                //旋转轴(角度值任意)
                                        //AXIS_TYPE_ROTATE_360            //旋转轴(角度值为0-360)

    Bit32        lInternalPulseScale;    //内部脉冲与编码器脉冲比例分子，取值范围：大于0
    Bit32        lQEIPulseScale;            //内部脉冲与编码器脉冲比例分母，取值范围：大于0
    Bit32        lGearRatioPosScale;        //轴脉冲当量分子,单位：用户计数单位，取值范围：大于0
    Bit32        lGearRatioPlusScale;    //轴脉冲当量分母,单位：脉冲数，取值范围：大于0    
                                        //两者的商为坐标轴的实际脉冲当量，即每发一个脉冲，坐标轴移动的距离或旋转的角度
    Bit32        lCycleQeiPulseCount;    //编码器一周的脉冲数，取值范围：大于0
}AXIS_SCALE_PARM;



//轴参数
typedef struct _AxisParm{  
    AXIS_SIGNAL_CONFIG            AxisSignalConfig;    //信号量配置数据
    AXIS_SAFE_PARM                AxisSafeParm;       //安全参数
    AXIS_HOME_PARM                AxisHomeParm;       //回零参数
    AXIS_SCALE_PARM                AxisScaleParm;      //比例参数
    AXIS_MOVE_PARM                AxisMoveParm;       //运动参数
    AXIS_BACK_LASH_CMP_PARM        AxisCmpParm;        //反向间隙补偿参数
}AXIS_PARM;





#pragma pack(pop)



#endif // #ifndef AXIS_DATA_STRUCT_DEF_H

