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
#include "BS09_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //KEY
    {GPIO_PORT_B, 14},
    {GPIO_PORT_B, 13},
    {GPIO_PORT_B, 12},
    {GPIO_PORT_A,  9},
    {GPIO_PORT_A,  8},
    {GPIO_PORT_B, 15},
    {GPIO_PORT_B,  1},
    {GPIO_PORT_B, 10},
    {GPIO_PORT_B, 11},
    {GPIO_PORT_B,  0},
    {GPIO_PORT_A,  6},
    {GPIO_PORT_A,  7},

};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_C,  13},
    
    //LCD IO
    {GPIO_PORT_B,  6},  //D0,
    {GPIO_PORT_B,  5},  //D1,
    {GPIO_PORT_B,  4},  //D2,
    {GPIO_PORT_B,  3},  //D3,
    {GPIO_PORT_A, 15},  //D4,
    {GPIO_PORT_A, 12},  //D5,
    {GPIO_PORT_A, 11},  //D6,
    {GPIO_PORT_A, 10},  //D7,
                 
    {GPIO_PORT_B,  7},  //EN
    {GPIO_PORT_B,  8},  //RW
    {GPIO_PORT_B,  9},  //RS
    
    //AT24C02
    {GPIO_PORT_C, 15},  //SDA
    {GPIO_PORT_C, 14},  //SCL
    

};


//控制IO表
GPIO_CTRL_TABLE g_BS09_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
