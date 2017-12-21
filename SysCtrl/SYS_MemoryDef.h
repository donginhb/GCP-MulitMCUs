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
#define AHB_SRAM0_ADDR                  (0x2007C000U)       //AHB SRAM0 地址
#define AHB_SRAM0_SIZE                  (16*1024)           //AHB SRAM0 大小

//LPC1759/8 LPC1769/8/7/6/5
#define AHB_SRAM1_ADDR                  (0x20080000U)       //AHB SRAM1 地址
#define AHB_SRAM1_SIZE                  (16*1024)           //AHB SRAM1 大小

//Bootloader(24K)
#define FLASH_BOOT_BANK                 (0)                 //BootloaderFLASH块
#define FLASH_BOOT_START_SECTOR         (0)                 //Bootloader开始扇区
#define FLASH_BOOT_END_SECTOR           (5)                 //Bootloader结束扇区
#define FLASH_BOOT_START_ADDR           (0x00000000)        //Bootloader开始地址
#define FLASH_BOOT_END_ADDR             (0x00005FFF)        //Bootloader结束地址

//UserData(4K)
#define FLASH_USER_BANK                 (1)                 //用户数据区FLASH块
#define FLASH_USER_START_SECTOR         (6)                 //用户数据区开始扇区
#define FLASH_USER_END_SECTOR           (6)                 //用户数据区结束扇区
#define FLASH_USER_START_ADDR           (0x00006000)        //用户数据区开始地址
#define FLASH_USER_END_ADDR             (0x00006FFF)        //用户数据区结束地址

//Application(484K)
#define FLASH_APP_BANK                  (2)                 //应用程序FLASH块
#define FLASH_APP_START_SECTOR          (7)                 //应用程序开始扇区
#define FLASH_APP_END_SECTOR            (29)                //应用程序结束扇区
#define FLASH_APP_START_ADDR            (0x00007000)        //应用程序开始地址
#define FLASH_APP_END_ADDR              (0x0007FFFF)        //应用程序结束地址

//Sub-Application(0)
#define FLASH_SUB_APP_BANK              (3)                 //副应用程序FLASH块
#define FLASH_SUB_APP_START_SECTOR      (7)                 //副应用程序开始扇区
#define FLASH_SUB_APP_END_SECTOR        (29)                //副应用程序结束扇区
#define FLASH_SUB_APP_START_ADDR        (0x00007000)        //副应用程序开始地址
#define FLASH_SUB_APP_END_ADDR          (0x0007FFFF)        //副应用程序结束地址

#elif defined(LPC43XX)

//LPC1759/8/6/4 LPC176X
#define AHB_SRAM0_ADDR                  (0x2007C000U)       //AHB SRAM0 地址
#define AHB_SRAM0_SIZE                  (16*1024)           //AHB SRAM0 大小

//LPC1759/8 LPC1769/8/7/6/5
#define AHB_SRAM1_ADDR                  (0x20080000U)       //AHB SRAM1 地址
#define AHB_SRAM1_SIZE                  (16*1024)           //AHB SRAM1 大小

//Bootloader(24K)
#define FLASH_BOOT_BANK                 (0)                 //BootloaderFLASH块
#define FLASH_BOOT_START_SECTOR         (0)                 //Bootloader开始扇区
#define FLASH_BOOT_END_SECTOR           (0)                 //Bootloader结束扇区
#define FLASH_BOOT_START_ADDR           (0x1A000000)        //Bootloader开始地址
#define FLASH_BOOT_END_ADDR             (0x1A00FFFF)        //Bootloader结束地址

//UserData(4K)
#define FLASH_USER_BANK                 (1)                 //用户数据区FLASH块
#define FLASH_USER_START_SECTOR         (0)                 //用户数据区开始扇区
#define FLASH_USER_END_SECTOR           (0)                 //用户数据区结束扇区
#define FLASH_USER_START_ADDR           (0x1B070000)        //用户数据区开始地址
#define FLASH_USER_END_ADDR             (0x1B07FFFF)        //用户数据区结束地址

//Application(484K)
#define FLASH_APP_BANK                  (2)                 //应用程序FLASH块
#define FLASH_APP_START_SECTOR          (0)                 //应用程序开始扇区
#define FLASH_APP_END_SECTOR            (0)                 //应用程序结束扇区
#define FLASH_APP_START_ADDR            (0x1A010000)        //应用程序开始地址
#define FLASH_APP_END_ADDR              (0x1A07FFFF)        //应用程序结束地址

//Sub-Application(0)
#define FLASH_SUB_APP_BANK              (3)                 //副应用程序FLASH块
#define FLASH_SUB_APP_START_SECTOR      (0)                 //副应用程序开始扇区
#define FLASH_SUB_APP_END_SECTOR        (0)                 //副应用程序结束扇区
#define FLASH_SUB_APP_START_ADDR        (0x1B000000)        //副应用程序开始地址
#define FLASH_SUB_APP_END_ADDR          (0x1B03FFFF)        //副应用程序结束地址


#elif defined(STM32F10X)

//LPC1759/8/6/4 LPC176X
#define AHB_SRAM0_ADDR                  (0x2007C000U)       //AHB SRAM0 地址
#define AHB_SRAM0_SIZE                  (16*1024)           //AHB SRAM0 大小

//LPC1759/8 LPC1769/8/7/6/5
#define AHB_SRAM1_ADDR                  (0x20080000U)       //AHB SRAM1 地址
#define AHB_SRAM1_SIZE                  (16*1024)           //AHB SRAM1 大小

//Bootloader(24K)
#define FLASH_BOOT_BANK                 (0)                 //BootloaderFLASH块
#define FLASH_BOOT_START_SECTOR         (0)                 //Bootloader开始扇区
#define FLASH_BOOT_END_SECTOR           (5)                 //Bootloader结束扇区
#define FLASH_BOOT_START_ADDR           (0x00000000)        //Bootloader开始地址
#define FLASH_BOOT_END_ADDR             (0x00005FFF)        //Bootloader结束地址

//UserData(4K)
#define FLASH_USER_BANK                 (1)                 //用户数据区FLASH块
#define FLASH_USER_START_SECTOR         (6)                 //用户数据区开始扇区
#define FLASH_USER_END_SECTOR           (6)                 //用户数据区结束扇区
#define FLASH_USER_START_ADDR           (0x00006000)        //用户数据区开始地址
#define FLASH_USER_END_ADDR             (0x00006FFF)        //用户数据区结束地址

//Application(484K)
#define FLASH_APP_BANK                  (2)                 //应用程序FLASH块
#define FLASH_APP_START_SECTOR          (7)                 //应用程序开始扇区
#define FLASH_APP_END_SECTOR            (29)                //应用程序结束扇区
#define FLASH_APP_START_ADDR            (0x00007000)        //应用程序开始地址
#define FLASH_APP_END_ADDR              (0x0007FFFF)        //应用程序结束地址

//Sub-Application(0)
#define FLASH_SUB_APP_BANK              (3)                 //副应用程序FLASH块
#define FLASH_SUB_APP_START_SECTOR      (0)                 //副应用程序开始扇区
#define FLASH_SUB_APP_END_SECTOR        (0)                 //副应用程序结束扇区
#define FLASH_SUB_APP_START_ADDR        (0x00000000)        //副应用程序开始地址
#define FLASH_SUB_APP_END_ADDR          (0x00000000)        //副应用程序结束地址


#endif


#endif /* __SYS_MEMORY_DEF_H */
