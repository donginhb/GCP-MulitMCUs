/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：IPO_ErrorCodeDef.h

摘    要：定义IPO错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年9月17日
*/

#ifndef IPO_ERROR_CODE_DEF_H
#define IPO_ERROR_CODE_DEF_H


#pragma pack(push)
#pragma pack(2)    


//定义指令类型
//#define IPO_CMD_UPDATE_CRDSYS_AXIS_MAP_TABLE                (1)                //更新坐标系坐标轴映射表
#define IPO_CMD_RESET                                        (1)                //插补器复位
#define IPO_CMD_ADD_MOTION_DATA                                (2)                //添加运动段数据到插补缓冲区IPO_AddMotionDataToIPOBuf
#define IPO_CMD_CREATE_RAPID_SPEED_CTRL_TABLE                (3)                //创建快移速度规划表
#define IPO_CMD_SET_CTRL_MODE                                (4)                //设置控制模式 IPO_SetCtrlMode
#define IPO_CMD_SET_VERIFY                                    (5)                //设置校验模式 IPO_SetVerifyEnable
#define IPO_CMD_PROG_RUN                                    (6)                //设置程序启动 IPO_ProgRun
#define IPO_CMD_PROG_STOP                                    (7)                //设置程序停止 IPO_ProgStop
#define IPO_CMD_PROG_ESTOP                                    (8)                //设置程序停止 IPO_ProgEStop
#define IPO_CMD_PROG_SEGMENT_END_STOP                        (9)                //设置程序停止 IPO_ProgSegmentEndStop
#define IPO_CMD_PROG_HOLD                                    (10)            //设置程序停止 IPO_ProgHold
#define IPO_CMD_SET_FEED_OVERRIDE                            (11)            //设置坐标系进给修调 IPO_SetCrdSysFeedOverride
#define IPO_CMD_SET_RAPID_OVERRIDE                            (12)            //设置坐标系快移修调 IPO_SetCrdSysRapidOverride
#define IPO_CMD_SET_SINGLE_SEG_MODE                            (13)            //设置单段模式 IPO_EnableSingleSegmentMode
#define IPO_CMD_GET_VERSION                                    (14)            //获取插补器版本信息 
#define IPO_CMD_UPDATE_SYS_CTRL_PARM                        (15)            //更新系统控制参数






//定义错误编号
#define IPO_ERR_INIT_ERROR                                    (1)                //插补器未正确初始化
#define IPO_ERR_CRDSYS_NOT_FREE                                (2)                //通道不空闲
#define IPO_ERR_CRDSYS_MODE_ERROR                            (3)                //系统控制模式不正确
#define IPO_ERR_BUF_FULL                                    (4)                //插补缓冲区已满
#define IPO_ERR_PROG_INVALID_CMD                            (5)                //非法的程序指令
#define IPO_ERR_CREATE_RAPID_SPEED_CTRL_TABLE_FAIL            (6)                //创建快移速度规划表失败
#define IPO_ERR_AXIS_DISABLE                                (7)                //轴未使能
#define IPO_ERR_EXE_STOP                                    (8)                //正在执行程序停止
#define IPO_ERR_INVALID_DATA                                (9)                //数据错误
#define IPO_ERR_NO_ANS                                        (10)            //插补器无应答




















#pragma pack(pop)


#endif

