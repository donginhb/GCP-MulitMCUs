/**
  ******************************************************************************
  * @file    HW_TimeDelay.c
  * @author  Duhanfneg
  * @version V1.1 
  * @date    2017.11.20
  * @brief   TIM delay drivers
  ******************************************************************************
  * @attention
  * 
  * V1.1------------
  * 修改描述: 规范接口,作为GCP(General control platform)的硬件抽象层使用
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2017.11.20
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_TimeDelay.h"
#include "HW_TimeComm.h"
#include "chip.h"



/*****************************************************************************
 * 延时功能定时器接口函数
 ****************************************************************************/

/**
  * @brief  定时器初始化
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HW_TIM_DelayInit(uint8_t uTimeNode)
{
    //开时钟
    HW_TIM_ClockEnable(uTimeNode);
    
    //配置工作模式
    TIM[uTimeNode]->CR1  |=  TIM_CR1_ARPE;   //开影子
    TIM[uTimeNode]->CR1  |=  TIM_CR1_OPM;    //单脉冲模式
    TIM[uTimeNode]->CR1  &= ~TIM_CR1_URS;    //配置更新源
    TIM[uTimeNode]->CR1  &= ~TIM_CR1_UDIS;   //使能更新源
    
    //配置事件,中断
    TIM[uTimeNode]->EGR  |=  TIM_EGR_UG;    //开启更新事件
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UDE;  //禁止更新DMA
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UIE;  //禁止更新中断
    
    //关定时器
    TIM[uTimeNode]->CR1  &= ~TIM_CR1_CEN;  
    
}


/**
  * @brief  us级延时
  * @param  uTimeNode 定时器节点
  * @param  要延时的us数,取值范围为: [0-65535]
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_TIM_DelayUs(uint8_t uTimeNode, uint32_t ulUs)
{
    if (!ulUs) return;
    
    //配置时序参数
    TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000); //计数频率为1MHz
    TIM[uTimeNode]->ARR = ulUs;             //计数器每记1个数为1us
    TIM[uTimeNode]->EGR  |=  TIM_EGR_UG;    //给更新,刷新影子
    TIM[uTimeNode]->SR   &= ~TIM_SR_UIF;    //清标志位
    
    //开定时器
    TIM[uTimeNode]->CR1  |=  TIM_CR1_CEN;
    
    //等待计数完成
    while (!(TIM[uTimeNode]->SR & TIM_SR_UIF)); //等待计数完成
    
}


/**
  * @brief  ms级延时
  * @param  uTimeNode 定时器节点
  * @param  要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_TIM_DelayMs(uint8_t uTimeNode, uint32_t ulMs)
{
    if (!ulMs) return;
    
    //配置时序参数
    TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE(2000);   //计数频率为2KHz
    TIM[uTimeNode]->ARR = 2 * ulMs;        //计数器每记2个数为1ms
    TIM[uTimeNode]->EGR  |=  TIM_EGR_UG;  //给更新,刷新影子
    TIM[uTimeNode]->SR   &= ~TIM_SR_UIF;  //清标志位
    
    //开定时器
    TIM[uTimeNode]->CR1  |=  TIM_CR1_CEN;
    
    //等待计数完成
    while (!(TIM[uTimeNode]->SR & TIM_SR_UIF)); //等待计数完成
}
