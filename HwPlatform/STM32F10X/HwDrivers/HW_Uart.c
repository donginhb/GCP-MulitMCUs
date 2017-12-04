/**
  ******************************************************************************
  * @file    HW_Uart.c
  * @author  Duhanfneg
  * @version V4.2
  * @date    2017.11.17
  * @brief   UART drivers
  ******************************************************************************
  * @attention
  * 
  * USART1: TX --- PA9  复用推挽,50M
  *         RX --- PA10 上拉输入
  *
  * USART2: TX --- PA2  复用推挽,50M
  *         RX --- PA3  上拉输入
  *
  * USART3: TX --- PB10 复用推挽,50M
  *         RX --- PB11 上拉输入
  *
  * USART4: TX --- PC10 复用推挽,50M
  *         RX --- PC11 上拉输入
  *   
  *   
  * V1.1------------
  * 修改描述: 修复波特率配置不准确的错误
  * 错误原因: 波特率分频因子计算公式(宏)中,默认定义为整型,导致小数部分丢失
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2015.12.13
  * 
  * V1.2------------
  * 修改描述: 增加单独的接收中断开启与关闭接口函数
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.2
  * 修改日期: 2015.12.21
  * 
  * V1.3------------
  * 修改描述: 修改文件架构,加入UART+DMA功能,修改H文件描述,增加文件注释头
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.3
  * 修改日期: 2016.02.18
  * 
  * V1.4------------
  * 修改描述: 1.修复在初始化阶段Tx脚产生一个负脉冲的错误(此负脉冲会被识别为一个字符)
  *           2.将串口接收修改为空闲中断模式
  * 错误原因: 在配置IO口为复用模式后,由于此时UARTx的时钟尚未开启,所以电平被拉低.
  *           紧接着,开启UARTx的时钟并且使能发送功能后,IO通电,恢复高电平,故而整体为
  *           一个负脉冲,并可被识别为一个字符(0X80,0XFF,0XFE等).
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.4
  * 修改日期: 2016.02.20
  *
  * V1.5------------
  * 修改描述: 1.增加了地址标志模式,方便用于多处理器模式下的硬件识别(一般是从机用)
  *           2.删除部分不必要的函数,精简架构
  * 地址标记: 可为本机配置一个本机地址(4位),并将接收器进入静默状态.此时,接收器会将最
  *           高位为'1'的数据识别为"地址",最高位为'0'的识别为数据.当接收器接收到一个
  *           "地址"时,会对此"地址"进行判断:低4位是否与"本机地址匹配",若匹配,则接收接
  *           下来的"数据",直到下一次"地址"的到来,若匹配,则继续接收,若不匹配,则
  *           重新进入静默状态.
  *           地址标记模式的使用在多处理器通信且大数据吞吐中,能利用硬件的筛选减少CPU
  *           的运行. 详见RM0008 P805  Multiprocessor communication
  * 已知缺陷: 1.地址标记模式中,当接收数据完成后,空闲帧的到来并不会触发"空闲帧"中断,这
  *             样会使UART的可用性降低
  *           2.地址标记模式中,由于是对最高位进行对数据的分类(地址/数据),导致最高位的
  *             使用受到了限制.
  * 避免方法: 开启"接收缓冲区非空中断","地址"字符匹配之后会触发此中断,在此中断内暂时关
  *           闭接收器静默状态,在触发空闲帧中断后再开启静默状态.
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.5
  * 修改日期: 2016.03.15
  *
  * V2.0------------
  * 修改描述: 1.移除地址标志模式
  *           2.发现一个关于DMA发送的BUG
  * 地址标志: 地址标志模式适用于同一条串口总线中一主多从的情况,使用上较为繁琐,不
  *           适用于常规的使用场合,而且地址标志模式会加大驱动的复杂程度,已知的缺
  *           陷较难避免,目前没有找到比较简单方便的使用方法,故而移除地址标志模式.
  * DMA 发送: 利用DMA发送一组数据,当触发"DMA传输完成(TCIF)"条件后关闭串口发送功能,
  *           使用示波器发现串口只发送了一部分的数据.根据 STM32参考手册(Rev 16)
  *           818页 Figure 296 可知,DMA搬运完成即触发"TCIF"标志,而此时串口尚未发
  *           送完成,关闭串口会对尚在发送的数据造成截断.若要解决此BUG,可以把DMA发
  *           送的完成标志判断由"DMA_TCIF"改成"USART_TC".
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.05.16
  *
  * V3.0------------
  * 修改描述: 1.将串口1~4整合到同一个文件中,同一个函数可以被各个串口进行调用,通
  *             过结构体指针数组的方式映射到每一个寄存器组之中,下标访问的实现机
  *             制避免了冗余的条件判断.
  *           2.兼容STM32F10x全系列的芯片,加入安全机制,避免因不同芯片的差异性或
  *             错误调用是对非法地址的访问.
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2016.08.09
  * 
  * V3.1------------
  * 修改描述: 修改DMA发送机制
  * 修改原因: 假如外部函数调用此DMA发送函数,入参的缓冲区是栈上的局部变量,那么在
  *           外部函数结束后栈上的空间被回收(此时USART还在发送中).如果此时在别的
  *           函数中修改了栈上的数据,串口的发送便无效.故而修改其发送机制,在发送
  *           函数中,将栈上临时存储的数据复制到堆上的空间中,用链表的方式链接多个
  *           发送缓冲区,当DMA发送完成后(DMA发送完成中断),释放其申请的空间.只要
  *           堆上可用的空间足够,发送几十K的数据,CPU的占用也不超过2MS
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.1
  * 修改日期: 2016.11.18
  * 
  * V4.1------------
  * 修改描述: 1.修改串口配置机制,将其由寄存器指针数组的索引改成直接的串口指针入参
  *           2.调用经过一层封装的DMA函数,而非直接对DMA寄存器进行操作
  *           3.不再集成串口空闲帧中断的回调,而是交由外部去调用,最大程度上避免
  *             对此文件的修改
  *           4.链表管理函数重写,尾节点的next不再指向NULL,而是指向自身,避免在错
  *             误地对NULL进行操作时产生HardFault
  *           5.在函数多处加入断言,便于发现潜在的BUG
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  *   
  *   
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Uart.h"
#include "HW_IRQHandler.h"
#include "HW_Gpio.h"
#include "HW_SysCtrl.h"
      
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
static void HW_UART2_RX_IRQHandler(void);
static void HW_UART3_RX_IRQHandler(void);

static void HW_UART0_DMATC_IRQHandler(void);
static void HW_UART1_DMATC_IRQHandler(void);
static void HW_UART2_DMATC_IRQHandler(void);
static void HW_UART3_DMATC_IRQHandler(void);


/* ---串口分频系数计算宏--- */
#define USARTx_GET_BRR(Fpclk, BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))

