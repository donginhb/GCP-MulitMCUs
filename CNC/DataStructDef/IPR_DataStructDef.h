//解释器输出到插补器的数据结构

#ifndef _PPI_DATA_STRUCT_DEF_H_
#define _PPI_DATA_STRUCT_DEF_H_

#include "DataType.h"
//#include "SYS_DataStructDef.h" 
#include "SYS_ConfigDef.h"


#pragma pack(push)
#pragma pack(8)        


struct universe
{
    uBit32 type;
    union {
        uBit32 b;
        Bit32 i;
        double64 r;
    } data;
};


typedef struct cutter_t
{
    Bit32 tool_dir;      //刀尖方向, 铣刀为0，车刀：1－8
    float32 tool_radius;  //刀尖半径（刀具半径） (YHZ 700~799刀具长度寄存器)
    float32 length_x;    //铣刀:长度补偿（径向），车刀:刀具磨损值（径向）（相对值）
    float32 length_z;    //铣刀:长度补偿（轴向），车刀:刀具磨损值（轴向）（相对值）

    float32 offset_x;    //车刀：刀具偏置值（径向）（绝对值）= 试切时X值 - 试切直径/2
    float32 offset_z;    //车刀：刀具偏置值（轴向）（绝对值）= 试切时Z值 - 试切长度

    float32 trial_cut_diameter;  //试切直径
    float32 trial_cut_length;  //试切长度
    Bit32 trial_pos_id_x;    //试切时X轴标志
    Bit32 trial_pos_id_z;    //试切时Z轴标志
}CUTTER_T;

//插补数据命令代号：直线，圆弧，时延等：
#define    ICMD_RAPID            (0)        //    快速移动
#define    ICMD_LINE            (1)        //    直线
#define    ICMD_CW                (2)        //    顺时针圆弧
#define    ICMD_CCW            (3)        //    逆时针圆弧
#define    ICMD_DWELL            (4)        //    延时
#define    ICMD_HOME            (28)    //    回零
#define    ICMD_THREAD            (33)    //    螺纹
#define ICMD_TAP            (34)    //    攻丝
#define ICMD_S_MAX_MIN        (46)    //    设置恒线速度S下限
#define    ICMD_SDIR_RAPID        (60)    //    ICMD_SDIR_RAPID

// define iip_flag for iip_buf
#define    IFLAG_SYNCHRO        (0x0001)    //转进给标志
#define    IFLAG_FINESTOP        (0x0002)    //需要准停标志
#define    IFLAG_SBLK_EN        (0x0004)    //单段使用
#define IFLAG_CIRCLE        (0x0008)    //整圆插补标志
#define    IFLAG_ESC_EN        (0x0010)    //跳段标志
#define    IFLAG_BLK_BEG        (0x0020)    //某段G代码插补开始
#define    IFLAG_BLK_END        (0x0040)    //某段G代码插补结束
#define    IFLAG_ABS_CRD        (0x0080)    //标记坐位为绝对坐标


#define    IFLAG_CIRCLE_USER    (0x8000)    //标记圆弧数据来源于上位机用户


#define    SIG_BLK_BEG            (0x0100)    //段开始
#define    SIG_BLK_END            (0x0200)    //段结束


//定义专用于IO操作的M代码值
#define M_CODE_IO_SET        (999)

//M代码参数数据
typedef struct _MCodeParm {
    uBit32 ulParm1;            //当前M代码为M_CODE_IO_SET，表示IO设备编号
    uBit32 ulParm2;            //当前M代码为M_CODE_IO_SET，表示IO状态位掩码
    uBit32 ulParm3;            //当前M代码为M_CODE_IO_SET，表示IO状态位状态值
    uBit32 ulParm4;            //当前M代码为M_CODE_IO_SET，表示延时时间，即执行IO状态设置后，延迟指定时间，再反转IO状态
}MCODE_PARM;


//定义圆弧描述模式
#define CIRCLE_MODE_CENTER_PT            (1)
#define CIRCLE_MODE_RADIUS                (2)
#define CIRCLE_MODE_MID_PT                (3)


//定义圆弧数据
typedef struct _UserCircleData {
    uBit16  nCircleMode;
    float32 dFeedSpeed;       //进给速度，单位：用户计数单位/ms

    union
    {
        Bit32 CenterPT[2];   //圆心坐标（相对于圆弧起点的增量值），单位：用户计数单位
        Bit32 MidPT[2];      //圆弧中间点，单位：用户计数单位
        Bit32 iRadius;         //圆弧半径，单位：用户计数单位；半径大于0，表示圆弧小于或等于180度；半径小于0，表示圆弧大于180度
    };

}USER_CIRCLE_DATA;





