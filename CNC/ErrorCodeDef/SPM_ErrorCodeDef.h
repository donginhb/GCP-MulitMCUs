/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SPM_ErrorCodeDef.h

摘    要：定义系统参数管理模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年9月17日
*/

#ifndef SPM_ERROR_CODE_DEF_H
#define SPM_ERROR_CODE_DEF_H



//定义指令类型
#define SPM_CMD_SET_SYS_CTRL_PARM                            (1)                //设置系统控制参数 SPM_SetSysCtrlParm
#define SPM_CMD_SET_AXIS_PARM                                (2)                //设置轴参数 SPM_SetAxisParm
#define SPM_CMD_SET_AXIS_PTICH_CMP_PARM                        (3)                //设置轴螺距补偿参数 SPM_SetAxisPitchCmpParm
#define SPM_CMD_SET_CRDSYS_AXIS_MAP_TABLE                    (4)                //设置坐标系坐标轴映射表 SPM_SetCrdSysAxisMapTable
#define SPM_CMD_SAVE_TO_CONTROLLER                            (5)                //控制器保存数据
#define SPM_CMD_SET_CONFIG                                    (6)                //设置硬件配置信息


//定义错误编号
#define SPM_ERR_CRDSYS_NOT_FREE                                (1)                //通道不空闲
#define SPM_ERR_IPO_NOT_FREE                                (2)                //插补器不空闲
#define SPM_ERR_PARM_ERROR                                    (3)                //参数错误
#define SPM_ERR_LESS_FLASH_FOR_SAVE                            (4)                //FLASH存储空间不能满足需要
#define SPM_ERR_FLASH_WRITE_FAIL                            (5)                //写FLASH失败



/*
#define CSM_ERR_CRDSYS_MODE_ERROR                            (2)                //系统控制模式不正确
#define CSM_ERR_CRDSYS_MODE_ERROR                            (3)                //系统控制模式不正确
#define CSM_ERR_PROG_BUF_FULL                                (4)                //程序缓冲区已满
#define CSM_ERR_EXE_STOP                                    (5)                //正在执行程序停止
#define CSM_ERR_EXE_RESET                                    (6)                //正在执行通道复位
#define CSM_ERR_PROG_RUNNING                                (7)                //程序正在运行
*/

















#endif

