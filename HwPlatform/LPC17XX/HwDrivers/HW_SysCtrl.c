/**
  ******************************************************************************
  * @file    HW_SysCtrl.h
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.19
  * @brief   MCU Ctrl Interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "HW_SysCtrl.h"
#include "chip.h"

/*****************************************************************************
 * 全局变量
 ****************************************************************************/

//系统晶振频率以及RTC晶振频率
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;


/*****************************************************************************
 * 系统初始化
 ****************************************************************************/

/**
  * @brief  系统初始化(在main函数之前中执行).
  * @param  None
  * @retval None
  * @note   由于库函数提供的系统初始化函数中,会将芯片内核频率设置为120M,而
  *         LPC1752最高内核频率是100M,直接调用库函数进行系统初始化会导致系
  *         统不稳定,所以在这里重新实现其内核的初始化.
  */
void Board_SystemInit(void)
{
    //配置系统时钟
    Chip_SetupXtalClocking();
    
    //初始化 GPIO
    Chip_GPIO_Init(LPC_GPIO);
    Chip_IOCON_Init(LPC_IOCON);
    
    //初始化DMA
    Chip_GPDMA_Init(LPC_GPDMA); 

}


/*****************************************************************************
 * 系统相关控制接口
 ****************************************************************************/

/**
  * @brief  系统复位函数
  * @param  None
  * @retval None
  */
void HW_SystemReset(void)
{
    NVIC_SystemReset(); //复位
    
}


/**
  * @brief  程序跳转
  * @param  None
  * @retval None
  * @note   此函数应用于Bootloader对APP的跳转,APP地址写死为0x00007000
  */
void HW_JumToApp(void)
{
    //关闭总中断	
    __disable_irq();
    
    asm("nop");
    
    //堆栈指针入口地址,该部分随着bootloader空间改变而改变
    asm("MOVW R0,#0X7000");
    asm("MOVT R0,#0X0000");
    
    //中断向量表的相对于基地址的偏移
    asm("MOVW R1,#0XED08");
    asm("MOVT R1,#0XE000");
    asm("STR R0, [R1]");
    
    //堆栈指针
    asm("LDR SP,[R0]");
    
    //应用程序入口地址,该部分随着bootloader控件改变而改变
    asm("MOVW R0,#0X7004");
    asm("MOVT R0,#0X0000");
    
    //跳转到应用程序入口处
    asm("LDR LR,[R0]");
    asm("BX LR");
    
}


/**
  * @brief  总中断使能
  * @param  None
  * @retval None
  */
void HW_EnableIRQ(void)
{
    __enable_irq();
    
}


/**
  * @brief  总中断关闭
  * @param  None
  * @retval None
  */
void HW_DisableIRQ(void)
{
    __disable_irq();
    
}
