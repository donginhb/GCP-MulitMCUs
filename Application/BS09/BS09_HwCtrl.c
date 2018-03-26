/**
  ******************************************************************************
  * @file    GCIOCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
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
#include "BS09_HwCtrl.h"
#include "BS09_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/KEY/KEY.h"


#include "ExtPeripheral/LCD/LCD.h"
#include "ExtPeripheral/Eeprom/Eeprom.h"

#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
static void BS09_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //初始化KEY引脚
    GPIO_InitIOGroup(&g_GcpIOTable.pInputGroup[INPUT_IO_KEY_0], 12, GOIO_MODE_INPUT_PUSH_UP);
    
    //翻转按键电平逻辑
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_0, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_1, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_2, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_3, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_4, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_5, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_6, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_7, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_8, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_9, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_10, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_11, true);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D0, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D1, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D2, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D3, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D4, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D5, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D6, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D7, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_EN, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_RW, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_RS, false);
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS09_HwInit(void)
{
    //初始化IO
    BS09_IOConfig();
    
    //初始化按键
    uBit32 ulKeyPinGourp[] = {INPUT_IO_KEY_0, INPUT_IO_KEY_1, INPUT_IO_KEY_2,  INPUT_IO_KEY_3,
                              INPUT_IO_KEY_4, INPUT_IO_KEY_5, INPUT_IO_KEY_6,  INPUT_IO_KEY_7,
                              INPUT_IO_KEY_8, INPUT_IO_KEY_9, INPUT_IO_KEY_10, INPUT_IO_KEY_11,};
    
    KEY_SetScanPinGroup(ulKeyPinGourp, sizeof(ulKeyPinGourp)/sizeof(ulKeyPinGourp[0]));
    
    //初始化串口
    UART_Init(BS09_COM_UART_NODE, 9600);
    
    //初始化LCD
    uBit32 ulPortNoGroup[] = {OUTPUT_IO_LCD_D0, OUTPUT_IO_LCD_D1, OUTPUT_IO_LCD_D2, OUTPUT_IO_LCD_D3,
                              OUTPUT_IO_LCD_D4, OUTPUT_IO_LCD_D5, OUTPUT_IO_LCD_D6, OUTPUT_IO_LCD_D7};
    
    LCD_Init(ulPortNoGroup, OUTPUT_IO_LCD_EN, OUTPUT_IO_LCD_RW, OUTPUT_IO_LCD_RS);
    LCD_Clear();
    LCD_WriteStr(0, 0, "NRF COM...");
    
    //初始化EEPROM
    EEPROM_Init(OUTPUT_IO_AT_SDA, OUTPUT_IO_AT_SCL);
    
    LCD_Clear();
        
    if (EEPROM_CheckRW())
    {
        LCD_WriteStr(0, 0, "AT: ERR!");
    }
    else 
    {
        LCD_WriteStr(0, 0, "AT: SUCCESS!");
    }
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS09_LED_TOGGLE_TIME             (100)            //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS09_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS09_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
    }

}


/*****************************************************************************
 * 按键扫描线程接口
 ****************************************************************************/
#define BS09_KEY_SCAN_INTERVAL       (50)         //按键扫描间隔(MS)
static  SYS_TIME_DATA m_KeyScanTimer = {1};     //扫描定时器

/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void BS09_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_KeyScanTimer))
    {
        SysTime_StartOneShot(&m_KeyScanTimer, BS09_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        switch (ulCurTrg)
        {
        case 0x0001: LCD_Clear(); LCD_WriteStr(0, 0, "1");break;
        case 0x0002: LCD_Clear(); LCD_WriteStr(0, 0, "2");break;
        case 0x0004: LCD_Clear(); LCD_WriteStr(0, 0, "3");break;
        case 0x0008: LCD_Clear(); LCD_WriteStr(0, 0, "4");break;
        case 0x0010: LCD_Clear(); LCD_WriteStr(0, 0, "5");break;
        case 0x0020: LCD_Clear(); LCD_WriteStr(0, 0, "6");break;
        case 0x0040: LCD_Clear(); LCD_WriteStr(0, 0, "7");break;
        case 0x0080: LCD_Clear(); LCD_WriteStr(0, 0, "8");break;
        case 0x0100: LCD_Clear(); LCD_WriteStr(0, 0, "9");break;
        case 0x0200: LCD_Clear(); LCD_WriteStr(0, 0, "*");break;
        case 0x0400: LCD_Clear(); LCD_WriteStr(0, 0, "0");break;
        case 0x0800: LCD_Clear(); LCD_WriteStr(0, 0, "#");break;
            
            
        }
        
    }
      
}

