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
#include "Application/GCPTest/GCP_MainProc.h"
#elif defined(LPC43XX)
#include "Application/CNMachine/CN_MainProc.h"
#elif defined(WFC_APP)
#include "Application/WIFIClock/WFC_MainProc.h"
#elif defined(MD_APP)
#include "Application/MotorDriver/MD_MainProc.h"
#elif defined(BMC_APP)
#include "Application/BLEMotorCtrl/BMC_MainProc.h"
#endif


int main(void)
{
    SYS_Init(); //系统初始化
    
#if !defined(BOOTLOADER)
#if defined(LPC17XX)
    GCP_Init();
#elif defined(LPC43XX)
    CN_Init();
#elif defined(WFC_APP)
    WFC_Init();
#elif defined(MD_APP)
    MD_Init();
#elif defined(BMC_APP)
    BMC_Init();
#endif
#endif 
    
    while(1)
    {
        //系统内部管理
        SYS_MainTaskHandler();
        
        //业务逻辑管理
#if !defined(BOOTLOADER)
#if defined(LPC17XX)
        GCP_MainProc();
#elif defined(LPC43XX)
        CN_MainProc();
#elif defined(WFC_APP)
        WFC_MainProc();
#elif defined(MD_APP)
        MD_MainProc();
#elif defined(BMC_APP)
        BMC_MainProc();
#endif
#endif
        
    }
    
} 

