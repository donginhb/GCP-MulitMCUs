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
#include "MD103_IODef.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {GPIO_PORT_A,  0},
    {GPIO_PORT_A,  1},

};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_C, 13},
    
    {GPIO_PORT_A, 12},
    {GPIO_PORT_A, 15},
    {GPIO_PORT_B,  5},
    {GPIO_PORT_B,  6},
    
    
 
};


//控制IO表
GPIO_CTRL_TABLE g_MD103_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