typedef struct iip_buf
{
    Bit16 nCmd;                    //命令：直线，圆弧，时延等，其值可为：
                                //    ICMD_RAPID：        快速移动
                                //    ICMD_LINE：            直线
                                //    ICMD_CW：            顺时针圆弧
                                //    ICMD_CCW：            逆时针圆弧
                                //    ICMD_DWELL：        延时
                                //    ICMD_HOME：            回零
                                //    ICMD_THREAD：        螺纹
                                //    ICMD_TAP            攻丝
                                //    ICMD_SDIR_RAPID        单方向定位

    Bit16 nFlag;                //插补标志：
                                //    IFLAG_SYNCHRO：        转进给标志
                                //    IFLAG_FINESTOP：    需要准停标志
                                //    IFLAG_SBLK_EN：        单段使用
                                //    IFLAG_CIRCLE：        圆弧
                                //    IFLAG_ESC_EN        跳段标志
                                //    IFLAG_BLK_BEG：        某段G代码插补开始
                                //    IFLAG_BLK_END：        某段G代码插补结束
                                //    IFLAG_ABS_CRD        标记坐位为绝对坐标

    Bit16  nMCode;                //M代码值
    Bit16  nTCode;                //刀具代码
    Bit16  nBCode;                //B代码
    Bit16  nSCode;                //主轴转速
    uBit32 ulCmdAxis;            //命令轴 （表示哪些轴有运动数据）
    uBit32 ulOutputEnableAxis;        //输出使能轴 （表示哪些轴可以输出）
    Bit32  lPinc[CRDSYS_AXIS_MAX_COUNT];    //轴的增量，单位：um
    Bit32  lPlong;                    //取值根据此结构中nCmd取值不同而不同 ，具体含义分别如下 ：
                                    //    当 nCmd = ICMD_RAPID    快速移动表示的意义为：CRDSYS_AXIS_MAX_COUNT轴的合成增量 lPlong = lPinc[0] *  lPinc[0] + lPinc[1] *  lPinc[1] + .....lPinc[AXIS_MAX_COUNT-1] *  lPinc[AXIS_MAX_COUNT-1] 
                                    //    当 nCmd = ICMD_LINE        直线表示的意义为：CRDSYS_AXIS_MAX_COUNT轴的合成增量 lPlong = lPinc[0] *  lPinc[0] + lPinc[1] *  lPinc[1] + .....lPinc[AXIS_MAX_COUNT-1] *  lPinc[AXIS_MAX_COUNT-1] 
                                    //    当 nCmd = ICMD_CW        顺时针圆弧表示的意义为：CRDSYS_AXIS_MAX_COUNT轴的合成增量 lPlong = lPinc[0] *  lPinc[0] + lPinc[1] *  lPinc[1] + .....lPinc[AXIS_MAX_COUNT-1] *  lPinc[AXIS_MAX_COUNT-1] 
                                    //    当 nCmd = ICMD_CCW        逆时针圆弧表示的意义为：CRDSYS_AXIS_MAX_COUNT轴的合成增量 lPlong = lPinc[0] *  lPinc[0] + lPinc[1] *  lPinc[1] + .....lPinc[AXIS_MAX_COUNT-1] *  lPinc[AXIS_MAX_COUNT-1] 
                                    //    当 nCmd = ICMD_DWELL    延时没有意义
                                    //    当 nCmd = ICMD_HOME        回零没有意义
                                    //    当 nCmd = ICMD_THREAD    螺纹表示的意义为：CRDSYS_AXIS_MAX_COUNT轴中增量最大的轴的绝对值
                                    //    当 nCmd = ICMD_TAP        攻丝没有意义



    union {                        //插补参数

        struct                    //直线或圆弧的参数
        {
            float32 dFeedSpeed;    //速度， 单位: 时间进给    -- 用户计数单位/ms
                                //               转进给    -- 用户计数单位/转
            Bit32 i,j;            //圆弧或者整园的圆心位置，相对起点的增量值，单位：用户计数单位
            Bit16 d_i,d_j;        //圆弧起点到终点的距离， d_i表示平面的第一轴， d_j表示平面的第二轴， 单位为：用户计数单位
        }normal;
    
        struct                    //螺纹数据
        {
            Bit32 lLead;        //螺距，单位：用户计数单位
            Bit32 iDepth;        //螺纹深度，单位：用户计数单位
            Bit32 lTime;        //攻丝孔底等待时间，单位：用户计数单位
        }Thread;

        struct                    //时延
        {
            Bit32 lReserved[2];
            Bit32 lTime;        //单位：毫秒
        }Dwell;

        struct                    //恒速切削上下限
        {
            Bit32 max;            //上限（物理意义，单位：转/分钟）
            Bit32 min;            //下限（物理意义，单位：转/分钟）
        }Spdl;

        //
        MCODE_PARM MCodeParm;   //M代码参数

        //
        USER_CIRCLE_DATA UserCircleData;   //上位机用户圆弧数据
    }Data;

    void *pNptr;
    Bit16 nAxis1;            //所在平面的两个轴——第一个
    Bit16 nAxis2;            //所在平面的两个轴——第二个
                            //(1)G17表示选择XY平面-----X轴定义为 0 / U轴定义为 6 -- Y轴定义为 1 / V轴定义为 7
                            //                    nAxis1=(x&U_MSK)? 6:0;
                            //                    nAxis2=(x&V_MSK)? 7:1;
                            //(2)G18表示选择ZX平面-----Z轴定义为2 / W轴定义为 8  -- X轴定义为 0 / U轴定义为 6
                            //                    nAxis1=(x&W_MSK)? 8:2;
                            //                    nAxis2=(x&U_MSK)? 6:0;
                            //(3)G19表示选择YZ平面-----Y轴定义为1/V轴定义为 7  -- Z轴定义为 2 / W轴定义为 8
                            //                    nAxis1=(x&V_MSK)? 7:1;
                            //                    nAxis2=(x&W_MSK)? 8:2;
    struct iip_buf *pNext, *pLast;    //插补数据链表的前后指针
    Bit16 nNullCnt;            //表示不能产生系统命令的段数（比如：空行、宏指令、O代码号等）

    uBit32 ulUserLineNO;        //用户自定义行号

}IIP_BUF;

#pragma pack(pop)
#endif

