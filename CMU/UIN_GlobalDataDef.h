/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：UIN_GlobalDataDef.h 

摘    要：通信模块内部和外部接口通用数据结构和宏定义

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：1、

当前版本：3.0

修改作者：Alan 

修改日期：2015年8月28日
//-----------------------------------------------------------------------------------------------
*/
#ifndef _UIN_GLOBAL_DATA_DEF_
#define _UIN_GLOBAL_DATA_DEF_

//#define RS_MONITOR_ENALBE       //通信性能测试开关
//#define RS_RECORD_ENABLE        //使能通信出错时记录通信性能状态数据


//UIN模块传输监视数据-------------------------------上位机
typedef struct uin_rs_ms_data
{
    //发送性能检测
    unsigned long ulSendBlockCount;        //发送的数据块总数(成功或失败次数)
    unsigned long ulBlockSendSucCount;    //数据块发送成功次数
    unsigned long ulSendFailCount;        //发送数据失败次数
    
    //接收性能检测
    unsigned long ulRcvBlockCount;        //接收到完整数据块的总数(完整、非完整)
    unsigned long ulBlockRcvSucCount;    //
    unsigned long ulLastTimeOutID;        //最后一次接收超时指令ID(重试后依然超时记录)
    unsigned long ulRcvTimeOutCount;    //接收回应包超时次数
    
    //时间监测
    unsigned long ulSentBlockTime;        //发送数据块最长时间(ms)
    unsigned long ulGetResponceTime;    //等待回应包最长耗时(ms)
    
    //其他特殊监视
    unsigned long ulIOSetCmdCount;        //IO输出指令执行次数(成功发送)
    unsigned long ulRev[4];
}UIN_RS_MS_DATA;

//-------------------------------------------------下位机
//CMU模块监视数据
typedef struct cmu_rs_ms_data
{
    //发送性能检测
    unsigned long ulSendBlockCount;        //发送的数据块总数(成功或失败次数)
    unsigned long ulBlockSendSucCount;    //数据块发送成功次数
    unsigned long ulSendFailCount;        //发送数据失败次数
    
    //接收性能检测
    unsigned long ulRcvBlockCount;        //接收到完整数据块的总数(完整、非完整)
    unsigned long ulBlockRcvSucCount;    //
    
    //其他监测
    unsigned long ulOutputSetCount;        //IO输出指令执行次数
    unsigned long ulRev[4];
}CMU_RS_MS_DATA;

//CAN总线传输监控数据
typedef struct can_rs_ms_data
{
    unsigned short nSendPulseFailTimes;    //发送脉冲回应包失败次数
    unsigned short nSendRetryFailTimes;    //发送重发失败次数
    unsigned short nRcvPulseFailTimes;    //接收脉冲回应包失败次数
    unsigned short nIOSetTimes;            //IO操作次数
    unsigned short nRev[4];
}CAN_RS_MS_DATA;

typedef struct miii_rs_ms_data
{
    unsigned short ulRev[8];
}MIII_RS_MS_DATA;


typedef union bus_rs_ms_data
{
    CAN_RS_MS_DATA sCanRsMsData;
    MIII_RS_MS_DATA sMiiiRsMsData;
}BUS_RS_MS_DATA;


typedef struct ctrl_rs_ms_data
{
    CMU_RS_MS_DATA sCmuRsMsData;
    BUS_RS_MS_DATA sCanRsMsData;    
    CAN_RS_MS_DATA sMIIIRsMsData;
}CTRL_RS_MS_DATA;



//参数数据类型
#define PARM_DATA_TYPE_F                            (1)//float型
#define PARM_DATA_TYPE_I                            (2)//int型
#define PARM_DATA_TYPE_U32                          (3)//unsigned long
#define PARM_DATA_TYPE_U16                          (4)//unsigned short

typedef struct _ParmData
{
    unsigned char uParmNo;      //参数编号
    unsigned char uDataType;    //参数数据类型
    union{                      //参数数据
        float           fData;
        int             iData;
        unsigned long   ulData;
        unsigned short  nData;
    };
}PARM_DATA;

