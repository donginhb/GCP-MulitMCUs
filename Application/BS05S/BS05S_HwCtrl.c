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
#include "BS05S_HwCtrl.h"
#include "BS05S_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/KEY/KEY.h"
      
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
static void BS05S_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //翻转IO逻辑
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_AUTO, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY_LIGHT, true);
   
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS05S_HwInit(void)
{
    //初始化IO
    BS05S_IOConfig();
    
    //初始化串口
    UART_Init(BS05S_COM_UART_NODE, 9600);
    
    //初始化按键
    uBit32 ulKeyPinGourp[] = {INPUT_IO_KEY_AUTO, INPUT_IO_KEY_LIGHT};
    KEY_SetScanPinGroup(ulKeyPinGourp, sizeof(ulKeyPinGourp)/sizeof(ulKeyPinGourp[0]));
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS05S_LED_TOGGLE_TIME          (100)     //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS05S_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS05S_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
    }

}


/*****************************************************************************
 * 按键扫描线程接口
 ****************************************************************************/

#define BS05S_KEY_SCAN_INTERVAL       (50)         //按键扫描间隔(MS)
static  SYS_TIME_DATA m_KeyScanTimer = {1};     //扫描定时器


static bool m_bCurAutoMode = true;
static bool m_bCurAutoLight = false;


#define BS05S_CMD_SET_AUTO_MODE_EN          "AUTO_MODE:EN."
#define BS05S_CMD_SET_AUTO_MODE_DIS         "AUTO_MODE:DIS."

#define BS05S_CMD_SET_LIGHT_EN              "LIGHT:EN."
#define BS05S_CMD_SET_LIGHT_DIS             "LIGHT:DIS."


/**
  * @brief  按键检测处理
  * @param  None
  * @retval None
  */
void BS05S_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_KeyScanTimer))
    {
        SysTime_StartOneShot(&m_KeyScanTimer, BS05S_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        if (ulCurTrg == 1)
        {
            m_bCurAutoMode = !m_bCurAutoMode;
            
            //发送指令
            if (m_bCurAutoMode)
            {
                UART_SendBuff(BS05S_COM_UART_NODE, BS05S_CMD_SET_AUTO_MODE_EN, strlen(BS05S_CMD_SET_AUTO_MODE_EN));
            }
            else 
            {
                UART_SendBuff(BS05S_COM_UART_NODE, BS05S_CMD_SET_AUTO_MODE_DIS, strlen(BS05S_CMD_SET_AUTO_MODE_DIS));
            }
            
        }
        else if (ulCurTrg == 2)
        {
            m_bCurAutoLight = !m_bCurAutoLight;
            
            //发送指令
            if (m_bCurAutoLight)
            {
                UART_SendBuff(BS05S_COM_UART_NODE, BS05S_CMD_SET_LIGHT_EN, strlen(BS05S_CMD_SET_LIGHT_EN));
            }
            else 
            {
                UART_SendBuff(BS05S_COM_UART_NODE, BS05S_CMD_SET_LIGHT_DIS, strlen(BS05S_CMD_SET_LIGHT_DIS));
            }
        }
    }
      
}

