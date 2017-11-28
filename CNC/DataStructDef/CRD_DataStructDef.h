
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：CRD_DataStructDef.h 

摘    要：坐标系数据结构定义

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

//-----------------------------------------------------------------------------------------------
修改描述：1、规范参数名
          2、去掉控制状态及运行状态位域定义
          3、明确数据意义，去掉意义不明的数据

当前版本：2.0

修改作者：毛军

修改日期：2013年11月15日

//-----------------------------------------------------------------------------------------------
修改描述：1、插补器控制状态字和运行状态字定义统一到坐标系运行状态字和控制状态字
          2、坐标系运行状态数据结构体定义中增加插补器程序运行状态数据

当前版本：2.2

修改作者：毛军

修改日期：2014年1月6日

//-----------------------------------------------------------------------------------------------

修改描述：1、重新定义了坐标系状态字
          2、修改了坐标系状态数据结构体

当前版本：3.0

修改作者：毛军

修改日期：2015年9月17日
*/

#ifndef _CRD_DATA_STRUCT_DEF_H_
#define _CRD_DATA_STRUCT_DEF_H_

#include "DataType.h"
#include "SYS_DataStructDef.h"
#include "SYS_ConfigDef.h"


#pragma pack(push)
#pragma pack(8)    

//---------------------------------------------坐标系（通道）参数数据结构定义--------------------------------------
#define AXIS_MAP_MOTOR_MAX_COUNT (2)


//坐标轴映射数据
typedef struct _CrdAxisMap {
    uBit8            cAxisName[2];                            //坐标轴标示(低8位存放第一个轴标示字符，高8位存放第二个轴标示字符)，
                                                            //坐标轴标示为英文字母或英文字母与阿拉伯数字的组合（必须字母开头），字母不能是标准G代码中的关键字，例如：S、F、G
                                                            //值为0表示无轴标示
    uBit8            cMotorNO[AXIS_MAP_MOTOR_MAX_COUNT];        //cMotorNO[0]存放第一个映射电机编号，cMotorNO[1]存放第二个映射电机编号, 电机编号从0开始，取值为0XFF表示未映射电机
                                                            //说明：必须优先使用cMotorNO[0]存放电机编号
                                                            //取值范围：大于等于0（必须为有效的设备编号），值为0xFF时表示未分配电机
}CRD_AXIS_MAP;


//坐标系坐标轴映射表
typedef struct _CrdSysAxisMapTable {
    uBit16            nCrdAxisCount;                            //坐标系中的坐标轴数量，取值范围：大于等于0
    CRD_AXIS_MAP    CrdSysAxisMap[CRDSYS_AXIS_MAX_COUNT];   //坐标轴映射表（说明：坐标映射数据必须从数组的第0个元素开始连续存放）
}CRDSYS_AXIS_MAP_TABLE;


//定义坐标系控制设置位
#define  CRDSYS_SET_ENABLE                    (0x00000001)            //通道使能
#define  CRDSYS_SET_LOOKAHEAD_ENABLE        (0x00000002)            //前瞻使能

//说明：长度的计数单位为毫米，角度的计算单位为度

