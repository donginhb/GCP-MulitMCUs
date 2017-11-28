/*
Copyright (c) 2016, 东莞华科精机有限公司 All rights reserved.

文件名称：FlashAddrDef.h

摘    要：4337 FLASH使用地址定义

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2016年1月27日
*/


#ifndef FLASH_ADDR_DEFINE_H
#define FLASH_ADDR_DEFINE_H



/*
#define BASE_FLASH_M4                   (0x1A000000)               // M4用户Flash基地址  
#define BASE_FLASH_M0                   (0x1B000000)               // M0用户Flash基地址  
#define M4APP_START_Flash               (0x1A010000)              // M4用户Flash起始地址  
#define M4APP_END_Flash                 (0x1A07FFFF)               // M4用户Flash结束地址 
#define M0APP_START_Flash               (0x1B000000)              // M0用户Flash起始地址  
#define M0APP_END_Flash                 (0x1B03FFFF)               // M0用户Flash结束地址 
#define BOOT_START_Flash                (0x1A000000)                 //BootLoder起始地址
#define BOOT_END_Flash                    (0x1A00FFFF)                     //BootLoder起始地址
*/

//-------------------------------------M4-Flash地址定义（0x1A000000----0x1A07FFFF）------------------------------
#define FLASH_M4_BASE_ADDR              (0x1A000000)            //M4-Flash基地址  

#define FLASH_M4_BOOT_START_ADDR        (0x1A000000)            //BootLoder起始地址
#define FLASH_M4_BOOT_END_ADDR            (0x1A00FFFF)            //BootLoder结束地址

#define FLASH_M4_APP_START_ADDR         (0x1A010000)            //M4应用程序起始地址  
#define FLASH_M4_APP_END_ADDR           (0x1A07FFFF)            //M4应用程序结束地址 


//-------------------------------------M0-Flash地址定义（0x1B000000----0x1B07FFFF）------------------------------
#define FLASH_M0_BASE_ADDR              (0x1B000000)            //M0-Flash基地址  

//(M0应用程序占用256K)
#define FLASH_M0_APP_START_ADDR         (0x1B000000)            //M0应用程序起始地址  
#define FLASH_M0_APP_END_ADDR           (0x1B03FFFF)            //M0应用程序结束地址 




//(最后的64K用于存放系统参数)
#define FLASH_M0_SYS_PARM_START_ADDR    (0x1B070000)            //M0系统参数存储区起始地址
#define FLASH_M0_SYS_PARM_END_ADDR        (0x1B07FFFF)            //M0系统参数存储区结束地址






//-------------------------------------M0-Flash地址定义（0x1B000000----0x1B07FFFF）------------------------------
/*
扇区0（8K）： 存放系统配置信息、控制参数
扇区1-3(24K)：存放轴螺距补偿参数
扇区4-6(24K)：存放刀库表及坐标系数据
扇区7-8(72K):存放固定循环代码数据
扇区9-14（384K）：存放M0程序代码数据
*/
/*
#define FLASH_M0_BASE_ADDR                  (0x1B000000)            //M0-Flash基地址  

//第1扇区(8K Byte)用于存放系统配置信息、控制参数（0x1B000000----0x1B001FFF）
#define FLASH_M0_SYS_CONFPARM_START_ADDR    (0x1B000000)
#define FLASH_M0_SYS_CONFPARM_END_ADDR        (0x1B001FFF)
#define FLASH_M0_SYS_CONFPARM_SIZE            (0x2000)

//第1扇区的第1-2页，共1K Byte用于存放系统配置信息
#define FLASH_M0_SYS_CONFIG_START_ADDR        (0x1B000000)            
#define FLASH_M0_SYS_CONFIG_END_ADDR        (0x1B0003FF)
#define FLASH_M0_SYS_CONFIG_SIZE            (0x400)

//第1扇区的第3-8页，共7K Byte用于存放系统控制参数、通道参数和轴控参数
#define FLASH_M0_SYS_PARM_START_ADDR        (0x1B000400)            
#define FLASH_M0_SYS_PARM_END_ADDR            (0x1B001FFF)    
#define FLASH_M0_SYS_PARM_SIZE                (0x1C00)    

//第2扇区到第4扇区，共24K Byte用于存放轴螺距补偿参数
#define FLASH_M0_PITCH_PARM_START_ADDR      (0x1B002000)
#define FLASH_M0_PITCH_PARM_END_ADDR        (0x1B007FFF)    
#define FLASH_M0_PITCH_PARM_SIZE            (0x6000)    

//第5扇区到第7扇区，共24K Byte用于存放IPR刀库及坐标系数据
#define FLASH_M0_CRD_TOOLBASE_START_ADDR    (0x1B008000)
#define FLASH_M0_CRD_TOOLBASE_END_ADDR      (0x1B00DFFF)
#define FLASH_M0_CRD_TOOLBASE_SIZE          (0x6000)

//第8扇区到第9扇区，共72K Byte用于存放车床固定循环代码数据
#define FLASH_M0_CC_FILE_START_ADDR         (0x1B00E000)
#define FLASH_M0_CC_FILE_END_ADDR           (0x1B01FFFF)
#define FLASH_M0_CC_FILE_SIZE               (0x12000)

//第10扇区到15扇区，共384K Byte用于存放M0应用程序代码
#define FLASH_M0_APP_START_ADDR             (0x1B020000)            //M0应用程序起始地址  
#define FLASH_M0_APP_END_ADDR               (0x1B07FFFF)            //M0应用程序结束地址 
#define FLASH_M0_APP_SIZE                   (0x60000)               // 
*/





#endif