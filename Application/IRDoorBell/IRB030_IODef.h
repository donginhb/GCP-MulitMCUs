#ifndef  __DS_IO_DEF_H
#define  __DS_IO_DEF_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_RF_IRQ,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    //LED指示灯 RUN1
    OUTPUT_IO_LED_RUN1 = 0,
    
    OUTPUT_IO_RF_CE,
    OUTPUT_IO_RF_CSN,

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_IRB030_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __VD_MACHINE_IO_H */
