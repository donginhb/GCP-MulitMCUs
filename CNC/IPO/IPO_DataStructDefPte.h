
//插补器数据结构定义
#ifndef IPO_DATA_STRUCT_DEF_PTE_H
#define IPO_DATA_STRUCT_DEF_PTE_H


#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"

#include "Version/Version.h"


#pragma pack(push)
#pragma pack(8)        

//----------------------------------------------------------------------------------------------------------

#define IPO_CPU_DUL_CORE            (2)                    //定义插补器CPU为双核
//#define IPO_CPU_SINGLE_CORE            (1)                    //定义插补器CPU为双核


//
#define ENDSPEED_SMOTH_FLAG            (999999.0)            //(前瞻模式下)段末速度平滑过度标识值

//插补运动模式数据结构体
typedef struct _IPOMotionDataPte {
    //以下参数由G代码程序设定
    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];            //当前段终点坐标（轴绝对坐标），单位：um
                                                //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
                                                //说明：数组中坐标值存放顺序按照坐标系坐标轴映射表中各坐标轴出现的先后顺序存放
    uBit16        nCmdType;
    uBit16      nReserval;

    float32        dFeedRate;                        //进给速度
                                                //说明：如果是G00，解释器自动将该值设置为系统最大快移速度，插补器直接取该值进行速度控制
    float32        dEndSpeed;                        //插补段终点速度， 当取值为ENDSPEED_SMOTH_FLAG时，表示(前瞻使能)段末速度平滑过度
                                                //如果设置了段末速度平滑过度，运动程序中设置的终点速度无效

    union
    {
        CIRCLE_DATA         Circle;
        DWELL_DATA         Dwell;
        CYLSPIRAL_DATA   CylSpiral;
    }Data;

    //以下参数由计算求得
    double64    dStartCrd[CRDSYS_AXIS_MAX_COUNT];        //当前运动段起点坐标，单位：um
    double64    dSegmentLen;                    //轨迹长度(合成增量)，单位： um
    float32        dCornerAngleCos;                //当前插补程序段与上一程序段之间矢量夹角的余弦值
    float32        dAxisCos[CRDSYS_AXIS_MAX_COUNT];        //每个轴的斜率（当运动段为圆弧或螺旋线时，该值不参与位移投影计算，设置为1）
    uBit32      ulMotionAxisMask;                //本程序段参与运动的轴标示，bit0-bit7分别对应8个轴，位为1表示当前段该坐标轴参与运动，位为0表示当前段坐标轴不参与运动
                                                //（说明：该参数可能需要插补器计算）


    //以下参数由前瞻计算求得
    float32       dPlanStartSpeed;                    //(计划)插补段起点速度，综合考虑以下三方面：
                                                //系统加速度决定的拐角速度限制
                                                //拐角误差决定的拐角速度限制
                                                //系统加减速能力限制
    float32    dCornerSpeedLim;                    //拐角速度限制


}IPO_MOTION_DATA_PTE;


//花样机运动模式数据结构体
typedef struct _StitchMotionDataPte {
     double64    dCrd[CRDSYS_AXIS_MAX_COUNT];            //当前段终点坐标（轴绝对坐标），单位：um
                                                //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
                                                //说明：数组中坐标值存放顺序按照坐标系坐标轴映射表中各坐标轴出现的先后顺序存放
   
    uBit16        nCmdType;                        //(保留)
    uBit16      nReserval;

    float32        dMaxSpeed;                        //最大速度，单位：um / ms
    
    Bit32       lUQeiPos;                        //当前U轴编码器位置, 单位：内部脉冲
    float32     dUQeiSpeed;                     //当前U轴编码器参考速度，单位：内部脉冲/ms   
    float32     dFeedEndPos;                    //送料终止位置

    //以下参数由计算求得(由解释器计算)
    double64    dStartCrd[CRDSYS_AXIS_MAX_COUNT];        //当前运动段起点坐标，单位：um
    double64    dSegmentLen;                    //轨迹长度(合成增量)，单位： um
    float32        dAxisCos[CRDSYS_AXIS_MAX_COUNT];        //每个轴的斜率
    uBit16      nMotionAxisMask;                //本程序段参与运动的轴标示，bit0-bit7分别对应8个轴，位为1表示当前段该坐标轴参与运动，位为0表示当前段坐标轴不参与运动
    
}STITCH_MOTION_DATA_PTE;