//坐标系参数
typedef struct CrdSysParm {
    uBit16  nCrdSysSet;                        //通道设置，各设置位的意义为：
                                            //CRDSYS_SET_ENABLE                通道使能
                                            //CRDSYS_SET_LOOKAHEAD_ENABLE    前瞻使能
    uBit16    nLookaheadLen;                    //前瞻段数，取值范围：大于等于0
    uBit32    ulMinConstantVelTime;            //最小匀速段时间，单位：豪秒，取值范围：大于等于0
    float32 dLineAxisCornerPreci;            //直线轴拐角误差，单位：用户计数单位，取值范围：大于等于0
    float32 dRotAxisCornerPreci;            //旋转轴拐角误差，单位：用户计数单位，取值范围：大于等于0
    float32    dCircleErrorLim;                //圆弧误差限制，单位：用户计数单位，取值范围：大于等于0
    float32 dProgMaxFeedSpeed;                //程序最大进给速度，单位：用户计数单位/毫秒，取值范围：大于0
    float32    dProgMaxAcc;                    //程序最大加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
    float32    dProgMaxJerk;                    //程序最大捷度，单位：用户计数单位/毫秒，取值范围：大于0
    float32    dMaxCornerAcc;                    //拐弯最大加速度，单位：用户计数单位/平方毫秒，说明：前瞻模式下该值有效，取值范围：大于0
    float32    dTimebaseSlewRate;                //坐标系时基转换速率，单位：毫秒/插补周期，取值范围：大于0，小于等于0.5
    float32    dFeedHoldSlewRate;                //坐标系进给保持减速速率，单位：毫秒/插补周期，取值范围：大于0，小于等于0.5
    float32    dStopSlewRate;                    //坐标系停止减速速率，单位：毫秒/插补周期，取值范围：大于0，小于等于0.5

    Bit32    lSpindleQeiSense;                //主轴编码器分辨率(编码器一周的脉冲数)，取值范围：大于0


    uBit32 ulReserval1;
    uBit32 ulReserval2;
}CRDSYS_PARM;


//--------------------------------------------坐标系（通道）控制状态位定义--------------------------------------------
//（通道和插补器公用的控制状态位）
#define    CRDSYS_CTRL_MODE_MASK                        (0x00000007)    //坐标系模式屏蔽位
#define    CRDSYS_CTRL_MODE_AUTO                        (1)                //自动模式
#define    CRDSYS_CTRL_MODE_JOG                        (2)                //点动模式
#define    CRDSYS_CTRL_MODE_STEP                        (3)                //步进模式
#define    CRDSYS_CTRL_MODE_HANDWHEEL                    (4)                //手摇模式
#define    CRDSYS_CTRL_MODE_HOME                        (5)                //回零模式
#define CRDSYS_CTRL_ENABLE_SGL_BLK                    (0x00000008)    //使能单段方式
#define CRDSYS_CTRL_DISABLE_SGL_BLK                    (0x00000010)    //关闭单段方式
#define    CRDSYS_CTRL_PROG_RUN                        (0x00000020)    //循环启动
#define    CRDSYS_CTRL_PROG_HOLD                        (0x00000040)    //进给保持
#define    CRDSYS_CTRL_PROG_STOP                        (0x00000080)    //程序停止
#define    CRDSYS_CTRL_PROG_ESTOP                        (0x00000100)    //程序急停
#define    CRDSYS_CTRL_PROG_CANCEL                        (0x00000200)    //程序取消
#define    CRDSYS_CTRL_PROG_STOP_AT_SEG_END            (0x00000400)    //程序在运动段结束点停止
#define    CRDSYS_CTRL_PROG_MASK                        (0x000007E0)    //程序状态屏蔽位
#define    CRDSYS_CTRL_RESET                            (0x00000800)    //坐标系复位
#define CRDSYS_CTRL_LOCATION                        (0x00001000)    //坐标系重定位（运动到指定位置）
#define CRDSYS_CTRL_IPR_PAUSE                        (0x00002000)    //解释器暂停控制位

//(通道使用的控制状态位)
//#define    CRDSYS_CTRL_DRY_RUN                            (0x00000000)    //空运行
//#define    CRDSYS_CTRL_ESC_BLK                            (0x00000000)    //跳段
    


