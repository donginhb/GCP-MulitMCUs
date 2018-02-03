/**
  ******************************************************************************
  * @file    VM_IOInit.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.26
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
/***********************************<INCLUDES>**********************************/
#include "VM_IOInit.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
      

//输出IO编号
typedef enum
{
    //LED指示灯 RUN1
    OUTPUT_IO_LED_RUN1 = 0,
    
    //EEPROM 
    OUTPUT_IO_I2C0_SCL,
    OUTPUT_IO_I2C0_SDA,
    
    //MOTOR DIR
    OUTPUT_IO_MOTOR_DIR,
    
    //4个OE_使能信号
    OUTPUT_IO_OE1_N,
    OUTPUT_IO_OE2_N,
    OUTPUT_IO_OE3_N,
    OUTPUT_IO_OE4_N,
    
    //10个电机行控制信号
    OUTPUT_IO_MOTRO_ROW_OUTPUT1,
    OUTPUT_IO_MOTRO_ROW_OUTPUT2,
    OUTPUT_IO_MOTRO_ROW_OUTPUT3,
    OUTPUT_IO_MOTRO_ROW_OUTPUT4,
    OUTPUT_IO_MOTRO_ROW_OUTPUT5,
    OUTPUT_IO_MOTRO_ROW_OUTPUT6,
    OUTPUT_IO_MOTRO_ROW_OUTPUT7,
    OUTPUT_IO_MOTRO_ROW_OUTPUT8,
    OUTPUT_IO_MOTRO_ROW_OUTPUT9,
    OUTPUT_IO_MOTRO_ROW_OUTPUT10,
    
    //10个电机列控制信号
    OUTPUT_IO_MOTRO_COL_OUTPUT11,
    OUTPUT_IO_MOTRO_COL_OUTPUT12,
    OUTPUT_IO_MOTRO_COL_OUTPUT13,
    OUTPUT_IO_MOTRO_COL_OUTPUT14,
    OUTPUT_IO_MOTRO_COL_OUTPUT15,
    OUTPUT_IO_MOTRO_COL_OUTPUT16,
    OUTPUT_IO_MOTRO_COL_OUTPUT17,
    OUTPUT_IO_MOTRO_COL_OUTPUT18,
    OUTPUT_IO_MOTRO_COL_OUTPUT19,
    OUTPUT_IO_MOTRO_COL_OUTPUT20,   
    
    //7个通用OUTPUT
    OUTPUT_IO_OUTPUT21,
    OUTPUT_IO_OUTPUT22, 
    OUTPUT_IO_OUTPUT23, 
    OUTPUT_IO_OUTPUT24, 
    OUTPUT_IO_OUTPUT25,     //紫外线消毒灯
    OUTPUT_IO_OUTPUT26,     //步进电机抱闸信号
    OUTPUT_IO_OUTPUT27,     //电磁门锁

}OUTPUT_IO_TABLE;
      

//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED指示灯 RUN1
    {0,  15},
    
    //EEPROM 
    {0,  28},
    {0,  27},
    
    //步进电机方向角
    {2,   13},               //DIR
    
    //OE_使能信号
    {0,  24},
    {1,  27},
    {1,  25},
    {0,  25},
    
    //电机行控制信号
    {2,   6},          
    {2,   7},
    {2,   8},
    {2,   9},
    {0,  16},
    {0,  17},
    {0,  18},
    {0,  19},
    {0,  20},
    {0,  21},
    
    //电机列控制信号
    {0,  22},
    {2,  10},
    {2,  11},
    {2,  12},
    {1,  29},
    {1,  28},
    {1,  22},
    {1,  21},
    {1,  19},
    {1,  18},
    
    //通用OUTPUT
    {2,   5},
    {2,   4},
    {2,   3},
    {2,   2},
    {2,   1},
    {2,   0},
    {0,   9},

};


//控制IO表
static GPIO_CTRL_TABLE g_VM_IOTable = 
{
    m_OutputIOTable,
    NULL,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    0,
    
};


/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
void VM_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_VM_IOTable);
    
    //先单独初始化IO使能脚
    GPIO_InitIOGroup(&g_VM_IOTable.pOutputGroup[OUTPUT_IO_OE1_N], 4, GPIO_MODE_OUPUT);  //模式配置
    
    //设置IO使能脚逻辑翻转
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE4_N, true);
    
    //在初始化其他IO之前,禁止IO输出
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, false);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_VM_IOTable);
    
    //对输出IO进行逻辑翻转
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT10, true);
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT11, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT12, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT13, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT14, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT15, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT16, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT17, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT18, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT19, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT20, true);
    //7个通用OUTPUT
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT21, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT22, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT23, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT24, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT25, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT26, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT27, true);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT10, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT12, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT13, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT14, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT15, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT16, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT17, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT18, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT19, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT20, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT21, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT22, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT23, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT24, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT25, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT26, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT27, false);
    
    //使能IO
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, true);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define VM_LED_TOGGLE_TIME            (500)             //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void VM_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, VM_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
    }

}






