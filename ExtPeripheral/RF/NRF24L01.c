/**
  ******************************************************************************
  * @file    sl_nrf24l01.c
  * @author  Duhanfeng
  * @version V1.1
  * @date    2017.04.09
  * @brief   control nrf24l01
  ******************************************************************************
  * @attention
  * 
  * 两个nrf24l01通信，需要满足3个条件相同：
  * 1.频道相同（设置频道寄存器RF_CH）
  * 2.地址相同（设置TX_ADDR和RX_ADDR_P0相同）
  * 3.每次发送接收的字节数相同（如果设置了通道的有效数据宽度为n，那么每次发送的字节数也必须为n，当然，n<=32）
  * 
  * 当触发IRQ信号,电平变化是下降沿;
  * 
  * V1.1------------    
  * 修改描述: 优化程序接口
  * 修改作者: Duhanfeng
  * 当前版本: V1.1
  * 修改日期: 2018.01.24
  *     
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "NRF24L01.h"
#include "NRF24L01_RegDef.h"
#include <string.h>
#include "Algorithm/RingBuff/RingBuff.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//外界控制接口
static uBit8   (*SPI_NRF_RW)(uBit8 cWriteData);
static void    (*NRF_CSN)(bool State);
static void    (*NRF_CE)(bool State);
static bool    (*NRF_IRQ)(void);

//通信地址定义
static uBit8 m_uNrfTxAddr[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};   //定义一个静态发送地址
static uBit8 m_uNrfRxAddr[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

//环形缓冲区定义
#define NRF_RB_SIZE    (128)
static RINGBUFF_T m_RxRing = {0};
static uBit8    m_uRxBuff[NRF_RB_SIZE] = {0};


/**
  * @brief  简单的微秒级延时函数
  * @param  ulDelay 延时参数,单位为微秒
  * @retval None
  */
static void nRF24L01_SimpleDelayUs(uBit32 ulDelay)
{
    for(volatile int i = 0; i < ulDelay*15; i++);
    
}


/**
  * @brief 用于向NRF特定的寄存器写入数据
  * @param uReg:NRF的命令+寄存器地址
  * @param uData:将要向寄存器写入的数据
  * @retval  NRF的status寄存器的状态
  */
static uBit8 nRF24L01_WriteReg(uBit8 uReg, uBit8 uData)
{
    uBit8 uStatus = 0;
    NRF_CE(0);
    
    //置低CSN，使能SPI传输
    NRF_CSN(0);
    
    //发送命令及寄存器号 
    uStatus = SPI_NRF_RW(uReg);
    
    //向寄存器写入数据
    SPI_NRF_RW(uData); 
    
    //CSN拉高，完成     
    NRF_CSN(1);  
    
    //返回状态寄存器的值
    return(uStatus);
}



/**
  * @brief 用于从NRF特定的寄存器读出数据
  * @param uReg:NRF的命令+寄存器地址
  * @retval  寄存器中的数据
  */
static uBit8 nRF24L01_ReadReg(uBit8 uReg)
{
    uBit8 uRegValue = 0;
    
    NRF_CE(0);
    
    //置低CSN，使能SPI传输
    NRF_CSN(0);
    
    //发送寄存器号
    SPI_NRF_RW(uReg); 
    
    //读取寄存器的值 
    uRegValue = SPI_NRF_RW(NOP);
    
    //CSN拉高，完成
    NRF_CSN(1);    
    
    return uRegValue;
}



/**
  * @brief 用于向NRF的寄存器中写入一串数据
  * @param uReg:NRF的命令+寄存器地址
  * @param pBuf:用于存储将被读出的寄存器数据的数组，外部定义
  * @param ulBuffLenght:pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
static uBit8 nRF24L01_ReadBuf(uBit8 uReg,uBit8 *pBuf,uBit8 ulBuffLenght)
{
    uBit8 uStatus = 0;
    
    NRF_CE(0);
    
    //置低CSN，使能SPI传输
    NRF_CSN(0);
    
    //发送寄存器号    
    uStatus = SPI_NRF_RW(uReg); 
    
    //读取缓冲区数据
    for(int i = 0; i < ulBuffLenght; i++)
    {
        pBuf[i] = SPI_NRF_RW(NOP); //从NRF24L01读取数据  
    }
    
    //CSN拉高，完成
    NRF_CSN(1);  
    
    return uStatus;    //返回寄存器状态值
}


/**
  * @brief 用于向NRF的寄存器中写入一串数据
  * @param uReg: NRF的命令+寄存器地址
  * @param pBuf：存储了将要写入写寄存器数据的数组，外部定义
  * @param ulBuffLenght: pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
static uBit8 nRF24L01_WriteBuf(uBit8 uReg ,uBit8 *pBuf, uBit8 ulBuffLenght)
{
    uBit8 uStatus = 0;
    
    NRF_CE(0);
    
    //置低CSN，使能SPI传输
    NRF_CSN(0);      
    
    //发送寄存器号  
    uStatus = SPI_NRF_RW(uReg); 
    
    //向缓冲区写入数据
    for(int i = 0; i < ulBuffLenght; i++)
    {
        SPI_NRF_RW(*pBuf++);  //写数据到缓冲区 
    }
    
    //CSN拉高，完成
    NRF_CSN(1);      
    
    return (uStatus);  //返回NRF24L01的状态     
}


/*****************************************************************************
 * NRF相关控制接口
 ****************************************************************************/

