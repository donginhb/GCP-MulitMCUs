/**
  ******************************************************************************
  * @file    HW_Uart.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.09
  * @brief   UART drivers
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Uart.h"
#include "HW_IRQHandler.h"
#include "HW_Gpio.h"

#include "chip.h"
#include "ring_buffer.h"
#include <string.h>
#include <stdlib.h>

#ifndef SL_ASSERT
#define SL_ASSERT(x)
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//中断回调函数
static void HW_UART0_RX_IRQHandler(void);
static void HW_UART1_RX_IRQHandler(void);

static void HW_UART0_DMATC_IRQHandler(void);
static void HW_UART1_DMATC_IRQHandler(void);

/* ---串口分频系数计算宏--- */
#define APB2_FCLK (48*1000000)
#define USARTx_GET_BRR(Fpclk,BaudRate)  \
    (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
    ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))


static USART_TypeDef * const USART[HW_UART_NODE_NUM] = {USART1,USART2};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[HW_UART_NODE_NUM] = {DMA1_Channel3, DMA1_Channel5};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[HW_UART_NODE_NUM] = {DMA1_Channel2, DMA1_Channel4};
static const IRQn_Type USART_IRQn[HW_UART_NODE_NUM] = {USART1_IRQn, USART2_IRQn};
static const IRQn_Type USART_DMA_IRQn[HW_UART_NODE_NUM] = {DMA1_Channel2_3_IRQn, DMA1_Channel4_5_IRQn};


/* ---定义接收缓冲区--- */
#define RBUF_SIZE   (64)    /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};

//环形缓冲区定义
#define UART_RB_SIZE    (64)
static RINGBUFF_T m_RxRing[HW_UART_NODE_NUM] = {0};
static uint8_t    m_uRxBuff[HW_UART_NODE_NUM][UART_RB_SIZE] = {0};



/*****************************************************************************
 * UART IO 配置
 ****************************************************************************/

/**
  * @brief  UART_IO_MUX初始化(选定某组引脚作为UART的引脚).
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_IOConfig(uint8_t uUartNode)
{
    switch (uUartNode)
    {
    case HW_UART_NODE0:
        // TX: PA9
        HW_GPIO_ModeConfig(0, 9, HW_GPIO_AF_PP_H);
        HW_GPIO_SetAFMode(0, 9, 1);
        
        // RX: PA10
        HW_GPIO_ModeConfig(0, 10, HW_GPIO_AF_PP_H);
        HW_GPIO_SetAFMode(0, 10, 1);
        
        break;                      
    case HW_UART_NODE1:
        // TX: PA2
        HW_GPIO_ModeConfig(0, 2, HW_GPIO_AF_PP_H);
        HW_GPIO_SetAFMode(0, 2, 1);
        
        // RX: PA3
        HW_GPIO_ModeConfig(0, 3, HW_GPIO_AF_PP_H);
        HW_GPIO_SetAFMode(0, 3, 1);
        
        break;
    default : break;
    }
    
}


/*****************************************************************************
 * 串口功能配置(私有)
 ****************************************************************************/