//运动段数据结构体(说明：该数据结构的长度需保证是偶数字节，以满足字节对齐要求)
typedef struct _SegmentMotionDataPte {
    uBit32        ulIPOLineNO;            //插补器内部运行行号(该行号由插补器内部生成、内部管理)
    uBit32        ulProgLineNO;            //程序行号（解释器解析出来的指令单元行号，包含了G代码、M-S-T指令等， 该行号从1开始，连续无限累加，不可重复）
    uBit16        nMotionMode;            //运动模式

    union
    {
        IPO_MOTION_DATA_PTE        IPO;    //插补模式运动数据
        PT_MOTION_DATA            PT;        //PT模式运动数据
        STITCH_MOTION_DATA_PTE  Stitch;    //花样机运动数据
        //PVT_MOTION_DATA                PVT[CRDSYS_AXIS_MAX_COUNT];
    };

}SEGMENT_MOTION_DATA_PTE;



//插补运算执行步骤
#define IPO_AUTO_SEGMENT_EXE_END                (0)            //插补结束
#define IPO_AUTO_SEGMENT_EXE_GOTONEXT            (1)            //准备进入下一段
#define IPO_AUTO_SEGMENT_EXE_PREPROCESS            (2)            //预处理
#define IPO_AUTO_SEGMENT_EXE_DWELL                (3)            //执行暂停
#define IPO_AUTO_SEGMENT_EXE_CHEK_ESTOP            (4)        //检测急停
#define IPO_AUTO_SEGMENT_EXE_RECDATA            (5)            //接收数据
#define IPO_AUTO_SEGMENT_EXE_SPEEDCTRL            (6)            //加减速规划
#define IPO_AUTO_SEGMENT_EXE_INTERPOLATION        (7)            //插补输出



//插补器内部状态数据结构体
typedef struct _IPOStateDataPte {
    uBit32        ulIPOLineNO;            //插补器内部运行行号

    uBit16        nCurLineExeStep;        //当前行执行步骤

    uBit16      nNewStart;                //新的循环启动

    uBit32        ulDwellTime;            //暂停执行时间，单位：ms
    uBit32        ulWaitLookaheadTime;    //等待前瞻模块的时间，单位：ms

    //uBit16        nSingleSegmentRunFlag;  //单段运行标记

}IPO_STATE_DATA_PTE;


//定义手动模式运动数据结构体
typedef struct _JogMotionData {
    unsigned short   nMotionMode;               //手动运动模式
    unsigned short   nVaild;                    //当前数据是否有效

    union
    {
        POSCTRL_MOTION_DATA  PosCtrlMotion;
        SPEEDCTRL_MOTION_DATA  SpeedCtrlMotion;
    };

}JOG_MOTION_DATA;


//-------------------------------------------------------------------------------------------------------------------
#define TIME_SECTION_MAX            (7)            //每一段轨迹的最大时间段数


