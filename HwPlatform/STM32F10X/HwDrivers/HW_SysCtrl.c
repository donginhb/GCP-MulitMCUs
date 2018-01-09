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
 * 系统复位相关
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


/*****************************************************************************
 * 系统NVIC相关
 ****************************************************************************/

/**
  * @brief  NVIC使能
  * @param  IRQn 中断号
  * @param  ulPreemptPrio  抢占优先级
  * @param  ulSubPrio  响应优先级
  * @retval None
  */
void HW_NVIC_Enable(IRQn_Type IRQn, uint32_t ulPreemptPrio, uint32_t ulSubPrio)
{
    NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
    NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, ulPreemptPrio, ulSubPrio));  
    NVIC_EnableIRQ(IRQn); 
    
}


/**
  * @brief  禁止对应的内核中断
  * @param  IRQn 中断号
  * @retval None
  */
void HW_NVIC_DisableIRQ(IRQn_Type IRQn)
{ 
    NVIC_DisableIRQ((IRQn));
    
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


/*****************************************************************************
 * 系统唤醒休眠相关
 ****************************************************************************/

/**
  * @brief  WFI指令(汇编)
  * @param  None
  * @retval None
  * @note   IAR下的汇编调用尚未验证过
  */
#if 1
void HW_WFI_SET(void)  //IAR下调用
{
    __asm("WFI");    
}
#else 
__asm void HW_WFI_SET(void)//MDK下调用
{
    WFI;    
}

#endif


/**
  * @brief  系统待机
  * @param  None
  * @retval None
  */
static void HW_SystemStandby(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP;          //使能SLEEPDEEP位 (SYS->CTRL)       
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;      //使能电源时钟        
    PWR->CSR |= PWR_CSR_EWUP;               //设置WKUP用于唤醒
    PWR->CR  |= PWR_CSR_PVDO;               //清除Wake-up标志
    PWR->CR  |= PWR_CSR_SBF;                //PDDS置位          
    HW_WFI_SET();                           //执行WFI指令         
    
}


/**
  * @brief  待机模式进入
  * @param  None
  * @retval None
  */
void HW_EnterSrandby(void)
{
    //关闭所有外设(根据实际情况写)
    RCC->APB2RSTR |= 0X01FC;//复位所有IO口
    HW_SystemStandby();//进入待机模式
    
}