/**
  * @brief  串口中断回调设置
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_ConfigItCallBack(uint8_t uUartNode)
{
    //设置中断回调
    switch (uUartNode)
    {
    case HW_UART_NODE0: 
        {
            HW_IRQ_SetTrgCallback(HW_UART0_RX_IRQHandler, STM32F0_IRQ_TRG_USART1);
            HW_IRQ_SetTrgCallback(HW_UART0_DMATC_IRQHandler, STM32F0_IRQ_TRG_DMA1_Channel2_3);
            break;
        }
    case HW_UART_NODE1: 
        {
            HW_IRQ_SetTrgCallback(HW_UART1_RX_IRQHandler, STM32F0_IRQ_TRG_USART2);
            HW_IRQ_SetTrgCallback(HW_UART1_DMATC_IRQHandler, STM32F0_IRQ_TRG_DMA1_Channel4_5);
            break;
        }
        
    default: break; //不应该出现在这里
    }
    
}


/**
  * @brief  串口模式配置
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_ModeConfig(uint8_t uUartNode, uint32_t ulBaudRate)
{
    //开时钟
    if (uUartNode == HW_UART_NODE0)
    {
        RCC->APB2ENR |=  (0X1<<14);
    }
    else 
    {
        RCC->APB1ENR |=  (0X1<<(16+uUartNode));
    }
    
    //配置工作模式
    USART[uUartNode]->CR1 &= ~(0X1<<12);  //8位字长
    USART[uUartNode]->CR1 &= ~(0X1<<10);  //不使用校验位
    USART[uUartNode]->CR1 |=  (0X1<<3);   //使能发送
    USART[uUartNode]->CR1 |=  (0X1<<2);   //使用接收
    USART[uUartNode]->CR2 &= ~(0X3<<12);  //1位停止位
    USART[uUartNode]->CR3 |=  (0X1<<7);   //DMA发送使能
    USART[uUartNode]->CR3 |=  (0X1<<6);   //DMA接收使能
    
    //配置中断
    USART[uUartNode]->CR1 &= ~(0X1<<6);   //关发送完成中断
    USART[uUartNode]->CR1 &= ~(0X1<<5);   //关接收中断
    USART[uUartNode]->CR1 |=  (0X1<<4);   //开空闲中断
    
    //清标志位
    USART[uUartNode]->ICR |=  (0X1<<6);
    USART[uUartNode]->ICR |=  (0X1<<5);
    USART[uUartNode]->ICR |=  (0X1<<4);
    
    NVIC_SetPriority(USART_IRQn[uUartNode], 2);
    NVIC_EnableIRQ(USART_IRQn[uUartNode]); //开内核中断
    
    //配置波特率
    USART[uUartNode]->BRR = USARTx_GET_BRR(APB2_FCLK, ulBaudRate);
    
    //开串口
    USART[uUartNode]->CR1 |=  (0x1<<0);
    
}


/**
  * @brief  串口DMA接收功能初始化
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_DmaRxInit(uint8_t uUartNode)
{
    const uint8_t * const arBuffAddr[HW_UART_NODE_NUM] = {Uart1_RecvBuff, Uart2_RecvBuff};
    
    //开时钟
    RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
    
    //配置工作模式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<14);  //非内存-内存模式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X3<<12);
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X1<<12);  //中等优先级
    
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X3<<10);
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X0<<10);  //内存:8位数据帧格式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X3<<8);
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X0<<8);   //外设:8位数据帧格式
    
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X1<<7);   //内存增量模式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<6);   //外设非增量模式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<5);   //非循环模式
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<4);   //从外设中读
    
    //配置外设中断
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<3);   //关闭错误中断
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<2);   //关闭半传输中断
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<1);   //关闭传输完成中断
    
    //配置外设地址
    USART_RxDmaChannel[uUartNode]->CPAR = (uint32_t)&(USART[uUartNode]->RDR);
    
    //配置内存地址
    USART_RxDmaChannel[uUartNode]->CMAR = (uint32_t)arBuffAddr[uUartNode];
    
    //配置传输数量
    USART_RxDmaChannel[uUartNode]->CNDTR = RBUF_SIZE;
    
    //开启通道
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X1<<0);   //关闭通道,在开启接受/发送时再启动
    
}


/**
  * @brief  串口DMA发送功能初始化
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_DmaTxInit(uint8_t uUartNode)
{
    //开时钟
    RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
    
    //配置工作模式
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<14);  //非内存-内存模式
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X3<<12);
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X1<<12);  //中等优先级
    
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X3<<10);
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X0<<10);  //内存:8位数据帧格式
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X3<<8);
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X0<<8);   //外设:8位数据帧格式
    
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X1<<7);   //内存增量模式
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<6);   //外设非增量模式
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<5);   //非循环模式
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X1<<4);   //从内存中读
    
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<3);   //关闭错误中断
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<2);   //关闭半传输中断
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X1<<1);   //开传输完成中断
    
    NVIC_SetPriority(USART_DMA_IRQn[uUartNode], 2);
    NVIC_EnableIRQ(USART_DMA_IRQn[uUartNode]); //开内核中断
    
    //配置外设地址
    USART_TxDmaChannel[uUartNode]->CPAR = (uint32_t)&(USART[uUartNode]->TDR);
    
    //关闭通道
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<0);   //关闭通道,在开启接受/发送时再启动
    
}


//UART1_Rx DMA通道复位
void HW_UART_RxDMAReset(uint8_t uUartNode)
{
    //关闭通道
    USART_RxDmaChannel[uUartNode]->CCR &= ~(0X1<<0);
    
    //配置发送数量
    USART_RxDmaChannel[uUartNode]->CNDTR = RBUF_SIZE;
    
    //开启通道
    USART_RxDmaChannel[uUartNode]->CCR |=  (0X1<<0);
    
}


/*****************************************************************************
 * UART 基本功能接口
 ****************************************************************************/