//程序段运动控制数据结构
typedef struct _SegmentMotionCtrlData{

    //---------------------------------------------M0-程序启动时初始化的数据-----------------------------------------------
    //时基控制量
    float32 dTimeCycle;                    //插补周期，单位：ms
    float32 dTimebaseSlewRate;            //坐标系时基转换速率，单位：毫秒/插补周期
    float32 dFeedHoldSlewRate;            //坐标系进给保持减速速率，单位：毫秒/插补周期
    float32 dStopSlewRate;                //坐标系运动停止减速速率，单位：毫秒/插补周期


    //精度控制量
    float32 dCircleErrorLim;            //圆弧误差限制，单位：um
    float32 dCornerPrecision;            //拐角误差，单位：um
    unsigned long ulMinConstantVelTime;    //最小匀速段时间，单位：ms

    float32 dMaxCornerAcc;                //拐弯最大加速度，单位：um/ms2

    //---------------------------------------------M0-每段程序开始时初始化的数据-----------------------------------------------
    float32 dStartSpeed;                //起始速度，单位：um/ms
    float32 dEndSpeed;                    //终点速度，单位：um/ms
  
    //
    double64 dStartCrd[CRDSYS_AXIS_MAX_COUNT];    //当前程序段起点坐标，单位：um
                                        //说明：这里的起点坐标为编程起点，由于段间可能存在过渡段，编程起点不一定等于上一段的查补终点
                                        //说明：这里的坐标是坐标系轴位置，而不是电机轴位置

    //---------------------------------------------M0-每个插补周期需更新的数据-----------------------------------------------

    float32        dFeedOverride;            //进给速度修调，取值返回0.0-1.0
    float32        dProgFeedRate;            //程序设定进给速度，单位：0.001计数/毫秒

    //当前的指令位置和脉冲位置




    //---------------------------------------------M4-负责计算的数据-----------------------------------------------
    //加减速控制量(综合考虑坐标系和单个轴的速度加速度限制)
    float32 dMaxAcc;                     //最大加速度，单位：um/ms2
    float32 dMaxDec;                     //最大减速度，单位：um/ms2
    float32 dAccJerk;                    //加速度捷度，单位：um/ms3  
    float32 dDecJerk;                    //减速度捷度，单位：um/ms3
    float32 dMaxSpeed;                     //最大速度，单位：um/ms

    double64 dDis;                        //位移量


    //
    double64 dTimeSection[TIME_SECTION_MAX];        //分段时间(分段结束点时间)
    double64 dDisSection[TIME_SECTION_MAX];            //分段位移(分段结束点位移)
    float32  dSpeedSection[TIME_SECTION_MAX];        //分段速度(分段起始点速度)
  
    //
    float32 dFeedTimeBase;                //当前进给插补时基，单位：ms
    float32 dRapidTimeBase;                //当前快移插补时基，单位：ms
    double64 dExeTotalFeed;                //当前程序段插补合成位移累计进给总量（实时），单位：um
    double64 dExeTotalTime;                //当前程序段插补累计时间（实时）
    double64 dExeCrdPos[CRDSYS_AXIS_MAX_COUNT];    //当前程序段插补位置坐标，单位：um
                                        //说明：这里的坐标是坐标系轴位置，而不是电机轴位置
  
    //
    unsigned short nExeStep;            //插补执行步骤

    //
    float32 dAccToMaxAccTime;            //加加速到最大加速度的时间
    float32 dDecToMaxDecTime;            //加减速到最大减速度的时间
    float32 dSpeedIncInAcc;                //加加速到最大加速度再减加速到0加速度过程中增加的速度量
    float32 dSpeedDecInDec;                //加减速到最大减速度再减减速到0减速度过程中减少的速度量

    //
    //unsigned long ulDwellTime;            //暂停执行时间，单位：ms
    //unsigned long ulWaitLookaheadTime;  //等待前瞻模块的时间，单位：ms

    //
    unsigned short nMotionAxisMask;        //本程序段参与运动的轴标示，bit0-bit7分别对应8个轴，位为1表示当前段该坐标轴参与运动，位为0表示当前段坐标轴不参与运动

}SEGMENT_MOTION_CTRL_DATA;


//定义运动单元速度控制方式
#define MOTION_SPEED_QICKACC            (1)        //加加速
#define MOTION_SPEED_CONSTACC            (2)        //匀加速
#define MOTION_SPEED_SLOWACC            (3)        //减加速
#define MOTION_SPEED_CONSTSPEED            (4)        //匀速
#define MOTION_SPEED_QICKDEC            (5)        //加减速
#define MOTION_SPEED_CONSTDEC            (6)        //匀减速
#define MOTION_SPEED_SLOWDEC            (7)        //减减速

#define MOTION_KEEP_CONSTANTSPEED_TIME    (100000000000.0)        //一直保持匀速运动时间标志

//运动单元数据结构体
typedef struct _MotionUnitData {
    uBit16        nSpeedCtrlMode;            //速度控制方式

    float32     dJerk;                            //捷度
    float32     dAcc;                            //运动单元起始加速度
    float32     dSpeed;                            //运动单元起始速度
    double64    dDis;                            //运动单元终点位移
    double64    dTime;                            //运动单元终点时间
}MOTION_UNIT_DATA;

//执行一个手动指令可能需要的最大运动单元数量
#define JOG_MOTION_UNIT_MAX_COUNT    (8)

