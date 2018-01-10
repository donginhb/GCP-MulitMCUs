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
#include "WFC103_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {4,   4},
    {4,   3},
    {4,   2},
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_B,   5},
    
    {GPIO_PORT_A, 12},
    {GPIO_PORT_A, 11},
    {GPIO_PORT_A, 10},
    {GPIO_PORT_A,  9},
    {GPIO_PORT_B, 15},
    {GPIO_PORT_B, 14},
    {GPIO_PORT_B, 13},
    {GPIO_PORT_B, 12},
    
    {GPIO_PORT_A, 15},  //E
    {GPIO_PORT_B,  3},  //RW
    {GPIO_PORT_B,  4},  //RS
};


//控制IO表
GPIO_CTRL_TABLE g_WFC103_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
