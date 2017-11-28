/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_ErrorCodeDef.h

摘    要：定义通道管理模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年9月17日
*/

#ifndef CSM_ERROR_CODE_DEF_H
#define CSM_ERROR_CODE_DEF_H



//定义指令类型
#define CSM_CMD_RESET                                        (1)                //通道复位 CSM_Reset
#define CSM_CMD_SET_CTRL_MODE                                (2)                //设置控制模式 CSM_SetCtrlMode
#define CSM_CMD_SET_VERIFY                                    (3)                //设置校验模式 CSM_SetVerifyEnable
#define CSM_CMD_PROG_RUN                                    (4)                //设置程序启动 CSM_ProgRun
#define CSM_CMD_PROG_STOP                                    (5)                //设置程序停止 CSM_ProgStop
#define CSM_CMD_PROG_ESTOP                                    (6)                //设置程序停止 CSM_ProgEStop
#define CSM_CMD_PROG_SEGMENT_END_STOP                        (7)                //设置程序停止 CSM_ProgSegmentEndStop
#define CSM_CMD_PROG_HOLD                                    (8)                //设置程序停止 CSM_ProgHold
#define CSM_CMD_PROG_CANCEL                                    (9)                //设置程序停止 CSM_ProgCancel
#define CSM_CMD_SET_PROG_BUF_MODE                            (10)            //设置程序缓冲区工作模式 CSM_SetProgBufMode
#define CSM_CMD_ADD_PROG_DATA                                (11)            //添加程序段数据到程序缓冲区 CSM_AddProgDataToBuf
#define CSM_CMD_CLEAR_PROG_DATA                                (12)            //清除程序数据 CSM_ClearProgData
#define CSM_CMD_SET_USER_BUF_MODE                            (13)            //设置用户自定义数据缓冲区工作模式 CSM_SetUserBufMode
#define CSM_CMD_ADD_USER_DATA                                (14)            //添加用户自定义数据到缓冲区 CSM_AddUserDataToBuf
#define CSM_CMD_AXIS_POS_CTRL_MOTION                        (15)            //设置轴位置控制运动指令 CSM_SetAxisPosCtrlMotion
#define CSM_CMD_AXIS_SPEED_CTRL_MOTION                        (16)            //设置轴速度控制运动指令 CSM_SetAxisSpeedCtrlMotion
#define CSM_CMD_SET_AXIS_JOG_STOP                            (17)            //设置坐标轴手动运动停止 CSM_SetAxisJogStop
#define CSM_CMD_MOTOR_POS_CTRL_MOTION                        (18)            //设置电机位置控制运动指令 CSM_SetMotorPosCtrlMotion
#define CSM_CMD_MOTOR_SPEED_CTRL_MOTION                        (19)            //设置电机速度控制运动指令 CSM_SetMotorSpeedCtrlMotion
#define CSM_CMD_SET_MOTOR_JOG_STOP                            (20)            //设置电机手动运动停止 CSM_SetMotorJogStop
#define CSM_CMD_CRDSYS_LOCATION                                (21)            //坐标系重定位 CSM_MoveAxis
#define CSM_CMD_LAX_ENABLE                                    (22)            //设置轴使能 LAX_Enable
#define CSM_CMD_LAX_SET_CMD_POS                                (23)            //设置轴指令位置 LAX_SetCmdPos
#define CSM_CMD_LAX_SET_QEI_POS                                (24)            //设置轴编码器位置 LAX_SetQeiPos
#define CSM_CMD_SET_USER_BUF_READ_POS                        (25)            //设置用户缓冲区读位置 CSM_SetUserBufReadPos
#define CSM_CMD_PROG_ARC_CENTERPT                            (26)            //以指定圆心方式设置圆弧程序段 CSM_ProgArcC
#define CSM_CMD_SET_SINGLE_SEG_MODE                            (27)            //设置单段模式 CSM_EnableSingleSegmentMode
#define CSM_CMD_LAX_HOME                                    (28)            //设置轴回零 LAX_Home
#define CSM_CMD_LAX_CHECK_CMD_EXE_STATE                        (29)            //检查轴控制指令执行状态 LAX_CheckLastCmdExeState



//定义错误编号
#define CSM_ERR_CRDSYS_NOT_FREE                                (1)                //通道不空闲
#define CSM_ERR_CRDSYS_MODE_ERROR                            (2)                //系统控制模式不正确
#define CSM_ERR_PROG_BUF_FULL                                (3)                //程序缓冲区已满
#define CSM_ERR_EXE_STOP                                    (4)                //正在执行程序停止
#define CSM_ERR_EXE_RESET                                    (5)                //正在执行通道复位
#define CSM_ERR_PROG_RUNNING                                (6)                //程序正在运行
#define CSM_ERR_USER_BUF_DYNAMIC                            (7)                //用户缓冲区为动态模式（不能设置读位置）
#define CSM_ERR_SYS_ALARM_EXIST                                (8)                //存在系统报警
#define CSM_ERR_AXIS_ALARM_EXIST                            (9)                //存在轴报警
#define CSM_ERR_CRDSYS_ALARM_EXIST                            (10)            //存在通道报警
#define CSM_ERR_PROG_DATA_ERROR                                (11)            //程序段数据错误
#define CSM_ERR_EXE_ESTOP                                    (12)            //正在执行程序急停
#define CSM_ERR_MOTOR_EXE_HOME                                (13)            //电机正在执行回零
#define CSM_ERR_MOTOR_DISABLE                                (14)            //电机未使能
#define CSM_ERR_MOTOR_ALARM_EXIST                            (15)            //存在电机报警
#define CSM_ERR_AXIS_MAP_MOTOR_CMD_EXE_DISMATCH                (16)            //坐标轴映射的电机执行指令不一致




















#endif