static USART_TypeDef * const USART[HW_UART_NODE_NUM] = {USART1,USART2, USART3, UART4};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[HW_UART_NODE_NUM] = {DMA1_Channel5, DMA1_Channel6, DMA1_Channel3, DMA2_Channel3};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[HW_UART_NODE_NUM] = {DMA1_Channel4, DMA1_Channel7, DMA1_Channel2, DMA2_Channel5};
static const IRQn_Type USART_IRQn[HW_UART_NODE_NUM] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn};
static const IRQn_Type USART_TX_DMA_IRQn[HW_UART_NODE_NUM] = {DMA1_Channel4_IRQn, DMA1_Channel7_IRQn, DMA1_Channel2_IRQn, DMA2_Channel4_5_IRQn};


/* ---定义接收缓冲区--- */
#define RBUF_SIZE   64      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
uint8_t Uart0_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart3_RecvBuff[RBUF_SIZE] = {0};

//环形缓冲区定义
#define UART_RB_SIZE    (128)
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
        HW_GPIO_ModeConfig(0, 9,  HW_GPIO_AF_PP_H);   // TX: PA9
        HW_GPIO_ModeConfig(0, 10, HW_GPIO_PUSH_UP);   // RX: PA10
        
        break;                      
    case HW_UART_NODE1:
        HW_GPIO_ModeConfig(0, 2, HW_GPIO_AF_PP_H);    // TX: PA2
        HW_GPIO_ModeConfig(0, 3, HW_GPIO_PUSH_UP);    // RX: PA3
        
    case HW_UART_NODE2:
        HW_GPIO_ModeConfig(1, 10, HW_GPIO_AF_PP_H);   // TX: PB10
        HW_GPIO_ModeConfig(1, 11, HW_GPIO_PUSH_UP);   // RX: PB11
        
    case HW_UART_NODE3:
        HW_GPIO_ModeConfig(2, 10, HW_GPIO_AF_PP_H);   // TX: PC10
        HW_GPIO_ModeConfig(2, 11, HW_GPIO_PUSH_UP);   // RX: PC11
        
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
            HW_IRQ_SetTrgCallback(HW_UART0_RX_IRQHandler, STM32F1_IRQ_TRG_USART1);
            HW_IRQ_SetTrgCallback(HW_UART0_DMATC_IRQHandler, STM32F1_IRQ_TRG_DMA1_Channel4);
            break;
        }
    case HW_UART_NODE1: 
        {
            HW_IRQ_SetTrgCallback(HW_UART1_RX_IRQHandler, STM32F1_IRQ_TRG_USART2);
            HW_IRQ_SetTrgCallback(HW_UART1_DMATC_IRQHandler, STM32F1_IRQ_TRG_DMA1_Channel7);
            break;
        }
    case HW_UART_NODE2: 
        {
            HW_IRQ_SetTrgCallback(HW_UART2_RX_IRQHandler, STM32F1_IRQ_TRG_USART3);
            HW_IRQ_SetTrgCallback(HW_UART2_DMATC_IRQHandler, STM32F1_IRQ_TRG_DMA1_Channel2);
            break;
        }
    case HW_UART_NODE3: 
        {
            HW_IRQ_SetTrgCallback(HW_UART3_RX_IRQHandler, STM32F1_IRQ_TRG_UART4);
            HW_IRQ_SetTrgCallback(HW_UART3_DMATC_IRQHandler, STM32F1_IRQ_TRG_DMA2_Channel5);
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
    switch (uUartNode)
    {
    case HW_UART_NODE0: RCC->APB2ENR |= RCC_APB2ENR_USART1EN; break;
    case HW_UART_NODE1: RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
    case HW_UART_NODE2: RCC->APB1ENR |= RCC_APB1ENR_USART3EN; break;
    case HW_UART_NODE3: RCC->APB1ENR |= RCC_APB1ENR_UART4EN;  break;
    }
    
    //配置工作模式
    USART[uUartNode]->CR1 &= ~USART_CR1_M;      //8位字长
    USART[uUartNode]->CR1 &= ~USART_CR1_PCE;    //不使用校验位
    USART[uUartNode]->CR1 |=  USART_CR1_TE;     //使能发送
    USART[uUartNode]->CR1 |=  USART_CR1_RE;     //使用接收
    USART[uUartNode]->CR2 &= ~USART_CR2_STOP;   //1位停止位
    USART[uUartNode]->CR3 |=  USART_CR3_DMAT;   //DMA发送使能
    USART[uUartNode]->CR3 |=  USART_CR3_DMAR;   //DMA接收使能
    
    //配置中断
    USART[uUartNode]->CR1 &= ~USART_CR1_TCIE;   //关发送完成中断
    USART[uUartNode]->CR1 &= ~USART_CR1_RXNEIE; //关接收中断
    USART[uUartNode]->CR1 |=  USART_CR1_IDLEIE; //开空闲中断
    
    HW_NVIC_Enable(USART_IRQn[uUartNode], 2, 2);    //开内核中断
    
    //配置波特率
    USART[uUartNode]->BRR = USARTx_GET_BRR((uUartNode == HW_UART_NODE0)? APB2_FCLK : APB1_FCLK, ulBaudRate);
    
    //开串口
    USART[uUartNode]->CR1 |=  USART_CR1_UE;
    
}


