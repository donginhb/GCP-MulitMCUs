/*
Copyright (c) 2017, 弘驰智能科技技术有限公司 All rights reserved.

文件名称：VD_MachineIO.h 

摘    要：水果自动售货机IO处理模块接口声明

说明：硬件版本4.0

运行环境：Windows Xp/WINCE

修改描述：

当前版本：4.0

修改作者：张腾

修改日期：2017-12-1
*/

#ifndef  __VD_MACHINE_IO_H
#define  __VD_MACHINE_IO_H
#ifdef __cplusplus
extern "C" {
#endif
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



  
//输入IO编号
typedef enum
{
    INPUT_IO_KEY0 = 0,
    INPUT_IO_1,
    INPUT_IO_2,
    INPUT_IO_3,
    INPUT_IO_4,
    INPUT_IO_5,
    INPUT_IO_6,
    INPUT_IO_7,
    INPUT_IO_8,
    INPUT_IO_9,
    INPUT_IO_10,
    INPUT_IO_11,
    INPUT_IO_12,
    INPUT_IO_13,
    INPUT_IO_14,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED
    OUTPUT_IO_1,
    OUTPUT_IO_2,
    OUTPUT_IO_3,
    OUTPUT_IO_4,
    OUTPUT_IO_5,
    OUTPUT_IO_6,
    OUTPUT_IO_7,
    OUTPUT_IO_8,
    OUTPUT_IO_9,
    OUTPUT_IO_10,
    OUTPUT_IO_11,
    OUTPUT_IO_12,
    OUTPUT_IO_13,
    OUTPUT_IO_14,
    OUTPUT_IO_15,
    OUTPUT_IO_16,
    OUTPUT_IO_17,
    OUTPUT_IO_18,
    OUTPUT_IO_19,
    OUTPUT_IO_20,

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_VD1768_IOTable;
    
    


#ifdef __cplusplus
}
#endif


#endif
