/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_ErrorCodeDef.h

摘    要：定义系统参数管理模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年10月17日
*/

#ifndef SYS_ERROR_CODE_DEF_H
#define SYS_ERROR_CODE_DEF_H



//定义指令类型
#define SYS_CMD_RESET                                        (1)                //系统复位 SYS_Reset
#define SYS_CMD_LOAD_CC_DATA                                (2)                //装载固定循环数据


//定义错误编号
#define SYS_ERR_CRDSYS_NOT_FREE                                (1)                //通道不空闲
#define SYS_ERR_IPO_NOT_FREE                                (2)                //插补器不空闲
#define SYS_ERR_DATA_LEN                                    (3)                //数据长度错误
#define SYS_ERR_BLOCK_DISMATCH                                (4)                //数据块不连续
#define SYS_ERR_BUF_LIMIT                                    (5)                //存储空间不足
#define SYS_ERR_FLASH_W                                        (6)                //写flash失败


#endif

