/**
  ******************************************************************************
  * @file    Ds18b20.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.25
  * @brief   Ds18b20 Drivers
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *      DS18B20_BUS  通用开漏,10M
  * 
  * 数据,
  *      8位数据宽度,低位先发
  * 
  * 使用,
  *      先发送温度采集命令,约800ms后调用温度获取函数,其返回的温度值为浮点数
  * 
  * DS18B20为单总线结构,需要接上拉电阻.
  *
  * V1.1------------
  * 修改描述: 修复获取到的温度值一直为0的错误
  * 错误原因: 在字节接收函数中,一释放总线,立即就对总线的电平进行判断,导致结果总为0
  * 解决方法: 在总线释放后加入一个缓冲时间(10us),让总线有时间拉高电平
  * 修改作者: Duhanfneg
  * 当前版本: V1.1
  * 修改日期: 2015.12.10
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/
#include "Ds18b20.h"
#include "DataType/DataType.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static void (*pf_SetBusStates) (bool bStates);
static bool (*pf_GetBusStates) (void);
static void (*pf_DelayUs)(uBit32 ulUs);


/**
  * @brief  总线状态设置
  * @param  bStates true-高电平 false-低电平
  * @retval None
  */
void DS18B20_SetBusStates(bool bState)
{
    if (pf_SetBusStates)
    {
        pf_SetBusStates(bState);
    }
    
}


/**
  * @brief  总线状态获取
  * @param  None
  * @retval true-高电平 false-低电平
  */
bool DS18B20_GetBusStates(void)
{
    if (pf_GetBusStates)
    {
        return pf_GetBusStates();
    }
    
    return 0;
}


/**
  * @brief  微秒级延时
  * @param  ulUs 要延时的微秒数
  * @retval None
  */
void DS18B20_DelayUs(uBit32 ulUs)
{
    if (pf_DelayUs)
    {
        pf_DelayUs(ulUs);
    }
    
}


/**
  * @brief  总线复位函数,仅本文件可用
  * @param  None
  * @retval None
  */
static void DS18B20_ResetPulses(void)
{
    DS18B20_SetBusStates(true);
    DS18B20_SetBusStates(false);
    DS18B20_DelayUs(600);
    DS18B20_SetBusStates(true);
    
    DS18B20_DelayUs(60);
    while(DS18B20_GetBusStates());   //等待DS18B20拉低BUS
    DS18B20_DelayUs(600);
    while(!DS18B20_GetBusStates());  //等待DS18B20拉高BUS
    
}


/**
  * @brief  字节发送
  * @param  uSendData  要发送的8位数据
  * @retval None
  */
static void DS18B20_SendData(uBit8 uSendData)
{
    uBit8 i = 0;
    for (i = 0; i < 8; i++)
    {
        DS18B20_SetBusStates(true);
        DS18B20_SetBusStates(false);
        DS18B20_DelayUs(2);
        
        if (uSendData & (0X1<<i))
        {
            DS18B20_SetBusStates(true);
            DS18B20_DelayUs(100);
        }
        else 
        {
            DS18B20_DelayUs(100);
            DS18B20_SetBusStates(true);   
        }
    }
    
}


/**
  * @brief  字节接收
  * @param  None
  * @retval cRecvData  接收到的8位数据
  */
static uBit8 DS18B20_RecvData(void)
{
    uBit8 i = 0;
    uBit8 cRecvData = 0;
    
    for (i = 0; i < 8; i++)
    {
        DS18B20_SetBusStates(true);
        DS18B20_SetBusStates(false);
        DS18B20_DelayUs(2);
        
        DS18B20_SetBusStates(true);   
        DS18B20_DelayUs(10);       //修改: 在释放总线后加一个时间缓冲
        
        if (DS18B20_GetBusStates())  //在12us的时候读取总线状态,当为"1"时表示收到的数据位"1"
        {
            cRecvData |=  (0X1<<i);
        }
        else
        {
            cRecvData &= ~(0X1<<i);
        }
        DS18B20_DelayUs(80);
    }
    
    return cRecvData ;
}


/**
  * @brief  暂存器读取,读取采集到的温度数据
  * @param  None
  * @retval nReadVal  读取到的16位数据
  */
static uBit16 DS18B20_ReadSP(void)  //SP:scratchpad  暂存器
{
    uBit8  cReadVal_H = 0;     //读取的暂存器的高八位
    uBit8  cReadVal_L = 0;     //读取的暂存器的低八位
    uBit16 nReadVal = 0;       //读取到的16位暂存器值
    
    DS18B20_ResetPulses();
    DS18B20_SendData(0XCC);
    DS18B20_SendData(0XBE);  //获取sp中的温度数据   //修改: 将0XEF改成0XBE
    
    cReadVal_L = DS18B20_RecvData();
    cReadVal_H = DS18B20_RecvData();
    
    nReadVal = (cReadVal_H<<8) | (cReadVal_L<<0);
    
    return nReadVal;
}


/*****************************************************************************
 * DS18B20 相关控制接口
 ****************************************************************************/

/**
  * @brief  DS18B20 硬件接口初始化
  * @param  pf_DS_SetBus    总线状态设置
  * @param  pf_DS_GetBus    总线状态获取
  * @param  pf_DS_DelayUs   微秒级延时函数
  * @retval 0-成功 非0-失败
  */
uBit32 DS18B20_InitInterfaces(void (*pf_DS_SetBus) (bool bStates),
                              bool (*pf_DS_GetBus) (void),
                              void (*pf_DS_DelayUs)(uBit32 ulUs))
{
    do 
    {
        //数据校验
        if (!pf_DS_SetBus)  break;
        if (!pf_DS_GetBus)  break;
        if (!pf_DS_DelayUs) break;
        
        //存储指针
        pf_SetBusStates = pf_DS_SetBus;
        pf_GetBusStates = pf_DS_GetBus;
        pf_DelayUs = pf_DS_DelayUs;
        
        return 0;
    }while (0);
    
    return 1;
}


/**
  * @brief  温度转换启动函数
  * @param  None
  * @retval Note
  */
void DS18B20_StartConvertTemp(void)  
{
    DS18B20_ResetPulses();   //复位
    DS18B20_SendData(0XCC);  //跳过ROM模式
    DS18B20_SendData(0X44);  //开始转换
}


/**
  * @brief  温度数值获取函数
  * @param  None
  * @retval fTempVal  转换后的温度值,单位:摄氏度
  */
float DS18B20_GetTempVal(void)
{
    float fTempVal = 0;
    uBit16 nTmpNum = 0;  //临时变量
    
    /* 获取温度数据 */
    nTmpNum = DS18B20_ReadSP();
    
    
    
    /* 转换为温度值 */
    if (nTmpNum == 0xFFFF)
    {
        fTempVal = 0;
    }
    else if (!(nTmpNum & (1<<15)))         //正温度
    {
        fTempVal = nTmpNum * 0.0625;
        
    }
    else  //负温度
    {
        nTmpNum = ~nTmpNum +1;
        fTempVal = -(nTmpNum*0.0625);   //添加负号
    }
    
    return fTempVal;
}


