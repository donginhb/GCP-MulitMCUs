#ifndef  __ALIENTEK_IO_TABLE_H
#define  __ALIENTEK_IO_TABLE_H


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

}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    OUTPUT_IO_LED0 = 0,     //LED
    OUTPUT_IO_LED1,
    OUTPUT_IO_BEEP,         //BEEP
    OUTPUT_IO_SCL,          //IIC
    OUTPUT_IO_SDA,
    OUTPUT_IO_DS18B20_BUS,  //DS18B20_BUS

}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_ATKIOTable;
    
    

#ifdef __cplusplus
}
#endif


#endif /* __DEMO_H */