//--------------------------控制参数单项定义---------------------------------
//坐标系控制参数类型定义
#define CRD_PARM_COUNT                              (7)
#define CRD_PARM_LOOKAHEADLEN                       (1)//前瞻段数，取值范围：大于等于0
#define CRD_PARM_MINCONSTANTVELTIME                 (2)//最小匀速段时间，单位：豪秒，取值范围：大于等于0
#define CRD_PARM_LINEAXISCORNERPRECI                (3)//直线轴拐角误差，单位：用户计数单位，取值范围：大于等于0
#define CRD_PARM_ROTAXISCORNERPRECI                 (4)//旋转轴拐角误差，单位：0.001度，取值范围：大于等于0
#define CRD_PARM_CIRCLEERRORLIM                     (5)//圆弧误差限制，单位：用户计数单位，取值范围：大于等于
#define CRD_PARM_PROGMAXFEEDSPD                     (6)//程序最大进给速度，单位：用户计数单位/毫秒，取值范围：大于0
#define CRD_PARM_PROGMAXACC                         (7)//程序最大加速度，单位：用户计数单位/平方毫秒，取值范围：大于0

//电机控制参数类型定义                
#define MOTOR_PARM_COUNT                            (6)
#define MOTOR_PARM_MAXJOGSPD                        (1)//最大手动速度，单位：用户计数单位/毫秒，取值范围：大于0
#define MOTOR_PARM_MAXJOGACC                        (2)//最大手动加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
#define MOTOR_PARM_MAXFEEDSPD                       (3)//最大加工速度，单位：用户计数单位/毫秒，取值范围：大于0
#define MOTOR_PARM_MAXFEEDACC                       (4)//最大加工加速度，单位：用户计数单位/平方毫秒，取值范围：大于0
#define MOTOR_PARM_RAPIDSPD                         (5)//快移速度，单位：用户计数单位/毫秒，取值范围：大于0
#define MOTOR_PARM_RAPIDACC                         (6)//快移加速度，单位：用户计数单位/平方毫秒，取值范围：大于0


//定义通信方式---------------------------------------------------------------------
#define COM_TYPE_NULL                               (0) //没有连接
#define COM_TYPE_CAN                                (1) //CAN 通信模式
#define COM_TYPE_ENET                               (2) //ENET通信模式
#define COM_TYPE_UART                               (4) //UART通信模式
#define COM_TYPE_MAX                                (4) //最大通信模式序号


#define PROG_SEG_DATA_MASK_ABS_BIT                  (0x0001)//绝对编程标志位
#define PROG_SEG_DATA_MASK_CW_BIT                   (0x0002)//顺时针标志位

#define COM_TRANSMIT_BUF_SIZE                       (1280)  //收发缓冲区大小
#define COM_BLOCK_DATA_SIZE                         (512)   //升级文件等数据块(FLASH数据)大小
#define COM_BLOCK_FILE_SIZE                         (512)   //下载文件数据块大小
#define MIN_PERIOD_MS                               (50)    //最小查询周期


//--------------------------周期性数据指令定义-------------------------------------
//系统数据周期性数据设置
#define SYS_CMD_INQ_COUNT                           (5)
#define SYS_CMD_INQ_ALL_STATE                       (1)//系统所有状态
#define SYS_CMD_INQ_CTRL_STATUS                     (2)//系统控制状态字查询
#define SYS_CMD_INQ_RUNNING_STATUS                  (3)//系统运行状态字查询
#define SYS_CMD_INQ_EXT_ALARM                       (4)//系统外部报警查询
#define SYS_CMD_INQ_ERROR                           (5)//下位主动发送的错误

