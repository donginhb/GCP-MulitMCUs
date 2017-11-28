/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：UIN_ErrorCodeDef.h

摘    要：定义上位机通信模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：Alan

修改日期：2015年9月17日
*/

#ifndef UIN_ERROR_CODE_DEF_H
#define UIN_ERROR_CODE_DEF_H


#pragma pack(push)
#pragma pack(2)    

//上位机错误指令类型定义
//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_OpenCom                            (1)//打开通讯模块，iComType—通信方式0-无1-CAN 2-ENET
#define UIN_CMDTYPE_CloseCom                        (2)//关闭通讯模块
#define UIN_CMDTYPE_SetComType                        (3)//设置通信方式
#define UIN_CMDTYPE_BlinkDevLed                     (4)//测试板卡
#define UIN_CMDTYPE_SetDeviceConfig                 (5)//设置系统硬件配置信息
#define UIN_CMDTYPE_SetSysCtrlParm                    (6)//设置系统控制参数
#define UIN_CMDTYPE_LoadUpdataFile                    (7)//下载升级文件
#define UIN_CMDTYPE_SetIprEnable                    (8)//使能解释器
#define UIN_CMDTYPE_DevScan                            (9)//扫描系统设备
#define UIN_CMDTYPE_SetSysStatePeriodInq            (10)//设置系统状态周期发送
#define UIN_CMDTYPE_SaveParm                        (11)//控制器保存控制参数
#define UIN_CMDTYPE_SaveConfig                        (12)//控制器保存配置信息
#define UIN_CMDTYPE_SysReset                        (13)//系统复位

