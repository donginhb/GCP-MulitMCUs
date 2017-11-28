/*    
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：DEV_COM_CmdDef.h

摘    要：本文定义所有设备通信模块指令类型

运行环境：IAR6.5 

修改描述：

当前版本：1.0

修改作者：

修改了内容：

修改日期：

原 作 者：Alan

完成日期：2016-3-29        
*/

#ifndef DEV_COM_CMD_DEF_H_
#define DEV_COM_CMD_DEF_H_

//-----------------------------------设备指令列表定义------------------------------- 
#define DEV_CMDTYPE_COUNT                (26)//设置指令种类数量
#define  DEV_CMD_INQ_INFO                (0)    //查询设备信息
#define  DEV_CMD_SET_NO                    (1)    //设置设备编号
#define  DEV_CMD_BLINK_LED                (2)    //设置LED闪烁（设备测试）
#define  DEV_CMD_INQ_VER                (3)    //查询版本信息
#define  DEV_CMD_CLR_ALM                (4)    //报警清除
#define  DEV_CMD_ENABLE_SV                (5)    //使能电机
#define  DEV_CMD_SET_HOME                (6)    //回零
#define  DEV_CMD_SET_LOCK                (7)    //轴锁定(表示轴正在进行插补运动) （待完善）
#define  DEV_CMD_RESET                    (8)    //系统复位
#define  DEV_CMD_SET_CMD_POS            (9)    //设置指令位置
#define  DEV_CMD_SET_QEI_POS            (10)//设置编码器位置
#define  DEV_CMD_SET_VERIFY_MODE        (11)//设置进入校验模式
#define  DEV_CMD_SET_IO_PWM_MODE        (12)//设置IO进入PWM模式
#define  DEV_CMD_SET_IO_PWM_DUTY        (13)//设置IO进入PWM占空比
#define  DEV_CMD_SET_IO_PWM_FREQ        (14)//设置IO进入PWM频率
#define  DEV_CMD_SET_FEED                (15)//设置进给
#define  DEV_CMD_SET_OUTPUT                (16)//设置IO输出
#define  DEV_CMD_SET_FSTP                (17)//CCW驱动禁止                              
#define  DEV_CMD_SET_RSTP                (18)//CW驱动禁止                                
#define  DEV_CMD_SET_CLESC1ZERO            (19)//偏差计算清零/速度选择1/零速箝位      
#define  DEV_CMD_SET_INH_SC2            (20)//指令脉冲禁止/速度选择2              
#define  DEV_CMD_SET_FIL                (21)//CCW转矩限制                            
#define  DEV_CMD_SET_RIL                (22)//CW转矩限制 
#define  DEV_CMD_INQ_SV_ALM                (23)//查询伺服报警
#define  DEV_CMD_SET_IPO_CYCLE            (24)//设置插补周期
#define    DEV_CMD_GET_SV_PARM                (25)//获取伺服参数


//-----------------------------------设备参数类型列表定义-------------------------------
#define DEV_PARMTYPE_COUNT                (5)//参数类型总数
#define DEV_PARM_TYPE_UPDATE_APP        (0)//更新应用程序数据
#define DEV_PARM_TYPE_UPDATE_BOOT        (1)//更新BOOTLOADER数据
#define DEV_PARM_TYPE_SV                (2)//伺服参数
#define DEV_PARM_TYPE_AXIS_PARM            (3)//设置轴参数
#define DEV_PARM_TYPE_SYS_CTRL_PARM        (4)//设置系统控制参数
#endif