/**
  * @brief  串口初始化
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @retval None
  * Note    当调用初始化函数后,无需再调用使能函数去使能串口
  */
void HW_UART_Init(uint8_t uUartNode, uint32_t ulBaudRate)
{
    //判断入参
    if (uUartNode >= HW_UART_NODE_NUM)
    {
        return;
    }
    
    //设置中断回调
    HW_UART_ConfigItCallBack(uUartNode);
    
    //初始化环形FIFO
    RingBuffer_Init(&m_RxRing[uUartNode], m_uRxBuff[uUartNode], 1, UART_RB_SIZE);
    
    //配置引脚
    HW_UART_IOConfig(uUartNode);
    
    //配置工作模式
    HW_UART_ModeConfig(uUartNode, ulBaudRate);
    
    //配置DMA
    HW_UART_DmaRxInit(uUartNode);
    HW_UART_DmaTxInit(uUartNode);
    
}


/**
  * @brief  串口使能
  * @param  uUartNode 串口节点号
  * @param  bIsEnable 使能标志
  * @retval None
  */
void HW_UART_Enable(uint8_t uUartNode, bool bIsEnable)
{
    if (bIsEnable)
    {
        USART[uUartNode]->CR1 |=   (0x1<<0);
    }
    else 
    {
        USART[uUartNode]->CR1 &=  ~(0x1<<0);
    }
   
}


/**
  * @brief  串口发送状态获取
  * @param  uUartNode 串口节点号
  * @retval 0-发送中 非0-发送完成
  */
uint32_t HW_UART_GetTransStatus(uint8_t uUartNode)
{
    
    return (USART[uUartNode]->ISR & (0x1<<6)) != 0;
}


/**
  * @brief  单字节发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  uData 要发送的字节数据
  * @retval None
  */
void HW_UART_SendByte(uint8_t uUartNode, uint8_t uData)
{
    //等待发送完成(发送缓冲区空)
    while (!(USART[uUartNode]->ISR & (1<<6)));
    
    //将要发送的数据写入到发生缓冲区中
    USART[uUartNode]->TDR = uData;
    
}


/**
  * @brief  缓冲区发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的缓冲区
  * @param  ulSize 要发送的字节数
  * @retval None
  */
void HW_UART_SendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize)
{
    for (int i = 0; i < ulSize; i++)
    {
        HW_UART_SendByte(uUartNode, ((uint8_t *)pSBuff)[i]);
    }
}


/**
  * @brief  字符串发送函数(阻塞性发送)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HW_UART_SendStr(uint8_t uUartNode, uint8_t *pSBuff)
{
    uint32_t i = 0;
    
    while (pSBuff[i] != '\0')
    {
        i++;
    }

    HW_UART_SendBuff(uUartNode, pSBuff, i);
    
}


/**
  * @brief  串口接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  uUartNode 串口节点号
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uint32_t HW_UART_RecvBuff(uint8_t uUartNode, void *pRBuff, uint32_t ulSize)
{
    
    return RingBuffer_PopMult(&m_RxRing[uUartNode], pRBuff, ulSize);
}


/**
  * @brief  数据接收处理(非阻塞接收线程)
  * @param  uUartNode 串口节点号
  * @retval None
  * @retval 本函数是应用于非中断接收的情况,单独一个线程做数据接收
  */
