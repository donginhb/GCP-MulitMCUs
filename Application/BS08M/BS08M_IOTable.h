#ifndef  __BS08M_IO_TABLE_H
#define  __BS08M_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    INPUT_IO_KEY0 = 0,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED

    //LCD1602 引脚
    OUTPUT_IO_LCD_D0,
    OUTPUT_IO_LCD_D1,
    OUTPUT_IO_LCD_D2,
    OUTPUT_IO_LCD_D3,
    OUTPUT_IO_LCD_D4,
    OUTPUT_IO_LCD_D5,
    OUTPUT_IO_LCD_D6,
    OUTPUT_IO_LCD_D7,
    OUTPUT_IO_LCD_EN,
    OUTPUT_IO_LCD_RW,
    OUTPUT_IO_LCD_RS,
    
}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_BS08M_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __BS08M_IO_TABLE_H */
