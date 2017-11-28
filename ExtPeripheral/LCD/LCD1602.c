/**
  ******************************************************************************
  * @file    LCD1602.c
  * @author  Duhanfeng
  * @version V1.0
  * @date    2017.04.10
  * @brief   control lcd1602
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  *     
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "LCD1602.h"
#include "DataType/DataType.h"



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static void (*pf_LCD1602_PORT)(bool bState);
static void (*pf_LCD1602_RS)(bool bState);
static void (*pf_LCD1602_EN)(bool bState);
static void (*pf_LCD1602_RW)(bool bState);
static void (*pf_LCD1602_DelayMs)(uBit32 ulMs);


/**
  * @brief  LCD并口数据脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void (LCD1602_PORT)(bool bState)
{
    if (pf_LCD1602_PORT)
    {
        pf_LCD1602_PORT(bState);
    }
    
}


/**
  * @brief  LCD复位脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void (LCD1602_RS)(bool bState)
{
    if (pf_LCD1602_RS)
    {
        pf_LCD1602_RS(bState);
    }
    
}


/**
  * @brief  LCD使能脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void (LCD1602_EN)(bool bState)
{
    if (pf_LCD1602_EN)
    {
        pf_LCD1602_EN(bState);
    }
    
}


/**
  * @brief  LCD读写使能脚控制
  * @param  bState IO状态,代表实际的电平
  * @retval 无
  */
static void (LCD1602_RW)(bool bState)
{
    if (pf_LCD1602_RW)
    {
        pf_LCD1602_RW(bState);
    }
    
}


/**
  * @brief  LCD毫秒级延时
  * @param  ulMs 要延时的毫秒数
  * @retval 无
  */
static void (LCD1602_DelayMs)(uBit16 ulMs)
{
    if (pf_LCD1602_DelayMs)
    {
        pf_LCD1602_DelayMs(ulMs);
    }
    
}


/**
  * @brief  写指令
  * @param  无
  * @retval 无
  */
static void LCD1602_WriteCmd(uBit8 uCmd)
{
    LCD1602_RS(0);
    LCD1602_DelayMs(1);
    LCD1602_EN(1);
    LCD1602_PORT(uCmd);
    LCD1602_DelayMs(1);
    LCD1602_EN(0);
}


/**
  * @brief  写数据
  * @param  无
  * @retval 无
  */
static void LCD1602_WriteData(uBit8 uData)
{
    LCD1602_RS(1);
    LCD1602_DelayMs(1);    
    LCD1602_PORT(uData);
    LCD1602_EN(1);
    LCD1602_DelayMs(1);
    LCD1602_EN(0);
}


/*****************************************************************************
 * LCD1602相关控制接口
 ****************************************************************************/

/**
  * @brief  NRF24L01 硬件接口
  * @param  pf_LCD_PORT     LCD并口数据脚控制
  * @param  pf_LCD_RS       LCD复位脚控制
  * @param  pf_LCD_EN       LCD使能脚控制
  * @param  pf_LCD_RW       LCD读写使能脚控制
  * @param  pf_LCD_DelayMs  LCD毫秒级延时
  * @retval 0-成功 非0-失败
  */
uBit32 LCD1602_InitInterfaces(void (*pf_LCD_PORT)(bool bState),
                              void (*pf_LCD_RS)(bool bState),
                              void (*pf_LCD_EN)(bool bState),
                              void (*pf_LCD_RW)(bool bState),
                              void (*pf_LCD_DelayMs)(uBit32 ulMs))
{
    do 
    {
        if (!pf_LCD_PORT)    break;
        if (!pf_LCD_RS)      break;
        if (!pf_LCD_EN)      break;
        if (!pf_LCD_RW)      break;
        if (!pf_LCD_DelayMs) break;
        
        //存储指针
        pf_LCD1602_PORT    = pf_LCD_PORT;
        pf_LCD1602_RS      = pf_LCD_RS;
        pf_LCD1602_EN      = pf_LCD_EN;
        pf_LCD1602_RW      = pf_LCD_RW;
        pf_LCD1602_DelayMs = pf_LCD_DelayMs;
        
        //设备初始化
        LCD1602_EN(0);
        LCD1602_RW(0);                  //设置为写状态
        LCD1602_DelayMs(30);
        LCD1602_WriteCmd(0x38);        //显示模式设定
        LCD1602_DelayMs(5);
        LCD1602_WriteCmd(0x0c);        //开关显示、光标有无设置、光标闪烁设置
        LCD1602_WriteCmd(0x06);        //写一个字符后指针加一
        LCD1602_WriteCmd(0x01);        //清屏指令
        
        return 0;
    }while (0);
    
    return 1;
}



/**
  * @brief  字符串输入
  * @param  pStr 字符串,必须以'\0'结束
  * @retval 无
  */
void LCD1602_WriteStr(uBit8 *pStr)
{
    uBit8 i = 0;
    
    while(pStr[i] > 0)
    {
        LCD1602_WriteData(pStr[i]);
        i++;
    }
}

