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
#include "DS_IODef.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键信号
    {GPIO_PORT_0,  4},
    {GPIO_PORT_0,  5},
    {GPIO_PORT_0, 23},
    {GPIO_PORT_2,  7},
    {GPIO_PORT_2, 12},


};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED指示灯
    {GPIO_PORT_3, 25},
    {GPIO_PORT_3, 26},
    {GPIO_PORT_2,  4},
    {GPIO_PORT_2,  5},
    
 
};


//控制IO表
GPIO_CTRL_TABLE g_DS1768_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
