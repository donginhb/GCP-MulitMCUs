/*
Copyright (c) 2017, ������ܿƼ��������޹�˾ All rights reserved.

�ļ����ƣ�VD_MachineIO.c 

ժ    Ҫ��ˮ���Զ��ۻ���IO����ģ��ӿ�����

˵����Ӳ���汾4.0

���л�����Windows Xp/WINCE

�޸�������

��ǰ�汾��4.0

�޸����ߣ�����

�޸����ڣ�2017-12-1
*/

#include "VD_MachineIO.h"

//����IO��
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


//���IO��
static GPIO_DATA m_OutputIOTable[] = 
{
    
    {0,  15},//LED��
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


//����IO��
GPIO_CTRL_TABLE g_VD1768_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};

