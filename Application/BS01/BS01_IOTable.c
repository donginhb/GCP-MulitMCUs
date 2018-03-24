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
#include "BS01_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {GPIO_PORT_A,  0},
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_C, 13},
    
    {GPIO_PORT_A, 11},  //D0,
    {GPIO_PORT_A, 10},  //D1,
    {GPIO_PORT_A,  9},  //D2,
    {GPIO_PORT_A,  8},  //D3,
    {GPIO_PORT_B, 15},  //D4,
    {GPIO_PORT_B, 14},  //D5,
    {GPIO_PORT_B, 13},  //D6,
    {GPIO_PORT_B, 12},  //D7,
    
    {GPIO_PORT_A, 12},  //EN
    {GPIO_PORT_B,  5},  //RW
    {GPIO_PORT_B,  6},  //RS
    
};


//控制IO表
GPIO_CTRL_TABLE g_BS01_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    //sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    0
    
};