void HW_UART_RecvHandler(uint8_t uUartNode)
{
    //假如接收到数据,则将其存储入环形缓冲区之中
    if (USART[uUartNode]->ISR & (0x1<<5))
    {
        uint8_t uData = USART[uUartNode]->RDR;
        RingBuffer_Insert(&m_RxRing[uUartNode], &uData);
    }
    
}


/*****************************************************************************
 * DMA链表传输接口
 ****************************************************************************/

//链表结构
typedef struct Usart_Tx_Node
{
      struct Usart_Tx_Node *pNext;  //下一个节点的地址(若为节点尾,则定义为NULL)
      uint32_t  ulLen;              //数据长度
      uint8_t   uBuff[];            //空数组
}UART_TX_NODE;


//链表首指针
static UART_TX_NODE *g_HeadNodePtr0 = NULL; //第0条链表的首节点指针
static UART_TX_NODE *g_HeadNodePtr1 = NULL; //第1条链表的首节点指针

/**
  * @brief  节点初始化
  * @param  pNode 要初始化的节点
  * @retval None
  */
static void List_Init(UART_TX_NODE *pNode)
{
    pNode->pNext = pNode;
  
}


/**
  * @brief  节点尾部嵌入
  * @param  pHeadNode 首节点地址
  * @param  pTailNode 要嵌入的尾节点
  * @retval None
  */
static void List_SinkTail(UART_TX_NODE *pHeadNode, UART_TX_NODE *pTailNode)
{
    SL_ASSERT(pHeadNode);
    SL_ASSERT(pTailNode);

    //指针偏移到链表尾部
    while (pHeadNode->pNext != pHeadNode)
    {
        pHeadNode = pHeadNode->pNext;
    }

    //嵌入尾部
    pHeadNode->pNext = pTailNode;
  
}


/**
  * @brief  节点释放
  * @param  pHeadNode 要释放的首节点地址
  * @retval 最新的首节点地址
  */
static UART_TX_NODE *List_FreeHead(UART_TX_NODE *pHeadNode)
{
    UART_TX_NODE *tmpNode = NULL;

    SL_ASSERT(pHeadNode);

    //假如链表并非只有单个节点,则返回最新的首节点的地址
    if (pHeadNode->pNext != pHeadNode)
    {
        tmpNode = pHeadNode->pNext;

    }
    memset(pHeadNode, 0, sizeof(UART_TX_NODE));
    free(pHeadNode);

    return tmpNode;
}


/**
  * @brief  首节点指针获取
  * @param  uUartNode 串口节点号
  * @retval None
  */
static UART_TX_NODE **List_GetHead(uint8_t uUartNode)
{
    UART_TX_NODE **pHeadNode = NULL;

    switch (uUartNode)
    {
    case HW_UART_NODE0: pHeadNode = &g_HeadNodePtr0; break;
    case HW_UART_NODE1: pHeadNode = &g_HeadNodePtr1; break;
    default: break;
    }

    return pHeadNode;
}