//-----------------------------------------------坐标系运行状态位定义----------------------------------------
//（通道及插补器公用的状态位）
#define    CRDSYS_STATUS_MODE_MASK                        (0x00000007)    //坐标系模式屏蔽位
#define    CRDSYS_STATUS_MODE_AUTO                        (1)                //自动模式
#define    CRDSYS_STATUS_MODE_JOG                        (2)                //点动模式
#define    CRDSYS_STATUS_MODE_STEP                        (3)                //步进模式
#define    CRDSYS_STATUS_MODE_HANDWHEEL                (4)                //手摇模式
#define    CRDSYS_STATUS_MODE_HOME                        (5)                //回零模式
#define CRDSYS_STATUS_SGL_BLK                        (0x00000008)    //单段方式
#define    CRDSYS_STATUS_PROG_RUN                        (0x00000010)    //循环启动
#define    CRDSYS_STATUS_PROG_HOLD                        (0x00000020)    //进给保持
#define    CRDSYS_STATUS_PROG_STOP                        (0x00000040)    //程序停止
#define    CRDSYS_STATUS_PROG_CANCEL                    (0x00000080)    //程序取消
#define    CRDSYS_STATUS_PROG_MASK                        (0x000000F0)    //程序状态屏蔽位
#define    CRDSYS_STATUS_VERIFY                        (0x00000100)    //坐标系校验运行
#define    CRDSYS_STATUS_RESET                            (0x00000200)    //通道正在复位
#define    CRDSYS_STATUS_FREE                            (0x00000400)    //通道空闲
#define CRDSYS_STATUS_CUR_SEGMENT_DONE                (0x00000800)    //当前程序行完成

//（通道负责设置的状态位）
#define    CRDSYS_ALARM_TOOL_USE_OUT                    (0x00010000)    //刀具寿命已到  

//（插补器负责设置的状态位）
#define    CRDSYS_STATUS_NORMAL_CUTTING                (0x00100000)    //正在加工，非G00,G04指令
#define    CRDSYS_STATUS_THREAD_CUTTING                (0x00200000)    //正在螺纹加工
#define    CRDSYS_STATUS_ROTATE_FEED                    (0x00400000)    //转进给
#define CRDSYS_STATUS_SEG_SMOTH_TRANS                (0x00800000)    //程序段间平滑过渡

#define CRDSYS_STATUS_IPO_IN_CHARGE_MASK            (0x00F00000)    //插补器负责设置的状态位


//-----------------------------------------------坐标系报警状态位定义----------------------------------------   
//(bit0--bit15 存放插补器产生的报警)
#define CRDSYS_ALARM_CREAT_RAPID_TABLE_FAIL            (0x00000001)    //建立快移速度规划表失败
#define CRDSYS_ALARM_IPO_INVALID_CMD                (0x00000002)    //插补器无法识别的程序指令
#define CRDSYS_ALARM_PROG_SPEEDCTRL_FAIL            (0x00000004)    //程序段加减速规划失败 (通道复位时清除报警)
#define CRDSYS_ALARM_IPO_INIT_ERROR                    (0x00000008)    //插补器未正确初始化（属于程序设计错误）
#define CRDSYS_ALARM_M4_ERROR                        (0x00000010)    //M4出错（插补运算单元工作不正常）
#define CRDSYS_ALARM_SEND_FEED_ERROR                (0x00000020)    //发送插补进给量失败

#define CRDSYS_ALARM_IPO_IN_CHARGE_MASK                (0x0000FFFF)    //插补器负责设置的报警状态位

//(bit16-bit19 存放MST产生的报警)
#define CRDSYS_ALARM_MST_ERROR                        (0x00010000)    //MST模块运行出错（属于程序设计错误）
#define CRDSYS_ALARM_INVALID_MCODE                    (0x00020000)    //无效的M代码
#define CRDSYS_ALARM_INVALID_TCODE                    (0x00040000)    //无效的T代码
#define CRDSYS_ALARM_TOO_MANY_PARA_MCODE            (0x00080000)    //在执行的并行M代码数量超限
#define CRDSYS_ALARM_MCODE_EXE_FAIL                    (0x00100000)    //M代码执行失败
#define CRDSYS_ALARM_MST_IN_CHARGE_MASK                (0x00FF0000)    //MST模块负责设置的报警状态位


//(bit16--bit31 存放通道产生的报警)
#define CRDSYS_ALARM_CSM_INIT_ERROR                    (0x01000000)    //通道未正确初始化（属于程序设计错误）
#define CRDSYS_ALARM_PROG_BUF_MAN_ERROR                (0x02000000)    //程序缓冲区管理出错（属于程序设计错误）
#define CRDSYS_ALARM_PROG_INVALID_CMD                (0x04000000)    //无法识别的程序指令
#define CRDSYS_ALARM_PROG_DATA_ERROR                (0x08000000)    //程序数据错误



