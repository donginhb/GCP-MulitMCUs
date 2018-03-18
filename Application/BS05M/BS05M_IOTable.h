#ifndef  __BS05M_IO_TABLE_H
#define  __BS05M_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_HUMAN_DETECT = 0,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED

    //LCD1602 引脚
    OUTPUT_IO_LED_SUM,      //光照检测指示
    OUTPUT_IO_LED_HUMAN,    //自动模式选择
    OUTPUT_IO_LED_AUTO,     //输出
    
}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_BS05M_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __BS05M_IO_TABLE_H */
