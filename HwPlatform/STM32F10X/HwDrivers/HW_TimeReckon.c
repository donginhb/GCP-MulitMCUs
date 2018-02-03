/**
  ******************************************************************************
  * @file    HW_TimeReckon.h
  * @author  Duhanfeng
  * @version V1.0
  * @date    2018.01.30
  * @brief   hardware time reckon drivers
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "HW_TimeReckon.h"
#include "HW_TimeComm.h"
#include "chip.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * 定时器计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  None
  * @retval None
  */
void HW_TIME_InitReckonMode(uint8_t uTimeNode)
{
    //开时钟
    HW_TIM_ClockEnable(uTimeNode);
    
    //配置时序参数
    TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE(1*1000*1000); //计数频率为1MHz
    TIM[uTimeNode]->ARR = TIM_ARR_ARR;       //计数器每记2个数为1ms
    
    //配置工作模式
    TIM[uTimeNode]->CR1 |=  TIM_CR1_ARPE;   //开影子
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_OPM;    //非单脉冲
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_URS;    //配置更新源:允许软件更新
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_UDIS;   //使能更新
    
    //配置事件/中断
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UDE;  //禁止更新DMA请求
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UIE;  //禁止更新中断
    TIM[uTimeNode]->SR   &= ~TIM_SR_UIF;    //清标志位
    
    //关闭定时器
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CEN;
    
}


/**
  * @brief  定时器最小计数单位设置(默认1US)
  * @param  uTimeNode 定时器节点
  * @param  uTimeNode 最小计时单位(us)
  * @retval None
  * @note   计时单位越小,计时的精度越高,同时计时的范围越小,受限于定时器的位数.
  *         系统初始化时默认1US计时一次,若要获取更广的范围或者更高的精度,则可
  *         以通过这个接口来配置,以适应不同的应用场合;
  */
void HW_TIME_SetMinReckonUnit(uint8_t uTimeNode, uint32_t ulMinReckonUnit)
{
    //关闭定时器
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CEN;
    
    //设置
    TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE((1*1000*1000)/ulMinReckonUnit); //计数频率为1MHz
    
}


/**
  * @brief  计时开始
  * @param  None
  * @retval None
  */
void HW_TIME_StartReckon(uint8_t uTimeNode)
{
    //启动定时器
    TIM[uTimeNode]->EGR |=  TIM_EGR_UG;   //给更新,刷新影子
    TIM[uTimeNode]->SR  &= ~TIM_SR_UIF;   //清标志位
    TIM[uTimeNode]->CR1 |=  TIM_CR1_CEN;  //开启定时器  
    
}


/**
  * @brief  计时停止
  * @param  None
  * @retval None
  */
void HW_TIME_StopReckon(uint8_t uTimeNode)
{
    //关闭定时器
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CEN;
    
}


/**
  * @brief  计时值获取
  * @param  None
  * @retval 计时值(单位: us)
  */
uint16_t HW_TIME_GetReckonValue(uint8_t uTimeNode)
{
    
    return TIM[uTimeNode]->CNT;
}


/**
  * @brief  计时值清零
  * @param  None
  * @retval None
  */
void HW_TIME_ClearRecvkonValue(uint8_t uTimeNode)
{
    //关闭定时器
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CEN;
    
    //清空计数
    TIM[uTimeNode]->CNT = 0;
    
}
