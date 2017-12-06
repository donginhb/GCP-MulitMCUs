/**
  ******************************************************************************
  * @file    main.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 通用控制平台程序入口
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysCtrl/SYS_Init.h"

#if defined(LPC17XX)
#include "Application/VDMachine/VM_MainProc.h"
#elif defined(LPC43XX)
#include "Application/CNMachine/CN_MainProc.h"
#elif defined(STM32F10X)
#include "Application/WIFIClock/WFC_MainProc.h"
#endif


int main(void)
{
    SYS_Init(); //系统初始化
    
#if defined(LPC17XX)
    VM_Init();
#elif defined(LPC43XX)
    CN_Init();
#elif defined(STM32F10X)
    WFC_Init();
#endif
    
    while(1)
    {
#if defined(LPC17XX)
        VM_MainProc();
#elif defined(LPC43XX)
        CN_MainProc();
#elif defined(STM32F10X)
        WFC_MainProc();
#endif
        SYS_MainTaskHandler();
    }
    
} 

