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
#include "BMC030_IODef.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {GPIO_PORT_A,  0},
    //{GPIO_PORT_A,  1},

};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_A,  0},
    
    {GPIO_PORT_A,  1},
    {GPIO_PORT_A,  2},
    {GPIO_PORT_A,  3},
    {GPIO_PORT_A,  4},
    
    
 
};


//控制IO表
GPIO_CTRL_TABLE g_BMC030_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    0,
    
};