#define UIN_CMDTYPE_GetSLCVersion                    (20)//获取SLC软件版本
#define UIN_CMDTYPE_GetIPOVersion                    (21)//获取IPO软件版本
#define UIN_CMDTYPE_GetDeviceVersion                (22)//获取设备软件版本
#define UIN_CMDTYPE_GetBusDriverVersion                (23)//获取总线驱动版本
#define UIN_CMDTYPE_GetDeivceCount                     (24)//获取设备总数量
#define UIN_CMDTYPE_GetDeviceInfo                     (25)//获取设备信息
#define UIN_CMDTYPE_GetSysCtrlParm                    (26)//获取系统控制数据
#define UIN_CMDTYPE_GetSysStateData                    (27)//获取系统状态数据
#define UIN_CMDTYPE_GetSysCtrlStatus                (28)//获取系统状态数据
#define UIN_CMDTYPE_GetSysRunStatus                 (29)//获取系统状态字
#define UIN_CMDTYPE_GetSysAlarm                        (30)//获取系统外部报警
#define UIN_CMDTYPE_GetCtrlRsMsData                    (31)//获取CMU通信监测数据
#define UIN_CMDTYPE_GetCSMVersion                    (32)//获取CSM软件版本
#define UIN_CMDTYPE_GetHSpdQeiPos                    (33)//获取高速编码器输入
#define UIN_CMDTYPE_GetHSpdQeiSpeed                    (34)//获取高速编码器输入

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_SetCrdAxisMap                    (40)//设置轴映射表
#define UIN_CMDTYPE_SetCrdCtrlParm                    (41)//设置坐标系控制参数
#define UIN_CMDTYPE_SetCrdRapidOverride                (42)//设置坐标系快移修调
#define UIN_CMDTYPE_SetCrdFeedOverride                (43)//设置坐标系进给修调
#define UIN_CMDTYPE_SetCrdSpindleOverride            (44)//设置坐标系主轴修调
#define UIN_CMDTYPE_SetCrdCtrlMode                    (45)//设置坐标系工作模式
#define UIN_CMDTYPE_CrdSpindleRun                    (46)//启动主轴
#define UIN_CMDTYPE_CrdSpindleStop                    (47)//停止主轴
#define UIN_CMDTYPE_SetCrdVerifyEnable                (48)//校验运行使能控制
#define UIN_CMDTYPE_CrdEStop                        (49)//设置指定的坐标系急停
#define UIN_CMDTYPE_CrdReset                        (50)//坐标系复位
#define UIN_CMDTYPE_SetProgBufMode                    (51)//设置程序缓冲区工作模式
//#define UIN_CMDTYPE_OpenProgBuf                    (52)//打开或者关闭坐标系程序缓冲区    
//#define UIN_CMDTYPE_CloseProgBuf                    (53)//打开或者关闭坐标系程序缓冲区    
#define UIN_CMDTYPE_ClearProgBuf                    (54)//清除坐标系程序
#define UIN_CMDTYPE_EnableSingleSegMode                (55)//设置系统单段方式
#define UIN_CMDTYPE_EnaleLookahead                    (56)//前瞻控制控制
#define UIN_CMDTYPE_ProgRun                            (57)//运行坐标系程序
#define UIN_CMDTYPE_ProgHold                        (58)//暂停坐标系程序
#define UIN_CMDTYPE_ProgStop                        (59)//停止坐标系程序
#define UIN_CMDTYPE_ProgCancel                        (60)//取消程序
#define UIN_CMDTYPE_ProgSegEndStop                    (61)//段结束停止坐标系程序
#define UIN_CMDTYPE_SetUserDataBufMode                (62)//设置用户缓冲区工作模式
//#define UIN_CMDTYPE_OpenUserDataBuf                (63)//打开用户坐标系程序缓冲区    
//#define UIN_CMDTYPE_CloseUserDataBuf                (64)//打开用户坐标系程序缓冲区
#define UIN_CMDTYPE_ClearUserDataBuf                (65)//清除用户自定义数据区
#define UIN_CMDTYPE_LoadUserData                    (66)//下载用户数据
#define UIN_CMDTYPE_SetCrdStatePeriodInq            (67)//设置坐标系周期发送
#define UIN_CMDTYPE_LoadCannedData                    (68)//下载固定循环数据
#define UIN_CMDTYPE_LoadGCode                        (69)//下载G代码数据
#define UIN_CMDTYPE_LoadBpData                        (70)//下载断点数据
#define UIN_CMDTYPE_SetToolData                        (71)//设置刀库数据
#define UIN_CMDTYPE_SetCurTool                        (72)//设置当前刀具
#define UIN_CMDTYPE_SetBaseTool                        (73)//设置基准刀具
#define UIN_CMDTYPE_RndStart                        (74)//任意行启动
#define UIN_CMDTYPE_SetCoord                        (75)//设置当前坐标系零点位置
//#define UIN_CMDTYPE_SetMCoord                        (76)//设置工件坐标系
#define UIN_CMDTYPE_Dwell                            (78)//设置坐标系程序延时
#define UIN_CMDTYPE_MCode                            (79)//下载M代码运动段
#define UIN_CMDTYPE_SCode                            (80)//下载S代码运动段
#define UIN_CMDTYPE_TCode                            (81)//下载T代码运动段
#define UIN_CMDTYPE_BCode                            (82)//下载B代码运动段
#define UIN_CMDTYPE_IOCode                            (83)//下载IO代码运动段
#define UIN_CMDTYPE_Rapid                            (84)//单个轴或者多个轴同时执行快速移动
#define UIN_CMDTYPE_Line                            (85)//直线插补
#define UIN_CMDTYPE_ArcC                            (86)//圆弧插补半径
#define UIN_CMDTYPE_ArcR                            (87)//圆弧插补半径
#define UIN_CMDTYPE_ArcPT                            (88)//圆弧插补半径


