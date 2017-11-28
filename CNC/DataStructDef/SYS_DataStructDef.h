
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_DataStructDef.h 

摘    要：定义轴数据结构

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年08月22日
///////////////////////////////////////////
修改描述：
1.添加系统状态位定义
    #define SYS_STATUS_UPDATE_IPR_CONF_DATA         (0x00000100)    //正在更新IPR配置数据(刀库表、坐标系设置等)
    #define SYS_STATUS_UPDATE_IPR_CC_DATA            (0x00000200)    //正在更新固定循环数据

2.添加系统报警字定义
    #define SYS_ALARM_IO_OVER_CURRENT                (0x00001000)    //IO过流
    #define SYS_ALARM_DEV_OFF_LINE                    (0x00002000)    //非轴设备掉线
    #define SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL        (0x00004000)    //非轴设备发送周期性数据失败
当前版本：3.1

修改作者：Alan

修改日期：2016-8-29
*/

#ifndef _SYS_DATA_STRUCT_DEF_H_
#define _SYS_DATA_STRUCT_DEF_H_

#include "DataType.h"


#pragma pack(push)
#pragma pack(8)        




//-------------------------------------------系统控制参数定义---------------------------------------------
//定义系统控制设置位
#define SYS_SET_CRDSYSINTERRELATION            (0x0001)        //坐标系关联（同时停止）
#define SYS_SET_EN_TOOLMAN                    (0x0002)        //刀具寿命管理使能

typedef struct _SysCtrlParm{
    uBit16    nTimeCycle;                //插补周期，单位：us，取值范围：1000-4000，且为200的整数倍
    uBit16  nSysSet;                //系统设置，各设置位的意义为：
    //SYS_SET_CRDSYSINTERRELATION    坐标系关联（同时停止）
    //SYS_SET_EN_TOOLMAN            刀具寿命管理使能
    Bit32   lSysPlusMovPerUm;        //移动轴脉冲当量分母,即内部运算的最小单位为：1u/此值，取值范围：大于0    (解释器使用)                    
    Bit32   lSysPlusRotPerD;        //旋转轴脉冲当量分母,旋转轴内部运算的最小单位为：1度/(1000*此值)，取值范围：大于0    (解释器使用)    

    uBit16    nCrdSysCount;            //坐标系数量, 取值：1--CRDSYS_MAX_COUNT
}SYS_CTRL_PARM;



//-------------------------------------------系统状态数据定义---------------------------------------------

//系统控制字定义
#define    SYS_CTRL_SAVE_POWEROFF_BUF                (0x00000001)    //保存断电缓冲
#define    SYS_CTRL_SAVE_TOOL                        (0x00000002)    //保存刀具状态
#define    SYS_CTRL_SAVE_AXIS_POS                    (0x00000004)    //保存轴位置   
#define    SYS_CTRL_LOAD_PARM                        (0x00000008)    //装载参数
#define    SYS_CTRL_ESTOP                            (0x00000010)    //系统停止
#define    SYS_CTRL_RESET                            (0x00000020)    //系统复位


//--------------------------------------------系统状态字定义--------------------------------------------
#define    SYS_STATUS_ACTIVE                        (0x00000001)    //系统已启动(软件安装完成)
#define    SYS_STATUS_RESET                        (0x00000002)    //系统复位（表示当前系统正在执行复位）   
#define    SYS_STATUS_COM_CAN                        (0x00000004)    //系统采用can通信  
#define    SYS_STATUS_COM_ENET                        (0x00000008)    //系统采用以太网通信  
#define SYS_STATUS_UPDATE_APP                    (0x00000010)    //系统正在更新主控应用程序
#define SYS_STATUS_UPDATE_BOOTLOADER            (0x00000020)    //系统正在更新主控BOOTLOADER
#define SYS_STATUS_UPDATE_DEV_PROG                (0x00000040)    //系统正在更新设备端程序
#define SYS_STATUS_UPDATE_SYS_CTRL_PARM            (0x00000080)    //正在更新系统控制参数
#define SYS_STATUS_UPDATE_IPR_CONF_DATA         (0x00000100)    //正在更新IPR配置数据(刀库表、坐标系设置等)
#define SYS_STATUS_UPDATE_IPR_CC_DATA            (0x00000200)    //正在更新固定循环数据

#define SYS_STATUS_IPO_CLOSE_MASK                (0x000000A0)    //系统有以下状态时，插补器需暂时关闭：SYS_STATUS_UPDATE_BOOTLOADER | SYS_STATUS_UPDATE_SYS_CTRL_PARM

//--------------------------------------------定义系统外部报警--------------------------------------------
#define SYS_ALARM_MAIN_POWER_OFF                (0x00000001)    //主电源掉电
#define SYS_ALARM_AHB_INIT_ERROR                (0x00000002)    //AHB初始化失败
#define SYS_ALARM_SDRAM_INIT_ERROR                (0x00000004)    //SDRAM初始化失败(SDRAM读写失败)
#define SYS_ALARM_DEV_DRV_INIT_ERROR            (0x00000008)    //设备驱动初始化失败
#define SYS_ALARM_IPO_INIT_ERROR                (0x00000010)    //插补器初始化失败
#define SYS_ALARM_CSM_INIT_ERROR                (0x00000020)    //通道管理模块初始化失败
#define SYS_ALARM_MST_INIT_ERROR                (0x00000040)    //MST模块初始化失败
#define SYS_ALARM_CMU_INIT_ERROR                (0x00000080)    //通信模块初始化失败
#define SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL        (0x00000100)    //更新系统控制参数失败
#define SYS_ALARM_UPDATE_AXIS_PARM_FAIL            (0x00000200)    //更新电机参数失败
#define SYS_ALARM_LOAD_SYS_PARM_FAIL            (0x00000400)    //加载系统控制参数失败
#define SYS_ALARM_LOAD_SYS_CONFIG_FAIL            (0x00000800)    //加载系统配置失败
#define SYS_ALARM_IO_OVER_CURRENT                (0x00001000)    //IO过流
#define SYS_ALARM_DEV_OFF_LINE                    (0x00002000)    //非轴设备掉线
#define SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL        (0x00004000)    //非轴设备发送周期性数据失败


//系统控制参数数据结构体
typedef struct _SysStateData {
    uBit32      ulSysCtrlStatus;        //系统控制字    
    uBit32        ulSysRunningStatus;        //系统状态字
    uBit32      ulSysExtAlarm;            //系统外部报警
    uBit32      ulSysRunId;                //运行ID
}SYS_STATE_DATA;




#pragma pack(pop)


#endif //#ifndef _SYS_DATA_STRUCT_DEF_H_

