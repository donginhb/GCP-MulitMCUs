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
    //KEY
    INPUT_IO_KEY_0,
    INPUT_IO_KEY_1,
    INPUT_IO_KEY_2,
    INPUT_IO_KEY_3,
    INPUT_IO_KEY_4,
    INPUT_IO_KEY_5,
    INPUT_IO_KEY_6,
    INPUT_IO_KEY_7,
    INPUT_IO_KEY_8,
    INPUT_IO_KEY_9,
    INPUT_IO_KEY_10,
    INPUT_IO_KEY_11,

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    //LED指示灯 RUN1
    OUTPUT_IO_LED_RUN1 = 0,
    
    //LCD IO
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
    
    //AT24C08
    OUTPUT_IO_AT_SDA,
    OUTPUT_IO_AT_SCL,

    
}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_BS09_IOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __VD_MACHINE_IO_H */
