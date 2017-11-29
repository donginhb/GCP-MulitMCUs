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
#include "Application/DIEBonder/DB_MainProc.h"
#elif defined(LPC43XX)
#include "Application/CNMachine/CN_MainProc.h"
#endif


int main(void)
{
    SYS_Init(); //系统初始化
    
#if defined(LPC17XX)
    DB_Init();
#elif defined(LPC43XX)
    CN_Init();
#endif
    
    while(1)
    {
#if defined(LPC17XX)
        DB_MainProc();
#elif defined(LPC43XX)
        CN_MainProc();
#endif
        SYS_MainTaskHandler();
    }
    
} 

