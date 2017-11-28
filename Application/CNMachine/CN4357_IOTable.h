#ifndef  __CN4357_IO_TABLE_H
#define  __CN4357_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_RESERCEDR0 = 0,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED
    OUTPUT_IO_LED1,

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_CN4357_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __CN4357_IO_TABLE_H */