#define UIN_CMDTYPE_GetCrdAxisMap                    (100)//获取坐标系映射表
#define UIN_CMDTYPE_GetCrdCtrlParm                    (101)//获取坐标系控制数据（添加获取坐标系控制参数单项数据接口）
#define UIN_CMDTYPE_GetToolData                        (102)//获取刀具数据
#define UIN_CMDTYPE_GetGCodeBufSize                    (103)//获取程序缓冲区大小
//#define UIN_CMDTYPE_GetGCodeBufFreeSize            (104)
#define UIN_CMDTYPE_UploadProgData                    (105)//上传程序缓冲区数据
#define UIN_CMDTYPE_UploadUserData                    (106)//上传用户缓冲区数据
#define UIN_CMDTYPE_UploadGCodeData                    (107)//上传G代码数据
#define UIN_CMDTYPE_GetBpDataLen                    (108)//获取断点数据总长度
#define UIN_CMDTYPE_GetBpData                        (109)//获取断点数据
#define UIN_CMDTYPE_GetIprCurLineNo                    (110)//获取解释器当前解释行
#define UIN_CMDTYPE_GetCrdZero                        (111)//获取坐标系零点
//#define UIN_CMDTYPE_IPR_get_var                    (112)//获取变量
#define UIN_CMDTYPE_GetCrdStateData                    (113)//获取坐标系状态数据
#define UIN_CMDTYPE_GetCrdActSpeed                    (114)//获取坐标系当前实际速度（合成速度）
#define UIN_CMDTYPE_GetCrdCmdSpeed                    (115)//获取坐标系设定速度（合成速度）
#define UIN_CMDTYPE_GetCrdAcc                        (116)//获取坐标系当前加速度
#define UIN_CMDTYPE_GetCrdRapidOverride                 (117)//获取坐标系快移修调
#define UIN_CMDTYPE_GetCrdFeedOverride                (118)//获取坐标系进给修调
#define UIN_CMDTYPE_GetCrdSpindleSpeed                 (119)//获取坐标系主轴速度
#define UIN_CMDTYPE_GetCrdSpindleOverride            (120)//获取坐标系主轴修调
#define UIN_CMDTYPE_GetCrdRunStatus                    (121)//获取坐标系运行状态
#define UIN_CMDTYPE_GetCrdAlarm                        (122)//获取坐标系报警状态
#define UIN_CMDTYPE_GetCrdMSTStatus                    (123)//获取坐标系MST运行状态字
#define UIN_CMDTYPE_GetCrdMCode                        (124)//获取坐标系当前运行的M码
#define UIN_CMDTYPE_GetCrdSCode                        (125)//获取坐标系当前运行的S码
#define UIN_CMDTYPE_GetCrdTCode                        (126)//获取坐标系当前运行的T码
#define UIN_CMDTYPE_GetCrdBCode                        (127)//获取坐标系当前运行的B码
#define UIN_CMDTYPE_GetCurProgLineNo                (128)//获取当前程序行
#define UIN_CMDTYPE_GetCurProgLineUserNo            (129)//获取坐标系程序运行行查询
#define UIN_CMDTYPE_GetProgBufState                    (130)//获取程序缓冲区全部状态
#define UIN_CMDTYPE_GetProgBufSize                    (131)//获取程序缓冲区长度
#define UIN_CMDTYPE_GetProgBufStatus                (132)//获取程序缓冲区状态
#define UIN_CMDTYPE_GetUserBufState                    (133)//获取用户缓冲区全部状态
#define UIN_CMDTYPE_GetUserBufSize                    (134)//获取用户程序缓冲区长度
#define UIN_CMDTYPE_GetUserBufStatus                (135)//获取用户程序缓冲区状态
#define UIN_CMDTYPE_GetCrdCtrlStatus                (136)//获取坐标系控制状态
//////////////////////////////////////////////////////////////////////////////////////////////////
                                                                         
//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_SetMotorCtrlParm                 (150)//设置电机控制参数
#define UIN_CMDTYPE_SetMotorPitchCmpParm            (151)//设置电机螺距补偿
#define UIN_CMDTYPE_MotorPosCtrlMotion                 (152)//设置位置控制运动数据
#define UIN_CMDTYPE_MotorSpeedCtrlMotion            (153)//设置速度控制运动数据
#define UIN_CMDTYPE_MotorHome                        (154)//电机回零
#define UIN_CMDTYPE_MotorStop                        (155)//电机停止
#define UIN_CMDTYPE_MotorEStop                        (156)//电机急停
#define UIN_CMDTYPE_SetMotorCmdPos                    (157)//设置电机当前置单位：um
#define UIN_CMDTYPE_SetMotorQeiPos                    (158)//设置电机当前位置单位：um
#define UIN_CMDTYPE_SetMotorEnable                    (159)//设置电机使能-断使能-加使能
#define UIN_CMDTYPE_SetMotorStatePeriodInq            (160)//设置电机状态周期发送
#define UIN_CMDTYPE_SetMotorReset                    (161)//电机复位
#define UIN_CMDTYPE_SetSvParm                        (162)//设置伺服参数

