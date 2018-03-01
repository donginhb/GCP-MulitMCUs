#ifndef  __DS_IO_DEF_H
#define  __DS_IO_DEF_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


#if 0
//输入IO编号
typedef enum
{
    INPUT_IO_RF_IRQ,

}INPUT_IO_TABLE;
#endif

//输入IO编号
typedef enum
{
    INPUT_IO_HUMAN_DETECT,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
#if 0
    //LED指示灯
    OUTPUT_IO_LED_RUN1 = 0,
    
    OUTPUT_IO_RF_CE,
    OUTPUT_IO_RF_CSN,
    
    OUTPUT_IO_LED,
#endif
    
    OUTPUT_IO_LED_RUN1 = 0,
    OUTPUT_IO_BEEP,
    OUTPUT_IO_OUT,

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_IRB030_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __VD_MACHINE_IO_H */