/**
  * @brief  NRF24L01 接口初始化
  * @param  SPI_ReadWriteByte SPI控制接口
  * @param  SetCSN  CSN引脚控制接口
  * @param  SetCE  CE引脚控制接口
  * @param  GetIRQ  IRQ引脚控制接口
  * @retval 0-成功 非0-失败
  */
uBit32 nRF24L01_InitInterFaces(uBit8 (*SPI_ReadWriteByte)(uBit8 cWriteData),
                               void (*SetCSN)(bool bState),
                               void (*SetCE)(bool bState),
                               bool (*GetIRQ)(void))
{
    do 
    {
        //参数校验
        if (SPI_ReadWriteByte == NULL)  break;
        if (SetCSN == NULL)  break;
        if (SetCE == NULL)  break;
        if (GetIRQ == NULL) break;
        
        SPI_NRF_RW = SPI_ReadWriteByte;
        NRF_CSN = SetCSN;
        NRF_CE  = SetCE;
        NRF_IRQ = GetIRQ;
        
        //初始化环形FIFO
        RingBuff_Init(&m_RxRing, m_uRxBuff, 1, NRF_RB_SIZE);
        
        return 0;
        
    }while(0);
    
    return 1;
}


/**
  * @brief  接收模式进入
  * @param  None
  * @retval None
  */
void nRF24L01_EnterRxMode(void)
{
    uBit8 uStatus = 0; 
    
    NRF_CE(0);
    
    uStatus=nRF24L01_ReadReg(STATUS);
    nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,uStatus);                            //清除中断标志    
    
    nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, m_uNrfRxAddr, RX_ADR_WIDTH);  //写RX节点地址 
    nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);                               //使能通道0的自动应答 
    nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);                           //使能通道0的接收地址 
    nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);                             //设置RF通信频率 
    nRF24L01_WriteReg(NRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);                  //选择通道0的有效数据宽度       
    nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);                            //设置TX发射参数,0db增益,2Mbps,低噪声增益开启    
    nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);                              //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式  
    
    //nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);                           //失能通道0自动应答 
    //nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01);                       //接收要使能接收通道
    
    nRF24L01_WriteReg(FLUSH_RX,NOP);                                            //清除RX FIFO寄存器 
    nRF24L01_WriteReg(FLUSH_TX,NOP);                                            //清除TX FIFO寄存器 
    
    NRF_CE(1); 
    
}


/**
  * @brief  发送模式进入
  * @param  None
  * @retval None
  */
void nRF24L01_EnterTxMode(void)
{  
    uBit8 uStatus = 0; 
    NRF_CE(0);
    
    uStatus=nRF24L01_ReadReg(STATUS);
    nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,uStatus);                            //清除中断标志  
    
    nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, m_uNrfTxAddr, TX_ADR_WIDTH);     //写TX节点地址  
    nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, m_uNrfRxAddr, RX_ADR_WIDTH);  //设置TX节点地址,主要为了使能ACK    
    nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);                               //使能通道0的自动应答         
    nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);                           //使能通道0的接收地址     
    nRF24L01_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x15);                          //设置自动重发间隔时间:500us + 86us;最大自动重发次数:5次    
    nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);                             //设置RF通道为CHANAL  
    nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);                            //设置TX发射参数,0db增益,2Mbps,低噪声增益开启       
    nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);                              //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断    
    
    //nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);                           //失能通道0自动应答 
    //nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x00);                       //失能接收通道0       
    //nRF24L01_WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x00);                      //失能自动重发 
    
    nRF24L01_WriteReg(FLUSH_RX,NOP);                                            //清除RX FIFO寄存器 
    nRF24L01_WriteReg(FLUSH_TX,NOP);                                            //清除TX FIFO寄存器 
    
    NRF_CE(1);          
    nRF24L01_SimpleDelayUs(20);                                                 //CE要拉高一段时间才进入发送模式，时间大于10us 
}


/**
  * @brief  NRF连接检测
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 nRF24L01_CheckConnect(void)
{
    uBit8 uWriteBuff[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
    uBit8 uReadBuff[5] = {0};;
    
    //写入5个字节数据.    
    nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, uWriteBuff, sizeof(uWriteBuff));
    
    //读出写入的数据
    nRF24L01_ReadBuf(TX_ADDR,uReadBuff,5); 
    
    //比较数据
    for(int i = 0; i < sizeof(uWriteBuff); i++)
    {
        if(uReadBuff[i] != uWriteBuff[i])
        {
            return 1;   //失败
        }
    } 
    
    return 0;   //成功
}


/**
  * @brief  NRF状态获取
  * @param  None
  * @retval 实际状态
  */