/**
  * @brief  数据DMA发送
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
static void HW_UART_DmaSend(uint8_t uUartNode, uint8_t *pSBuff, uint32_t ulSize)
{
    //等待串口上轮发送完成
    while (!(USART[uUartNode]->ISR & (1<<6)));
    
    //关闭通道
    USART_TxDmaChannel[uUartNode]->CCR &= ~(0X1<<0);
    
    //配置内存地址
    USART_TxDmaChannel[uUartNode]->CMAR = (uint32_t)pSBuff;
    
    //配置发送数量
    USART_TxDmaChannel[uUartNode]->CNDTR = ulSize;
    
    //启动传输
    USART_TxDmaChannel[uUartNode]->CCR |=  (0X1<<0);
    
    
}


/**
  * @brief  当前链表首节点数据发送
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_SendHeadNodeBuff(uint8_t uUartNode)
{
    //获取节点指针
    UART_TX_NODE **pHeadNode = List_GetHead(uUartNode);
    SL_ASSERT(pHeadNode);
    SL_ASSERT(*pHeadNode);

    //发送节点中的数据
    HW_UART_DmaSend(uUartNode, (*pHeadNode)->uBuff, (*pHeadNode)->ulLen);
  
}


/**
  * @brief  串口数据DMA发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void HW_UART_DmaSendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize)
{
    UART_TX_NODE *pTxNode = NULL;
    UART_TX_NODE **pHeadNode = List_GetHead(uUartNode);

    //判断参数
    SL_ASSERT(ulSize);

    //分配空间(若分配不成功,则重复申请,以保证数据能发送出去)
    //如果要传输的数量量太大而导致内存空间不够,会阻塞在这里.由于此时内存空间是一边申请一边释放,
    //所以在超大数据量传输的情况中,超限数据的阻塞时间==超限数据的串口传输时间(依据波特率)
    //如果要增强其数据容量,需要增大其链表可以申请的内存空间的容量,即堆的容量
    while ((pTxNode = (UART_TX_NODE *)malloc(sizeof(UART_TX_NODE) + ulSize*sizeof(unsigned char))) == NULL);

    //初始化链表
    List_Init(pTxNode);

    //将堆上的数据存储到节点空间中
    memcpy(pTxNode->uBuff, pSBuff, ulSize);
    pTxNode->ulLen = ulSize;

    //嵌入链表
    if (*pHeadNode != NULL)  //假如当前链表非空(发送中)
    {
        //将节点嵌入链表
        List_SinkTail(*pHeadNode, pTxNode);
    }
    else //假如当前链表为空(串口空闲),启动传输
    {
        *pHeadNode = pTxNode; //将当前节点设置为链表头
        HW_UART_SendHeadNodeBuff(uUartNode);  //发送链表头节点的数据
    }

}


/**
  * @brief  串口字符串DMA发送函数(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HW_UART_DmaSendStr(uint8_t uUartNode, uint8_t *pSBuff)
{
    uint32_t i = 0;
    
    while (pSBuff[i] != '\0') i++;

    HW_UART_DmaSendBuff(uUartNode, pSBuff, i);
    
}



/*****************************************************************************
 * 中断回调函数
 ****************************************************************************/

/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART0_RX_IRQHandler(void)
{
    uint16_t nUart1RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度
    
    //中断入口判断 */
    if (USART1->ISR & (0X1<<4))  //空闲帧中断入口
    {
        //清标志位 */
        USART1->ICR |= (0X1<<4);
        
        //计算接收到的帧长 */
        nUart1RecvCnt = RBUF_SIZE - DMA1_Channel3->CNDTR;
        
        //复位DMA */
        HW_UART_RxDMAReset(HW_UART_NODE0);
        
        //将数据存储入缓冲区
        RingBuffer_InsertMult(&m_RxRing[0], Uart1_RecvBuff, nUart1RecvCnt);
        
    }
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART1_RX_IRQHandler(void)
{
    uint16_t nUart2RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度
    
    //中断入口判断
    if (USART2->ISR & (0X1<<4))  //空闲帧中断入口
    {
        //清标志位
        USART2->ICR |= (0X1<<4);
        
        //计算接收到的帧长
        nUart2RecvCnt = RBUF_SIZE - DMA1_Channel5->CNDTR;
        
        //复位DMA
        HW_UART_RxDMAReset(HW_UART_NODE1);
        
        //将数据存储入缓冲区
        RingBuffer_InsertMult(&m_RxRing[1], Uart2_RecvBuff, nUart2RecvCnt);
        
    }
    
}



/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART0_DMATC_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<5))
    {
        DMA1->IFCR |= (0x1<<4);  //清全局标志位
        
        g_HeadNodePtr0 = List_FreeHead(g_HeadNodePtr0);
        
        if (g_HeadNodePtr0 != NULL)  //若当前的首节点非空
        {
            HW_UART_SendHeadNodeBuff(HW_UART_NODE0);
        }
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART1_DMATC_IRQHandler(void)
{
    if (DMA1->ISR & (0x1<<13))
    {
        DMA1->IFCR |= (0x1<<12);  //清全局标志位
        
        g_HeadNodePtr1 = List_FreeHead(g_HeadNodePtr1);
        
        if (g_HeadNodePtr1 != NULL)  //若当前的首节点非空
        {
            HW_UART_SendHeadNodeBuff(HW_UART_NODE1);
        }
    }
    
}