#define UIN_CMDTYPE_GetMotorCtrlParm                (170)//获取电机控制参数
#define UIN_CMDTYPE_GetMotorSignalParm                (171)//获取电机信号量参数
#define UIN_CMDTYPE_GetMotorSafeParm                (172)//获取电机安全参数
#define UIN_CMDTYPE_GetMotorHomeParm                (173)//获取电机回零参数
#define UIN_CMDTYPE_GetMotorScaleParm                (174)//获取电机比例参数
#define UIN_CMDTYPE_GetBackLashCmpParm                (175)//获取电机反向间隙参数
#define UIN_CMDTYPE_GetMotorMoveParm                (176)//获取电机运动参数
#define UIN_CMDTYPE_GetMotorPitchCmpParm            (177)//获取電機螺距补偿表
#define UIN_CMDTYPE_GetMotorStateData                 (178)//获取电机态数据
#define UIN_CMDTYPE_GetMotorCtrlStatus                (179)//获取电机控制状态字
#define UIN_CMDTYPE_GetMotorRunStatus                 (180)//获取电机运行状态字
#define UIN_CMDTYPE_GetMotorQeiPos                    (181)//获取电机编码器位置
#define UIN_CMDTYPE_GetMotorQeiSpeed                 (182)//获取电机实际速度
#define UIN_CMDTYPE_GetMotorCmdPos                     (183)//获取电机指令位置
#define UIN_CMDTYPE_GetMotorCmdSpeed                (184)//获取电机指令速度
#define UIN_CMDTYPE_GetMotorAcc                         (185)//获取电机加速度
#define UIN_CMDTYPE_GetSvParm                        (186)//获取伺服参数
#define UIN_CMDTYPE_GetSvAlarm                        (187)//获取伺服报警
#define UIN_CMDTYPE_GetSvStatus                        (188)//获取伺服状态

#define UIN_CMDTYPE_AxisPosCtrlMotion                (190)//设置位置控制运动数据
#define UIN_CMDTYPE_AxisSpeedCtrlMotion                (191)//设置速度控制运动数据
#define UIN_CMDTYPE_AxisHome                         (192)//轴回零
#define UIN_CMDTYPE_AxisStop                         (193)//轴停止
#define UIN_CMDTYPE_AxisEStop                         (194)//轴急停
#define UIN_CMDTYPE_SetAxisCmdPos                     (195)//设置轴当前位置单位：um
#define UIN_CMDTYPE_SetAxisQeiPos                    (196)//设置轴当前位置单位：um
#define UIN_CMDTYPE_SetAxisEnable                    (197)//设置轴使能-断使能-加使能
#define UIN_CMDTYPE_SetAxisStatePeriodInq            (198)//设置轴状态周期发送
#define UIN_CMDTYPE_GetAxisStateData                (199)//获取轴状态数据
#define UIN_CMDTYPE_GetAxisCtrlStatus                (200)//获取轴控制状态字
#define UIN_CMDTYPE_GetAxisRunStatus                (201)//获取轴运行状态字
#define UIN_CMDTYPE_GetAxisQeiPos                    (202)//获取轴编码器位置
#define UIN_CMDTYPE_GetAxisQeiSpeed                    (203)//获取轴实际速度
#define UIN_CMDTYPE_GetAxisCmdPos                     (204)//获取轴指令位置
#define UIN_CMDTYPE_GetAxisCmdSpeed                    (205)//获取轴指令速度
#define UIN_CMDTYPE_GetAxisAcc                        (206)//获取轴加速度
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_SetIOState                         (210)//设置IO板状态
#define UIN_CMDTYPE_SetIOPwmEnable                    (211)//使能IOPWM功能    
#define UIN_CMDTYPE_SetIOPwmDutyRatio                (212)//设置占空比-100    
#define UIN_CMDTYPE_SetIOPwmFrq                     (213)//设置输出口PWM频率
#define UIN_CMDTYPE_SetIOPeriodInq                    (214)//设置IO状态周期发送
#define UIN_CMDTYPE_GetIOAllState                    (215)//获取IO板系统状态
#define UIN_CMDTYPE_GetIOSysStatus                     (216)//获取IO板系统状态
#define UIN_CMDTYPE_GetInputStatus                    (217)//获取IO输出状态
#define UIN_CMDTYPE_GetOutputStatus                    (218)//获取IO输入状态
#define UIN_CMDTYPE_GetOutputCmd                    (219)//获取IO输出指令状态
#define UIN_CMDTYPE_SetHSpdOutputState                (220)//设置高速IO输出状态
#define UIN_CMDTYPE_GetHSpdOutputState                (221)//获取高速IO输出状态
#define UIN_CMDTYPE_GetHSpdInputState                (222)//获取高速IO输入状态

