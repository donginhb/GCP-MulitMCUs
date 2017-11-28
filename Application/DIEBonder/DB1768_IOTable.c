/**
  ******************************************************************************
  * @file    GCIOCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
/***********************************<INCLUDES>**********************************/
#include "DB1768_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {0,   4},
    {0,   5},
    {0,  23},
    {2,   7},
    {2,  12},
    
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {3,  25},
    {3,  26},
    {2,   4},
    {2,   5},

};


//控制IO表
GPIO_CTRL_TABLE g_DB1768_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
