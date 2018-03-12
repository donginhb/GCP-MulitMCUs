#ifndef  __WB01_IO_TABLE_H
#define  __WB01_IO_TABLE_H


#ifdef __cplusplus
extern "C"
{
#endif
    
#include "SysPeripheral/GPIO/GPIO_Man.h"


//输入IO编号
typedef enum
{
    //主轴电机检测
    INPUT_IO_MAIN_AXIS_UP = 0,  //上限位 0-触发 1-空闲
    INPUT_IO_MAIN_AXIS_DOWN,    //下限位 0-触发 1-空闲
    INPUT_IO_MAIN_AXIS_STOP,    //停止位 0-触发 1-空闲
    
    //霍尔检测
    INPUT_IO_HALL_SENSOR,       //0-触发 1-空闲
    
    //进货门电机
    INPUT_IO_IN_DOOR_OPEN_LIMIT,    //0-触发 1-空闲
    INPUT_IO_IN_DOOR_CLOSE_LIMIT,   //0-触发 1-空闲
    
    //出货门电机
    INPUT_IO_OUT_DOOR_OPEN_LIMIT,   //0-触发 1-空闲
    INPUT_IO_OUT_DOOR_CLOSE_LIMIT,  //0-触发 1-空闲
    
    //进出货门红外检测
    INPUT_IO_GOODS_DECTECT,     //0-空闲 1-触发
    
    //人体红外检测
    INPUT_IO_HUMAN_DECTECT,     //0-空闲 1-触发
    
    //货道电机限位检测
    INPUT_IO_CH_MOTOR_LIMIT,    //0-触发 1-空闲
    
}INPUT_IO_TABLE;


//输出IO编号
typedef enum
{
    //LED灯
    OUTPUT_IO_LED0 = 0,         //LED
    
    //主轴电机控制
    OUTPUT_IO_MAIN_AXIS_EN,      //正转
    OUTPUT_IO_MAIN_AXIS_DIR,      //反转
    OUTPUT_IO_MAIN_AXIS_S,      //刹车
    
    //传输皮带电机
    OUTPUT_IO_TRANSF_MOTOR,
    
    //进货门电机
    OUTPUT_IO_IN_DOOR_DIR,      //入货门电机方向控制 0-正转 1-反转
    OUTPUT_IO_IN_DOOR_EN,       //入货门电机使能  0-断使能 1-使能
    
    //出货门电机
    OUTPUT_IO_OUT_DOOR_DIR,     //出货门电机方向控制 0-正转 1-反转
    OUTPUT_IO_OUT_DOOR_EN,      //出货门电机使能  0-断使能 1-使能
    
    //货道电机控制(74HC595)
    OUTPUT_IO_HC595_SCK,
    OUTPUT_IO_HC595_RCK,
    OUTPUT_IO_HC595_G,
    OUTPUT_IO_HC595_SI,
    
    //光栅供电开关
    OUTPUT_IO_LIGHT_POWER,
    
}OUTPUT_IO_TABLE;


//格子柜控制IO资源表
extern GPIO_CTRL_TABLE g_WB01_IOTable;
    

#ifdef __cplusplus
}
#endif


#endif /* __WB01_IO_TABLE_H */