//手动模式运动控制数据结构体
typedef struct _JogMotionCtrlData {
    float32        dTimeCycle;                //插补周期，单位：ms
    
    //
    float32     dAccJerk;                //最大加速段捷度
    float32        dDecJerk;                //最大减速段捷度
    float32        dMaxAcc;                //最大加速度
    float32        dMaxDec;                //最大减速度
    float32        dMaxSpeed;                //最大速度

    //
    float32        dStartAcc;                //起始加速度(非负值)
    float32        dStartSpeed;            //起始速度(非负值)
    double64    dStartPos;                //起始位置
    uBit16        nStartAccState;            //起始加减速状态， 取值为：0-匀速，1-加速， 2-减速
    
    //
    double64    dDis;                    //本次运动的位移总量
    float32        dDir;                    //本次运动方向系数，取值为-1.0或1.0

    uBit16        nChangeDirRun;            //标记当前是否正在进行换向处理，1-正在进行换向处理，0-非换向处理
                                        //说明：该值在开始进行换向处理时值1，在换向处理结束或收到新的手动指令时清0

    //
    uBit16        nMotionUnitCount;
    MOTION_UNIT_DATA MotionUnitData[JOG_MOTION_UNIT_MAX_COUNT];

    //
    uBit16        nCurUnitIndex;
    float32        dTimeBase;                    //当前插补时基，单位：ms
    double64    dExeTotalFeed;                //当前程序段插补合成位移累计进给总量（实时）
    double64    dExeTotalTime;                //当前程序段插补累计时间（实时）

    //
    float32        dAccToMaxAccTime;            //加加速到最大加速度的时间
    float32        dDecToMaxDecTime;            //减加速到最大减速度的时间
    float32        dSpeedIncInAcc;                //加加速到最大加速度再减加速到0加速度过程中增加的速度量
    float32        dSpeedDecInDec;                //加减速到最大减速度再减减速到0减速度过程中减少的速度量

}JOG_MOTION_CTRL_DATA;


//
typedef struct _RapidSpeedCtrl {
    double64 dDis;                                    //总位移
    double64 TimeSection[TIME_SECTION_MAX];            //分段时间
    double64 DisSection[TIME_SECTION_MAX];            //分段位移
    float32  SpeedSection[TIME_SECTION_MAX];        //分段起始速度
}RAPID_SPEED_CTRL;

//
typedef struct _RapidSpeedCtrlTable {
    RAPID_SPEED_CTRL*    pTableStartAddr;        //速度规划表起始地址            
    int                    iTableTotalLen;            //速度规划表总长度
    int                    iTableValidLen;            //速度规划表有效长度
    unsigned long        ulDisGap;                //位移间隔，单位：um
}RAPID_SPEED_CTRL_TABLE;



typedef struct _AxisMotionCtrl {
    union 
    {
        SEGMENT_MOTION_CTRL_DATA    RapidMotionCtrlData;    //程序段快移运动控制数据
        JOG_MOTION_CTRL_DATA        JogMotionCtrlData;        //手动模式控制数据
    };
}AXIS_MOTION_CTRL;




//----------------------------------------------------------------------------------------------------------------
//定义轴手动模式控制状态字
#define JOG_AXIS_CTRL_ESTOP                (0x00000001)            //轴手动运动急停


//----------------------------------------------------------------------------------------------------------------
//定义轴手动模式运行状态字
#define JOG_AXIS_STATUS_STOP            (0x00000001)            //轴手动运动停止
#define JOG_AXIS_STATUS_RUN                (0x00000002)            //轴手动运动中






//----------------------------------------------------------------------------------------------------------------

//
/*
typedef struct _IPODulCoreDataTrans {
    uBit16                        nAutoNewSegmentMask;

    SEGMENT_MOTION_DATA_PTE        AutoMotionData[CRDSYS_MAX_COUNT];                        //自动模式运动段数据
    SEGMENT_MOTION_CTRL_DATA    AutoMotionCtrl[CRDSYS_MAX_COUNT];                        //自动模式运动控制数据


    AXIS_MOTION_CTRL            AxisMotionCtrl[AXIS_MAX_COUNT];                            //轴单独运动控制数据区
    SEGMENT_MOTION_CTRL_DATA*    pRapidMotionCtrlData[CRDSYS_MAX_COUNT][AXIS_MAX_COUNT];    //程序段快移运动控制数据地址
    JOG_MOTION_CTRL_DATA*        pJogMotionCtrl[AXIS_MAX_COUNT];                            //手动模式控制数据地址

    uBit32                        ulJogNewSegmentMask;
    JOG_MOTION_DATA                JogMotionData[AXIS_MAX_COUNT];
    uBit32                        ulJogFinishSegmentMask;                                    //当前完成的运动段屏蔽字

    //
    double64                    PhysicalAxisCmdPos[AXIS_MAX_COUNT];                        //物理轴指令位置

}IPO_DUL_CORE_DATA_TRANS;
*/

//M0负责准备的数据
typedef struct _IPO_AUTO_M0_CTRL_DATA {
    uBit16 nNewSegment;                                //标记是否有新运动开始

    SEGMENT_MOTION_DATA_PTE    MotionData;                //自动模式运动段数据

    //
    float32 dStartSpeed;                            //起始速度，单位：um/ms
    float32 dEndSpeed;                                //终点速度，单位：um/ms

    //
    double64 dStartCrd[CRDSYS_AXIS_MAX_COUNT];        //当前程序段起点坐标，单位：um
                                                    //说明：这里的起点坐标为编程起点，由于段间可能存在过渡段，编程起点不一定等于上一段的查补终点
                                                    //说明：这里的坐标是坐标系轴位置，而不是电机轴位置

}IPO_AUTO_M0_CTRL_DATA;

