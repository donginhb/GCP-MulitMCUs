
//MST数据结构定义
#ifndef _MST_DATA_STRUCT_DEF_H_
#define _MST_DATA_STRUCT_DEF_H_

#include "DataType.h"


#pragma pack(push)
#pragma pack(8)        

#define TCODE_MAX_COUNT        (100)            //T代码最大数量
#define MCODE_MAX_COUNT        (100)            //M代码最大数量


typedef struct _MSTStateData {
    uBit32 ulCtrl;            //MST控制状态
    uBit32 ulStatus;        //MST运行状态
    uBit32 ulAlarm;            //MST报警状态

    uBit16 nSCode;            //S代码

}MST_STATE_DATA;


/*
//----------------------------------------------------------------------------------------
//通道MST控制及状态字各位的意义说明
//        PLC通过下述宏来向通道发出MST应答信号
//            #define    ch_mst_ack(x)    (G_ui[(x)*20+163])        //通道对MST指令的响应
//        其中宏参数x表示通道号，其值可为0,1,2,3，即系统最大可有4个通道。
//    各位的定义如下：


#define    MST_CTRL_LOCK            0x0040        //MST用户锁定 ??
#define    MST_CTRL_M_FIN            0x0080        //?
#define    MST_CTRL_M_STB            0x0100        //M指令
#define    MST_CTRL_S_STB            0x0200        //S指令
#define    MST_CTRL_T_STB            0x0400        //T指令
#define    MST_CTRL_B_STB            0x0800        //B指令
#define    MST_CTRL_RUNING            0x1000        //
#define    MST_CTRL_HOLD            0x2000        //数控系统请求停止MST
#define    MST_CTRL_STOP            0x4000        //
#define    MST_CTRL_CANCEL            0x8000        //数控系统请求取消MST ?? 与stop的区别?

//    等价的位域表示
typedef struct MSTCtrlStatus
{
    uBit32 MSTLock:            1;
    uBit32 MSTResever1:        1;
    uBit32 MST_M_stb:        1;
    uBit32 MST_S_stb:        1;
    uBit32 MST_T_stb:        1;
    uBit32 MST_B_stb:        1;
    uBit32 MSTRunning:        1;
    uBit32 MSTHold:            1;
    uBit32 MSTStop:            1;
    uBit32 MSTCancel:        1;
};


#define    MST_STATUS_M_ACK            0x0001        //PLC已完成M指令
#define    MST_STATUS_S_ACK            0x0002        //PLC已完成S指令
#define    MST_STATUS_T_ACK            0x0004        //PLC已完成T指令
#define    MST_STATUS_B_ACK            0x0008        //PLC已完成B指令
#define    MST_STATUS_BUSY                0x0010        //通道正在执行MST指令
#define    MST_STATUS_HOLD_LAMP        0x0020        //MST暂停
#define    MST_STATUS_LOCK                0x0040        //MST用户锁定
#define    MST_STATUS_M_FIN            0x0080
#define    MST_STATUS_M_STB            0x0100        //M指令
#define    MST_STATUS_S_STB            0x0200        //S指令
#define    MST_STATUS_T_STB            0x0400        //T指令
#define    MST_STATUS_B_STB            0x0800        //B指令
#define    MST_STATUS_PROC_BEG            0x1000
#define    MST_STATUS_HOLD_CMD            0x2000        //数控系统请求停止MST
#define    MST_STATUS_PROC_END            0x4000
#define    MST_STATUS_CANCEL            0x8000        //数控系统请求取消MST

//    等价的位域表示
typedef struct MSTRunningStatus
{
    uBit32 MST_M_ack:            1;    //PLC已完成M指令
    uBit32 MST_S_ack:            1;    //PLC已完成S指令
    uBit32 MST_T_ack:            1;    //PLC已完成T指令
    uBit32 MST_B_ack:            1;    //PLC已完成B指令
    uBit32 MSTBusy:                1;    //通道正在执行MST指令
    uBit32 MSTHoldLamp:            1;    //MST暂停
    uBit32 MSTLock:                1;    //
    uBit32 MSTResever1:            1;
    uBit32 MST_M_stb:            1;    //M指令
    uBit32 MST_S_stb:            1;    //S指令
    uBit32 MST_T_stb:            1;    //T指令
    uBit32 MST_B_stb:            1;    //B指令
    uBit32 MSTResever4:            1;    
    uBit32 MSTHoldCmd:            1;    //数控系统请求停止MST
    uBit32 MSTResever5:            1;
    uBit32 MSTCancel:            1;    //数控系统请求取消MST
    uBit32 MSTResever16:        16;
};
*/

/*
//状态数据结构体
typedef struct _MSTStateData {
    struct str_ch_mst_ctrl    ulStatus;    //M-S-T运行状态

    uBit16        nBufReadPos;            //MST缓冲区读位置
    uBit16        nBufWritePos;            //MST缓冲区写位置
    uBit16        nBufCmdCount;            //MST缓冲区指令数量

    uBit32        ulCurExeLineNO;            //当前运行行内部行号
    uBit32        ulCurExeProgLineNO;        //当前运行行程序行号

}MST_STATE_DATA;
*/


#pragma pack(pop)


#endif/*_MST_CTRL_H_*/