//////////////////////////////////////////////////////////////////////////////////////////////////
                                                                         
//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_SetDAData                        (230)//设置DA板DA输出
#define UIN_CMDTYPE_SetADDAPeriodInq                (231)//设置AD状态周期发送
#define UIN_CMDTYPE_GetADDAAllState                    (232)//获取ADDA板的所有状态
#define UIN_CMDTYPE_GetADDASysStatus                (233)//获取ADDA板系统状态
#define UIN_CMDTYPE_GetADStatus                        (234)//获取ADDA板AD输入状态
#define UIN_CMDTYPE_GetDAStatus                        (235)//获取ADDA板DA输出状态
#define UIN_CMDTYPE_GetDACmdStatus                    (236)//获取ADDA板DA指令状态
#define UIN_CMDTYPE_SetHSpdDAData                        (237)//设置高速DA输出
#define UIN_CMDTYPE_GetHSpdDAData                        (238)//获取高速DA输出值
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
#define UIN_CMDTYPE_SetUserReg                        (240)//设置用户寄存器
#define UIN_CMDTYPE_GetUserReg                        (241)//设置用户寄存器
//////////////////////////////////////////////////////////////////////////////////////////////////


//--------------上位机通信模块错误编号定义-------------------
#define UIN_ERR_SUCCESS                    (0)//成功
#define UIN_ERR_CONNECT                    (1)//表示通信连接出错(接收和发送抽象层硬件出错)
#define UIN_ERR_SEND                    (2)//发送数据失败
#define UIN_ERR_DEV_NOT_EXIST            (3)//设备不存在，或未编号
#define UIN_ERR_CRD_NOT_EXIST            (4)//通道不存在
#define UIN_ERR_DATA_WRONG                (5)//数据错误,接口参数错误
#define UIN_ERR_SEND_OVERFLOW            (6)//发送缓冲区溢出
#define UIN_ERR_EMPTY                    (7)//未收到任何数据
#define UIN_ERR_RCV_CHECK                (8)//接收数据校验错误
#define UIN_ERR_TIME_OUT                (9)//接收回应包超时
#define UIN_ERR_DATA_LOST                (10)//接收数据存在数据丢失
#define UIN_ERR_RCV_OVERFLOW            (11)//接收缓冲区溢出
#define UIN_ERR_FILE_OPEN                (12)//表示文件打开错误
#define UIN_ERR_INVALID_UPDATE_FILE        (13)//非法升级文件
#define UIN_ERR_UPDATE_FILE_CHECK        (14)//升级文件校验错误
#define UIN_ERR_CRD_NOT_MAPPED            (15)//坐标系未配置
#define UIN_ERR_DEVNUM_NOT_MATCH        (16)//设备总数跟实际不符
#define UIN_ERR_DEVCONF_INVALID            (17)//配置信息不合法
#pragma pack(pop)
#endif

