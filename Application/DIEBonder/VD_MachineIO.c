/*
Copyright (c) 2017, 弘驰智能科技技术有限公司 All rights reserved.

文件名称：VD_MachineIO.c 

摘    要：水果自动售货机IO处理模块接口声明

说明：硬件版本4.0

运行环境：Windows Xp/WINCE

修改描述：

当前版本：4.0

修改作者：张腾

修改日期：2017-12-1
*/

#include "VD_MachineIO.h"

//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    {2,   6},
    {2,   6},
    {2,   5},
    {2,   4},
    {2,   3},
    {2,   2},
    {2,   1},
    {2,   0},
    {1,   8},
    {1,   9},
    {1,   10},
    {1,   14},
    {1,   15},
    {1,   16},
    {1,   17},
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    
    {0,  15},//LED灯
    {2,   7},
    {2,   8},
    {2,   9},
    {0,   16},
    {0,   17},
    {0,   18},
    {0,   19},
    {0,   20},
    {0,   21},
    {0,   22},
    {2,   10},
    {2,   11},
    {2,   12},
    {0,   9},
    {0,   8},
    {0,   7},
    {0,   6},
    {0,   5},
    {0,   4},
    {4,   28},
};


//控制IO表
GPIO_CTRL_TABLE g_VD1768_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};