/**
  * @brief  串口DMA接收功能初始化
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HW_UART_DmaRxInit(uint8_t uUartNode)
{
    const uint8_t * const arBuffAddr[4] = { Uart0_RecvBuff,Uart1_RecvBuff, Uart2_RecvBuff,Uart3_RecvBuff };
    
    //开时钟
    if (uUartNode != HW_UART_NODE3)
    {
        RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
    }
    else
    {
        RCC->AHBENR |= (0X1<<1);  //DMA2时钟使能
    }
    
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
    USART_RxDmaChannel[uUartNode]->CPAR = (uint32_t)&(USART[uUartNode]->DR);
    
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
    if (uUartNode != HW_UART_NODE3)
    {
        RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
    }
    else
    {
        RCC->AHBENR |= (0X1<<1);  //DMA2时钟使能
    }
    
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
    
    HW_NVIC_Enable(USART_TX_DMA_IRQn[uUartNode], 2, 2); //开内核中断
    
    //配置外设地址 
    USART_TxDmaChannel[uUartNode]->CPAR = (uint32_t)&(USART[uUartNode]->DR);
    
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
    
    return (USART[uUartNode]->SR & (0x1<<6)) != 0;
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
    while (!(USART[uUartNode]->SR & (1<<6)));
    
    //将要发送的数据写入到发生缓冲区中
    USART[uUartNode]->DR = uData;
    
    
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
    if (USART[uUartNode]->SR & (0x1<<5))
    {
        uint8_t uData = USART[uUartNode]->DR;
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
static UART_TX_NODE *g_HeadNodePtr2 = NULL; //第1条链表的首节点指针
static UART_TX_NODE *g_HeadNodePtr3 = NULL; //第1条链表的首节点指针

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
    case HW_UART_NODE2: pHeadNode = &g_HeadNodePtr2; break;
    case HW_UART_NODE3: pHeadNode = &g_HeadNodePtr3; break;
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
    while (!(USART[uUartNode]->SR & (1<<6)));
    
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
    
    //参数判断
    if (pHeadNode == NULL)
    {
        return;
    }

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
    if (USART[HW_UART_NODE0]->SR & (0x1<<4))  //空闲帧中断入口
    {
        //清标志位
        USART[HW_UART_NODE0]->DR;      //总线空闲中断标志位由软件序列清除:先读SR,再读DR
        
        //计算接收到的帧长
        uint16_t nRecvCnt = RBUF_SIZE - USART_RxDmaChannel[HW_UART_NODE0]->CNDTR;
        
        //复位DMA
        HW_UART_RxDMAReset(HW_UART_NODE0);
        
        //数据帧处理
        RingBuffer_InsertMult(&m_RxRing[HW_UART_NODE0], Uart0_RecvBuff, nRecvCnt);
    }
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART1_RX_IRQHandler(void)
{
    if (USART[HW_UART_NODE1]->SR & (0x1<<4))  //空闲帧中断入口
    {
        //清标志位
        USART[HW_UART_NODE1]->DR;      //总线空闲中断标志位由软件序列清除:先读SR,再读DR
        
        //计算接收到的帧长
        uint16_t nRecvCnt = RBUF_SIZE - USART_RxDmaChannel[HW_UART_NODE1]->CNDTR;
        
        //复位DMA
        HW_UART_RxDMAReset(HW_UART_NODE1);
        
        //数据帧处理
        RingBuffer_InsertMult(&m_RxRing[HW_UART_NODE1], Uart1_RecvBuff, nRecvCnt);
    }
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART2_RX_IRQHandler(void)
{
    if (USART[HW_UART_NODE2]->SR & (0x1<<4))  //空闲帧中断入口
    {
        //清标志位
        USART[HW_UART_NODE2]->DR;      //总线空闲中断标志位由软件序列清除:先读SR,再读DR
        
        //计算接收到的帧长
        uint16_t nRecvCnt = RBUF_SIZE - USART_RxDmaChannel[HW_UART_NODE2]->CNDTR;
        
        //复位DMA
        HW_UART_RxDMAReset(HW_UART_NODE2);
        
        //数据帧处理
        RingBuffer_InsertMult(&m_RxRing[HW_UART_NODE2], Uart2_RecvBuff, nRecvCnt);
    }
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART3_RX_IRQHandler(void)
{
    if (USART[HW_UART_NODE3]->SR & (0x1<<4))  //空闲帧中断入口
    {
        //清标志位
        USART[HW_UART_NODE3]->DR;      //总线空闲中断标志位由软件序列清除:先读SR,再读DR
        
        //计算接收到的帧长
        uint16_t nRecvCnt = RBUF_SIZE - USART_RxDmaChannel[HW_UART_NODE3]->CNDTR;
        
        //复位DMA
        HW_UART_RxDMAReset(HW_UART_NODE3);
        
        //数据帧处理
        RingBuffer_InsertMult(&m_RxRing[HW_UART_NODE3], Uart3_RecvBuff, nRecvCnt);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART0_DMATC_IRQHandler(void)
{
    g_HeadNodePtr0 = List_FreeHead(g_HeadNodePtr0);
    
    if (g_HeadNodePtr0 != NULL)  //若当前的首节点非空
    {
        HW_UART_SendHeadNodeBuff(HW_UART_NODE0);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART1_DMATC_IRQHandler(void)
{
    g_HeadNodePtr1 = List_FreeHead(g_HeadNodePtr1);
    
    if (g_HeadNodePtr1 != NULL)  //若当前的首节点非空
    {
        HW_UART_SendHeadNodeBuff(HW_UART_NODE1);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART2_DMATC_IRQHandler(void)
{
    g_HeadNodePtr2 = List_FreeHead(g_HeadNodePtr2);
    
    if (g_HeadNodePtr2 != NULL)  //若当前的首节点非空
    {
        HW_UART_SendHeadNodeBuff(HW_UART_NODE2);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HW_UART3_DMATC_IRQHandler(void)
{
    g_HeadNodePtr3 = List_FreeHead(g_HeadNodePtr3);
    
    if (g_HeadNodePtr3 != NULL)  //若当前的首节点非空
    {
        HW_UART_SendHeadNodeBuff(HW_UART_NODE3);
    }
    
}
