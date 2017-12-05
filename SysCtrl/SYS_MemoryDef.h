/**
  ******************************************************************************
  * @file    SysCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   System control interface
  ******************************************************************************
  * @attention
  * 
  * LPC17XX:
  *   若使用CNC, 则使用 AHB_SRAM0 作为CNC的缓冲区;
  *   若使用LWIP,则使用 AHB_SRAM1 作为LWIP的缓冲区;
  *   
  *   考虑到性能原因,原则上LPC1752/1不使用CNC;
  * 
  * LPC43XX:
  * 
  * 
  * 
  * 
  * 
  *  
  * 
  ******************************************************************************
  */


#ifndef  __SYS_MEMORY_DEF_H
#define  __SYS_MEMORY_DEF_H

#include "DataType/DataType.h"


#if defined(LPC17XX)

//LPC1759/8/6/4 LPC176X
#define AHB_SRAM0_ADDR          (0x2007C000)    //AHB SRAM0 地址
#define AHB_SRAM0_SIZE          (16*1024)       //AHB SRAM0 大小

//LPC1759/8 LPC1769/8/7/6/5
#define AHB_SRAM1_ADDR          (0x20080000)    //AHB SRAM1 地址
#define AHB_SRAM1_SIZE          (16*1024)       //AHB SRAM1 大小


#elif defined(LPC43XX)



#elif defined(STM32F10X)




#endif


#endif /* __SYS_MEMORY_DEF_H */