//坐标系周期性数据设置或获取指令
#define CRD_CMD_INQ_COUNT                           (22)
#define CRD_CMD_INQ_ALL_STATE                       (1)//坐标系所有状态数据查询
#define CRD_CMD_INQ_ACT_SPEED                       (2)//坐标系实际速度查询
#define CRD_CMD_INQ_CMD_SPEED                       (3)//坐标系设定速度查询
#define CRD_CMD_INQ_ACT_ACC                         (4)//坐标系实际加速度查询
#define CRD_CMD_INQ_RAPID_OVERRIDE                  (5)//坐标系快移修调查询
#define CRD_CMD_INQ_FEED_OVERRIDE                   (6)//坐标系进给修调查询
#define CRD_CMD_INQ_SPINDLE_SPEED                   (7)//坐标系主轴速度查询
#define CRD_CMD_INQ_SPINDLE_OVERRIDE                (8)//坐标系主轴修调查询
#define CRD_CMD_INQ_RUNNING_STATUS                  (9)//坐标系运行状态查询
#define CRD_CMD_INQ_ALARM                           (10)//坐标系报警状态查询
#define CRD_CMD_INQ_M_CODE                          (11)//坐标系当前运行的M码
#define CRD_CMD_INQ_S_CODE                          (12)//坐标系当前运行的S码
#define CRD_CMD_INQ_T_CODE                          (13)//坐标系当前运行的T码
#define CRD_CMD_INQ_B_CODE                          (14)//坐标系当前运行的B码
#define CRD_CMD_INQ_PROG_LINENO                     (15)//坐标系程序运行行查询
#define CRD_CMD_INQ_USER_LINENO                     (16)//坐标系程序用户自定义行号
#define CRD_CMD_INQ_PROG_BUF_STATE                  (17)//程序缓冲区数据
#define CRD_CMD_INQ_PROG_BUF_SIZE                   (18)//坐标系程序缓冲区总大小查询
#define CRD_CMD_INQ_PROG_BUF_STATUS                 (19)//坐标系程序缓冲区状态查询
#define CRD_CMD_INQ_USER_BUF_STATE                  (20)//用户缓冲区数据
#define CRD_CMD_INQ_USER_BUF_SIZE                   (21)//坐标系自定义程序缓冲区总大小查询
#define CRD_CMD_INQ_USER_BUF_STATUS                 (22)//坐标系自定义程序缓冲区状态查询
#define CRD_CMD_INQ_CTRL_STATUS                     (23)//获取坐标系控制状态


//电机周期性数据设置指令
#define MOTOR_CMD_INQ_COUNT                         (8)
#define MOTOR_CMD_INQ_ALL_STATE                     (1)//电机(或轴)所有状态数据
#define MOTOR_CMD_INQ_CTRL_STATUS                   (2)//电机(或轴)控制状态字
#define MOTOR_CMD_INQ_RUNNING_STATUS                (3)//电机(或轴)运行状态字
#define MOTOR_CMD_INQ_QEI_POS                       (4)//电机(或轴)编码器位置查询
#define MOTOR_CMD_INQ_QEI_SPEED                     (5)//电机(或轴)编码器速度查询
#define MOTOR_CMD_INQ_CMD_POS                       (6)//电机(或轴)指令位置查询
#define MOTOR_CMD_INQ_CMD_SPEED                     (7)//电机(或轴)指令速度查询
#define MOTOR_CMD_INQ_CMD_ACC                       (8)//电机(或轴)加速度查询
#define MOTOR_CMD_INQ_SV_ALRM                       (9)//电机(或轴)伺服报警查询
#define MOTOR_CMD_INQ_SV_STATUS                     (10)//电机(或轴)伺服状态查询

//IO周期性数据设置指令
#define IO_CMD_INQ_COUNT                            (5)
#define IO_CMD_INQ_ALL_STATE                        (1)//IO板系统状态
#define IO_CMD_INQ_SYS_STATUS                       (2)//获取IO板系统状态
#define IO_CMD_INQ_INPUT_STATUS                     (3)//获取IO板端口状态
#define IO_CMD_INQ_OUTPUT_STATUS                    (4)//获取IO板端口状态
#define IO_CMD_INQ_OUTPUT_CMD                       (5)//获取IO板端口状态


//ADDA周期性数据设置指令
#define ADDA_CMD_INQ_COUNT                          (5)
#define ADDA_CMD_INQ_ALL_STATE                      (1)//ADDA板所有状态
#define ADDA_CMD_INQ_SYS_STATUS                     (2)//ADDA板系统状态
#define ADDA_CMD_INQ_AD_STATUS                      (3)//ADDA板AD通道输入状态
#define ADDA_CMD_INQ_DA_STATUS                      (4)//ADDA板DA通道输出状态
#define ADDA_CMD_INQ_DA_CMD                         (5)//ADDA板DA通道指令输出状态
#endif                                        
