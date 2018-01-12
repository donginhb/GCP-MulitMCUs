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
    //10个电机INDEX信号
    INPUT_IO_KEY1,
    INPUT_IO_KEY2,
    INPUT_IO_KEY3,
    INPUT_IO_KEY4,


}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    //LED指示灯 RUN1
    OUTPUT_IO_LED_RUN1 = 0,
    OUTPUT_IO_LED_RUN2,
    OUTPUT_IO_LED_RUN3, 
    OUTPUT_IO_LED_RUN4

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_DS1768_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __VD_MACHINE_IO_H */
