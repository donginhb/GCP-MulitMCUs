/**
  ******************************************************************************
  * @file    hw_can.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.07.21
  * @brief   CAN drivers
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *     CAN1_TD  --- P0_1(Fun1),  P0_22(Fun3),
  *     CAN1_RD  --- P0_0(Fun1),  P0_21(Fun3),
  *                  
  *     CAN2_TD  --- P0_5(Fun2),  P2_8(Fun1),
  *     CAN2_RD  --- P0_4(Fun2),  P2_7(Fun1),
  * 
  * V1.1------------
  * 修改描述: 为CAN的发送函数增加超时操作
  * 修改原因: 如果当前CAN总线没有连接外挂设备的时候,调用CAN发送函数,会造成发送阻
  *           塞,从而导致系统卡死.为避免这种情况,为发送函数增加发送超时退出的功能.
  * 修改作者: Duhanfneg
  * 当前版本: V1.1
  * 修改日期: 2017.09.05
  * 
  * 需要补充的功能: 按照位的ID过滤
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Can.h"
#include "HW_SysTick.h"
#include "chip.h"
#include <string.h>
#include <stdio.h>


/*****************************************************************************
 * 私有类型/变量定义
 ****************************************************************************/

//CAN数量定义
#define HW_CAN_COUNT       (2)

//static CAN_TypeDef * const CAN[HW_CAN_COUNT] = {(CAN_TypeDef *)CAN1_BASE, (CAN_TypeDef *)CAN2_BASE};    //CAN基地址

/**
  * @brief  CAN_IO_MUX初始化(选定某组引脚作为CAN的引脚).
  * @param  CAN 外设指针
  * @retval None
  */
static void HW_CAN_IO_Init(uint8_t uCanNode)
{
    

}


/*****************************************************************************
 * CAN 基本功能接口函数
 ****************************************************************************/

/**
  * @brief  CAN总线初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval None
  */
void HW_CAN_Init(uint8_t uCanNode, uint32_t ulBitRate)
{
    //初始化IO
    HW_CAN_IO_Init(uCanNode);
    
    
}


/**
  * @brief  CAN使能
  * @param  uCanNode CAN节点号
  * @param  bIsEnable 使能标志
  * @retval None
  */
void HW_CAN_Enable(uint8_t uCanNode, bool bIsEnable)
{
    if (bIsEnable)
    {
        
    }
    else 
    {
        
    }
    
}


/**
  * @brief  数据发送
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_SendMsg(uint8_t uCanNode)
{
    //发送数据
    
    //开始超时计时
    uint32_t ulTick = HW_SysTick_GetCount();
    
    //等待发送完成
    while (0)
    {
        //发送超时
        if ((HW_SysTick_GetCount() - ulTick) >= 200)
        {
            return 1;
        }
    }
    
    return 0;
}


/**
  * @brief  数据接收
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_RecvMsg(uint8_t uCanNode)
{
    
    
    return 1;
}


