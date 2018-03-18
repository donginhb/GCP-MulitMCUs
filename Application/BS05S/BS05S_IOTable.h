#ifndef  __BS05S_IO_TABLE_H
#define  __BS05S_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_KEY_AUTO = 0,
    INPUT_IO_KEY_LIGHT,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_BS05S_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __BS05S_IO_TABLE_H */
