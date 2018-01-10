/**
  ******************************************************************************
  * @file    LCD.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.10
  * @brief   LCD application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "LCD.h"
#include "LCD1602.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define LCD_DATA_PORT_COUNT     (8)     //LCD并口位数
static uBit32 m_ulDataNoGroup[LCD_DATA_PORT_COUNT] = {0};    //LCD并口数据脚编号组
static uBit32 m_ulRSNo = 0;
static uBit32 m_ulENNo = 0;
static uBit32 m_ulRWNo = 0;


/**
  * @brief  LCD并口数据脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void LCD_PORT(uBit8 uData)
{
    for (int i = 0; i < LCD_DATA_PORT_COUNT; i++)
    {
        GPIO_SetOutputState(m_ulDataNoGroup[i], (uData & (0x1<<i)) ? true : false);
    }
    
}


/**
  * @brief  LCD复位脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void LCD_RS(bool bState)
{
    GPIO_SetOutputState(m_ulRSNo, bState);
    
}


/**
  * @brief  LCD使能脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void LCD_EN(bool bState)
{
    GPIO_SetOutputState(m_ulENNo, bState);
    
}


/**
  * @brief  LCD读写使能脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void LCD_RW(bool bState)
{
    GPIO_SetOutputState(m_ulRWNo, bState);
    
}


/*****************************************************************************
 * LCD相关控制接口
 ****************************************************************************/

/**
  * @brief  LCD 初始化
  * @param  pDataNoGroup 并口IO编号组
  * @param  ulENNo EN 脚编号
  * @param  ulRWNo RW 脚编号
  * @param  ulRSNo RS 脚编号
  * @retval 0-成功 非0-失败
  */
uBit32 LCD_Init(uBit32 *pDataNoGroup, uBit32 ulENNo, uBit32 ulRWNo, uBit32 ulRSNo)
{
    uBit32 ulRet = 0;
    
    memcpy(m_ulDataNoGroup, pDataNoGroup, sizeof(m_ulDataNoGroup));
    m_ulRSNo = ulRSNo;
    m_ulRWNo = ulRWNo;
    m_ulENNo = ulENNo;
    
    ulRet = LCD1602_InitInterfaces(LCD_PORT, LCD_RS, LCD_EN, LCD_RW, SysTime_DelayMs);
    
    return ulRet;
}


/**
  * @brief  字符串输入
  * @param  pStr 字符串,必须以'\0'结束
  * @retval 无
  */
void LCD_WriteStr(uBit8 *pStr)
{
    LCD1602_WriteStr(pStr);
    
}