//M4负责准备的数据
typedef struct _IPO_AUTO_M4_CTRL_DATA {
    uBit16        nSegmentFinish;                            //运动段插补完成


}IPO_AUTO_M4_CTRL_DATA;


//
typedef struct _IPO_JOG_M0_CTRL_DATA {
    uBit32 ulJogAxisMask;                                //有手动运动的轴掩码
    JOG_MOTION_DATA JogMotionData[AXIS_MAX_COUNT];        //手动运动数据
    double64 dAxisCmdPOs[AXIS_MAX_COUNT];                //轴当前指令位置
    float32 dAxisCmdSpeed[AXIS_MAX_COUNT];                //轴当前指令速度
    float32 dAxisAcc[AXIS_MAX_COUNT];                    //轴当前指令加速度

    uBit32 ulEStopAxisMask;                                //手动运动急停轴掩码
}IPO_JOG_M0_CTRL_DATA;

//
typedef struct _IPO_JOG_M4_CTRL_DATA {
    uBit32 ulJogRunningAxisMask;                        //正在执行手动运动的轴掩码

}IPO_JOG_M4_CTRL_DATA;



//定义插补器参数更新状态标记
#define IPO_UPDATE_END                    (0)            //插补器数据更新结束
#define IPO_UPDATE_SYS_CTRL_PARM        (1)            //插补器请求更新系统控制参数
#define IPO_UPDATE_AXIS_MAP_TABLE        (2)            //插补器请求更新坐标系映射表
#define IPO_UPDATE_RUNNING                (3)            //插补器正在执行数据更新

//
#define IPO_TIME_START_TIMING            (1)            //通知M4开始插补周期计时
#define IPO_TIME_START_INTERPOLATION    (2)            //通知M4开始插补运算

#define IPO_TIME_COUNT_START            (3)            //通知M4计时开始（不进行插补运算）
#define IPO_TIME_COUNT_END                (4)            //通知M4计时结束（不进行插补运算）


//
typedef struct _IPO_M0_M4_DATA_TRANS {
    uBit16                    nUpdateSysData;                        //M0通知M4更新坐标系映射表（M0置位，M4清除）

    //
    IPO_STATE_DATA            IPOStateData[CRDSYS_MAX_COUNT];        //M4的插补器状态

    //
    IPO_AUTO_M0_CTRL_DATA    AutoM0CtrlData[CRDSYS_MAX_COUNT];    //自动模式下M0负责设置的数据

    union {
        RAPID_SPEED_CTRL        RapidSpeedCtrl[AXIS_MAX_COUNT];        //当前可用的快移速度规划表
        RAPID_SPEED_CTRL        JogPosCtrl[AXIS_MAX_COUNT];            //当前可用的手动位置控制速度规划表
    };

    uBit32                    ulRapidSpeedCtrlMask;                //标记是否有可用的快速速度规划表
    uBit32                    ulJogPosCtrlMask;                    //标记是否有可用的手动位置控制速度规划表

    IPO_JOG_M0_CTRL_DATA    JogM0CtrlData;                        //手动模式下M0负责设置的数据

    //
    IPO_AUTO_M4_CTRL_DATA    AutoM4CtrlData[CRDSYS_MAX_COUNT];    //自动模式下M4负责设置的数据

    IPO_JOG_M4_CTRL_DATA    JogM4CtrlData;                        //手动模式下M4负责设置的数据


    //
    //double64 MotorCmdPos[AXIS_MAX_COUNT];            //物理轴指令位置（M4插补输出结果）
    IPO_AXIS_STATE_DATA        IPOAxisStateData[AXIS_MAX_COUNT];  //
    uBit32   ulMotorOutputMask;                        //电机位置输出屏蔽字（M4插补输出结果）

    ERROR_CODE    ErrorCode;

    SOFTWARE_VERSION        IPOVersion_M4;

    uBit32   ulRes1;                                //用于通知M4开始插补周期计时
    uBit32   ulRes2;

}IPO_M0_M4_DATA_TRANS;



//









#pragma pack(pop)



/*
函 数 名：
功    能：
参    数：
返 回 值：
调用位置：
注意事项：
*/




#endif