//#define CRDSYS_ALARM_UPDATE_BOOT_FLASH_RW_FAIL      (0x00004000)    //升级4337BOOTLOADER时写flash失败 （ 直接通道应答包返货错误码）
//#define CRDSYS_ALARM_MST_SET_ERROR                  (0x00100000)    //MST存放出错，可能数目超出，可能正在执行不能设置 （待重新定义）



//坐标系运行状态数据结构体
typedef struct _CrdSysStateData {
    double64    dTargetCrd[CRDSYS_AXIS_MAX_COUNT];    //当前段目标位置

    uBit32        ulCtrl;                    //坐标系控制字
    uBit32        ulStatus;                //坐标系状态字
    uBit32      ulAlarm;                //坐标系报警      

    uBit32      ulSCode;                // S码
    uBit16      nMCode;                    // M码
    uBit8        cTCode;                    // T码
    uBit8       cBCode;                    // B码

    uBit32      ulUserLineNO;            //用户自定义行号
    uBit32        ulCurProgLineNO;        //当前运行程序行号（解释器解析出来的指令单元行号，包含了G代码、M-S-T指令等， 该行号从1开始，连续无限累加，不可重复）
    
    uBit32        ulStartLineCount;        //记录完成了多少行，包括空行
    uBit32        ulFinishLineCount;        //记录开始了多少行，包括空行
    
    float32        dRapidOverride;            //快移速度修调，取值返回0.0-1.0
    float32        dFeedOverride;            //进给速度修调，取值返回0.0-1.0
    float32        dProgFeedRate;            //程序设定进给速度，单位：用户计数单位/毫秒
    float32        dFeedRate;                //程序当前进给速度，单位：用户计数单位/毫秒
    float32        dAcc;                    //程序当前加速度，单位：用户计数单位/平方毫秒


    uBit16        ulToolInUse;            //当前使用的刀具
    uBit16        cReserval;                //保留

    uBit32        ulSpindleSpeed;            //主轴速度
    float32        dSpindleOverride;        //主轴修调

    //
    uBit32      ulNcAxes;               //系统NC轴，只参与运算，但不输出运动
    uBit32      ulUserIn;               //用户自定义输入IO点，32路；
    uBit32      ulUserOut;              //用户自定义输出IO点，32路；

}CRDSYS_STATE_DATA;


//-------------------------------------------------------数据缓冲区状态字定义-------------------------------------------
#define    DATA_BUF_STATUS_STATIC                (0x00000001)    //数据缓存区静态工作模式，该位为0表示动态工作模式
#define DATA_BUF_STATUS_OPEN                (0x00000002)    //数据缓存区打开
#define DATA_BUF_STATUS_FULL                (0x00000004)    //数据缓存区存满
#define DATA_BUF_STATUS_HALF                (0x00000008)    //数据缓存区半空
#define DATA_BUF_STATUS_EMPTY                (0x00000010)    //数据缓存区空
#define DATA_BUF_STATUS_USESTATUSMASK        (0x0000001C)    //数据缓存区使用状态掩码



//缓冲区管理数据结构体
typedef struct _DataBufMan {
    uBit32        ulStartAdr;                //缓冲区起始地址
    uBit32        ulSize;                    //缓冲区大小
    uBit32        ulTotalSegmentCount;    //最大能存放的程序段数（当为用户缓冲区时，表示最大能存放的数据字节数）

    uBit32        ulStatus;                //数据缓冲区状态

    uBit32        ulSegmentCount;            //当前存放的程序段数（当为用户缓冲区时，表示当前存放的数据字节数）
    uBit32        ulReadPos;                //当前读位置
    uBit32        ulWritePos;                //当前写位置

}DATA_BUF_MAN;




#pragma pack(pop)




#endif  //#ifndef _CRD_DATA_STRUCT_DEF_H_