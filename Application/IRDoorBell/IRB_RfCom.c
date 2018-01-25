/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.26
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "IRB_RfCom.h"
#include "IRB_HardwareDef.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SPI/SPI.h"
#include "ExtPeripheral/RF/RF.h"

#include <string.h>


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define IRB_BUFF_LEN     (64)

#define IRB_SET_LED_OPEN            "LED_OPEN."
#define IRB_SET_LED_CLOSE           "LED_CLOSE."
#define IRB_SET_LED_AUTO            "LED_AUTO."

static uBit8 uCmdBuff[IRB_BUFF_LEN] = {0};   //定义指令缓冲区

/*****************************************************************************
 * RF通信相关控制接口
 ****************************************************************************/

/**
  * @brief  RF通信初始化
  * @param  None
  * @retval None
  */
void RF_InitCom(void)
{
    RF_Init(SPI_NODE_0, INPUT_IO_RF_IRQ, OUTPUT_IO_RF_CSN, OUTPUT_IO_RF_CE);
    
}


/**
  * @brief  RF通信处理
  * @param  None
  * @retval None
  */
void RF_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //接收处理
    RF_RecvHandler();
    
    //如果成功接收到数据
    if (RF_RecvBuff(&uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            //LED灯启动
            if (memcmp(uCmdBuff, IRB_SET_LED_OPEN, strlen(IRB_SET_LED_OPEN)) == 0)
            {
                //执行指令
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED, true);
                
                //返回应答
                RF_SendBuff("OK.", strlen("OK."));
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //LED灯关闭
            else if (memcmp(uCmdBuff, IRB_SET_LED_CLOSE, strlen(IRB_SET_LED_CLOSE)) == 0)
            {
                //执行指令
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED, false);
                
                //返回应答
                RF_SendBuff("OK.", strlen("OK."));
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //LED灯自动控制
            else if (memcmp(uCmdBuff, IRB_SET_LED_AUTO, strlen(IRB_SET_LED_AUTO)) == 0)
            {
                //执行指令
                
                
                //返回应答
                RF_SendBuff("OK.", strlen("OK."));
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
        }
        
        ulRxIndex++;
    }
    
    
    
}