uBit32 nRF24L01_GetStatus(void)
{
#if 0
    uBit32 ulStatus = 0;
    
    NRF_CE(0);
    
    ulStatus = nRF24L01_ReadReg(STATUS);//读取status寄存器的值         
    
    if(ulStatus & RX_DR)        //接收到数据 
    {
        nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, ulStatus);          //清除中断标志 
        nRF24L01_ReadBuf(RD_RX_PLOAD, NRF_RX_BUF, RX_PLOAD_WIDTH);  //读取数据 
        nRF24L01_WriteReg(FLUSH_RX, NOP);                           //清除RX FIFO寄存器 
        NRF24L01_Flag = RX_DR; 
    }
    else if(ulStatus & MAX_RT)  //达到最大重发次数 
    {   
        nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, ulStatus);          //清除TX_DS或MAX_RT中断标志 
        nRF24L01_WriteReg(FLUSH_TX,NOP);                            //清除TX FIFO寄存器 
        NRF24L01_Flag = MAX_RT;
    }
    else if(ulStatus & TX_DS)   //发送完成 
    {
        nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, ulStatus);          //清除TX_DS或MAX_RT中断标志 
        nRF24L01_WriteReg(FLUSH_TX,NOP);                            //清除TX FIFO寄存器 
        NRF24L01_Flag = TX_DS;  
    }
    else 
    {
        NRF24L01_Flag = 0;      //没收到任何数据
    }
    
    NRF_CE(1);
    
    return NRF24L01_Flag;
    
#else
    
    NRF_CE(0);
    
    uBit8 uStatus = nRF24L01_ReadReg(STATUS);//读取status寄存器的值
    
    NRF_CE(1);
    
    return uStatus;
#endif
}


/**
  * @brief  NRF地址设置
  * @param  pRFAddr 地址指针,长度为5个字节
  * @retval None
  * @note   发送和接收的节点需要相同地址才能通信
  */
void nRF24L01_SetAddress(uBit8 *pRFAddr)
{
    memcpy(m_uNrfTxAddr, pRFAddr, TX_ADR_WIDTH);
    memcpy(m_uNrfRxAddr, pRFAddr, TX_ADR_WIDTH);
    
}


/**
  * @brief  数组数据发送
  * @param  pTBuff 发送缓冲区,最大31个字节
  * @param  ulSize 要发送的数据字节数
  * @retval 发送结果
  * @note   值得注意的是,这个函数不能在本模块的中断里面执行
  */
uBit8 nRF24L01_SendBuff(uBit8 *pTBuff, uBit8 uSize)
{
    if (uSize > (TX_PLOAD_WIDTH - sizeof(uSize)))
    {
        return 0;
    }
    
    uBit8 uTxBuff[TX_PLOAD_WIDTH] = {0};
    uBit8 uStatus = 0;
    
    memcpy(&uTxBuff[0], &uSize, sizeof(uSize));    //第一个字节表示当前数据帧的实际有效数据长度
    memcpy(&uTxBuff[1], pTBuff, uSize);
    
    //进入发送模式
    nRF24L01_EnterTxMode();  
    
    //发送数据
    NRF_CE(0);
    nRF24L01_WriteBuf(WR_TX_PLOAD, uTxBuff, TX_PLOAD_WIDTH);
    NRF_CE(1);
    
    //等待执行结果
    while (1)
    {
        while (!NRF_IRQ());
    
        //获取执行执行结果
        uStatus = nRF24L01_GetStatus();
        
        if ((uStatus == TX_DS) || (uStatus == MAX_RT))
        {
            NRF_CE(0);
            
            //清除中断标志
            nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, uStatus);
            
            //清除TX FIFO寄存器
            nRF24L01_WriteReg(FLUSH_TX,NOP);
            
            NRF_CE(1);
            
            break;
        }
        
    }

    //进入接收模式
    nRF24L01_EnterRxMode();
    
    return uStatus;
}


/**
  * @brief  数据接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uBit32 nRF24L01_RecvBuff(void *pRBuff, uBit32 ulSize)
{
    
    return RingBuff_PopMult(&m_RxRing, pRBuff, ulSize);
}


/**
  * @brief  接收处理
  * @param  None
  * @retval None
  */
void nRF24L01_RecvHandler(void)
{
    //当IRQ引脚变为低电平时,表示中断产生
    if (!NRF_IRQ())
    {
       //获取执行执行结果
        uBit8 uStatus = nRF24L01_GetStatus();
        
        //判断是否因为中断产生的中断
        if(uStatus & RX_DR)
        {
            uBit8 uRecvBuff[RX_PLOAD_WIDTH] = {0};
            
            //清除中断标志 
            nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, uStatus);
            
            //读取数据 
            nRF24L01_ReadBuf(RD_RX_PLOAD, uRecvBuff, RX_PLOAD_WIDTH); 
            
            //清除RX FIFO寄存器 
            nRF24L01_WriteReg(FLUSH_RX, NOP);
            
            //将数据存储入环形缓冲区
            RingBuff_InsertMult(&m_RxRing, &uRecvBuff[1], uRecvBuff[0]);
        }
        
    }
    
}


