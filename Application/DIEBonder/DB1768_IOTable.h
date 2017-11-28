#ifndef  __DB1768_IO_TABLE_H
#define  __DB1768_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_KEY0 = 0,
    INPUT_IO_KEY1,
    INPUT_IO_KEY2,
    INPUT_IO_KEY3,
    INPUT_IO_KEY4,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED
    OUTPUT_IO_LED1,
    OUTPUT_IO_LED2,
    OUTPUT_IO_LED3,

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_DB1768_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __DB1768_IO_TABLE_H */
