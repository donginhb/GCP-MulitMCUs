
#include "string.h"
#include "AbstractionLayer.h"
#include "..\\CMU_Interface.h"
#include "..\\CMU_ErrorCodeDef.h"


#define COM_CAN_MAX_PACK_PER_TIME        (8) //CAN 单次连续可传输的数据包数量
#define COM_ENET_MAX_PACK_PER_TIME       (16)//ENET单次连续可传输的数据包数量
#define COM_UART_MAX_PACK_PER_TIME       (32)//UART单次连续可传输的数据包数量

#define COM_CAN_VALID_MASK          (0x01)
#define COM_ENET_VALID_MASK         (0x02)
#define COM_UART_VALID_MASK         (0x04)

CAN_INTERFACE  m_sCanInterface  = {0};
ENET_INTERFACE m_sEnetInterface = {0};
UART_INTERFACE m_sUartInterface = {0};

//-------------------------------抽象层内部数据区定义------------------------------
static uBit8   m_ucComInvalidFlag=0;                //通信模块可用标志，在相应的通信接口挂接成功后置位
static uBit8   m_ucCanNode;                         //CAN通信节点, 挂接CAN通信接口时设置
static uBit32  m_ulComType = COM_TYPE_NULL;         //连接方式(1-CAN 2-ENET)
static BooLean m_bALInitFlag = false;               //数据初始化标志

/*
函数名称：void CMU_SetCanInterface(uBit16 nCanNode, CAN_INTERFACE sCanInterface);
功    能：挂接CAN通信接口
参    数：
          uComNode               --CAN通信节点
          sCanInterface          --CAN通信接口
返 回 值：无
注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持CAN通信方式，则需要挂接
*/
void CMU_SetCanInterface(uBit16 nCanNode, CAN_INTERFACE sCanInterface)
{
    m_ucComInvalidFlag |= COM_CAN_VALID_MASK;
    m_sCanInterface = sCanInterface;
    m_ucCanNode = nCanNode;
}


/*
函数名称：void CMU_SetEnetInterface(ENET_INTERFACE sEentInterface);
功    能：挂接ENET通信接口
参    数：sEnetInterface         --以太网通信接口
返 回 值：无
注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持以太网通信方式，则需要挂接
*/
void CMU_SetEnetInterface(ENET_INTERFACE sEnetInterface)
{
    m_ucComInvalidFlag |= COM_ENET_VALID_MASK;
    m_sEnetInterface = sEnetInterface;
}


/*
函数名称：void CMU_SetUartInterface(UART_INTERFACE sUartInterface);
功    能：挂接UART通信接口
参    数：sUartInterface         --串口通信接口
返 回 值：无
注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持UART通信方式，则需要挂接
*/
void CMU_SetUartInterface(UART_INTERFACE sUartInterface)
{
    m_ucComInvalidFlag |= COM_UART_VALID_MASK;
    
}


/*
函 数 名：void COM_AL_Init()
函数说明：抽象数据初始化
参数说明：无
返 回 值：无
注意事项：
*/
void COM_AL_Init(void)
{
    m_ulComType = COM_TYPE_NULL;
    m_bALInitFlag = true;
}

/**************协议层调用接口*******************/
/*
函 数 名：uBit32 COM_AL_Open(int iComType)
函数说明：打开通信方式
参数说明：1--CAN通信 2--以太网通信 其他--未设置
返 回 值：0成功 非0-失败
注意事项：
*/
uBit32 COM_AL_Open(uBit32 ulComType)
{
    uBit32 ulRet;

    if (m_bALInitFlag==false)
        COM_AL_Init();

    if (ulComType <= COM_TYPE_NULL || ulComType > COM_TYPE_MAX) //指定的不是合法的通信方式
        return CMU_ERR_CONNECT;

    //通信接口已经打开
    if (ulComType == m_ulComType)
        return CMU_ERR_SUCCESS;

    if ((ulComType&m_ucComInvalidFlag)==0)//要打开的通信方式未挂接通信接口
        return CMU_ERR_CONNECT;

    //关闭已经建立的连接
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        m_sCanInterface.pf_CAN_Close(m_ucCanNode);
        break;
    case COM_TYPE_ENET:
        m_sEnetInterface.pf_ENET_Close();
        break;
    case COM_TYPE_UART:
        m_sUartInterface.pf_UART_Close();
        break;
    default:break;
    }
    
    m_ulComType = ulComType;

    //打开新的通信连接
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        ulRet = m_sCanInterface.pf_CAN_Open(m_ucCanNode, CANBAUD_500K, 0XFFFFFFFF, 0);
        break;
    case COM_TYPE_ENET:
        ulRet = m_sEnetInterface.pf_ENET_Open();
        break;
    case COM_TYPE_UART:
        ulRet = m_sUartInterface.pf_UART_Open();
        break;
    default:break;
    }

    if (ulRet!=CMU_ERR_SUCCESS)
    {
        ulComType=COM_TYPE_NULL;
        return CMU_ERR_CONNECT;
    }

    return CMU_ERR_SUCCESS;
}

//获取当前通信模块的通信类型
uBit32 COM_AL_GetComType(void)
{
    
    return m_ulComType;
}

/*----------------------------------
函 数 名：BooLean COM_AL_IsComTypeValid(uBit32 ulComType)
函数说明：检查通信方式相应的接口是否已经挂接
参数说明：无
返 回 值：无
注意事项：需要时调用
*/
extern BooLean COM_AL_IsComTypeValid(uBit32 ulComType)
{
    if (m_ulComType==COM_TYPE_CAN && 
        (m_ucComInvalidFlag &= COM_CAN_VALID_MASK)==COM_CAN_VALID_MASK)
        return true;

    if (m_ulComType==COM_TYPE_ENET && 
        (m_ucComInvalidFlag &= COM_ENET_VALID_MASK)==COM_ENET_VALID_MASK)
        return true;
    
    if (m_ulComType==COM_TYPE_UART && 
        (m_ucComInvalidFlag &= COM_UART_VALID_MASK)==COM_UART_VALID_MASK)
        return true;

    return false;
}

/*----------------------------------
函 数 名：void COM_AL_Close()
函数说明：关闭通信
参数说明：无
返 回 值：无
注意事项：
*/
void COM_AL_Close(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        m_sCanInterface.pf_CAN_Close(m_ucCanNode);
        break;
    case COM_TYPE_ENET:
        m_sEnetInterface.pf_ENET_Close();
        break;
    case COM_TYPE_UART:
        m_sUartInterface.pf_UART_Close();
        break;
    default:break;
    }
    
    COM_AL_Init();
}

/*----------------------------------
函 数 名：uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack)
函数说明：获取数据包
参数说明：pDataPack--数据包地址
返 回 值：0成功 非0-失败
注意事项：
*/
uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return m_sCanInterface.pf_CAN_GetPack(m_ucCanNode, &pDataPack->ulID.ulFrameID, &pDataPack->ulDataLen, &pDataPack->pDataBuf);
    case COM_TYPE_ENET:
        return m_sEnetInterface.pf_ENET_GetPack(&pDataPack->ulID.ulFrameID, &pDataPack->ulDataLen, &pDataPack->pDataBuf);
    case COM_TYPE_UART:
        return m_sUartInterface.pf_UART_GetPack(&pDataPack->ulID.ulFrameID, &pDataPack->ulDataLen, &pDataPack->pDataBuf);
        break;
    default:break;
    }

    return CMU_ERR_CONNECT;
}


/*----------------------------------
函 数 名：uBit32 COM_AL_SendPack(uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen)
函数说明：发送数据包
参数说明：ulID -- 数据包ID
          pDataBuf -- 数据区
          iDataBufLen -- 数据区长度
返 回 值：0成功 非0-失败
注意事项：
*/
uBit32 COM_AL_SendPack(uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return m_sCanInterface.pf_CAN_SendPack(m_ucCanNode, ulID, pDataBuf, ulDataBufLen);
    case COM_TYPE_ENET:
        return m_sEnetInterface.pf_ENET_SendPack(ulID, pDataBuf, ulDataBufLen);
    case COM_TYPE_UART:
        return m_sUartInterface.pf_UART_SendPack(ulID, pDataBuf, ulDataBufLen);
    default:break;
    }

    return CMU_ERR_CONNECT;
}

/*----------------------------------
函 数 名：uBit32 COM_AL_GetMaxSendLen()
函数说明：抽象层获取单次最大能发送的数据长度
参数说明：无
返 回 值：单包最大允许发送长度
注意事项：
*/
uBit32 COM_AL_GetMaxSendLen(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return m_sCanInterface.pf_CAN_GetMaxSendLen();
    case COM_TYPE_ENET:
        return m_sEnetInterface.pf_ENET_GetMaxSendLen();
    case COM_TYPE_UART:
        return m_sUartInterface.pf_UART_GetMaxSendLen();
    default:break;
    }

    return 0;
}

/*----------------------------------
函 数 名：uBit32 COM_AL_GetMaxSendPacks()
函数说明：获取抽象层可连续发送的数据包数量
参数说明：无
返 回 值：连续可发送的数据包个数
注意事项：最大连续可发送的数据包个数，对于CAN来说可以是多包，但是考虑到效率，必须指定合适值，对于以太网，必须每次
          只能发一包，否则上载大数据时如上传G代码，用户缓冲区数据等，会有丢包，lwip发送成功但实际网络上并没有收到数据包
*/
uBit32 COM_AL_GetMaxSendPacks(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return COM_CAN_MAX_PACK_PER_TIME;
    case COM_TYPE_ENET:
        return COM_ENET_MAX_PACK_PER_TIME;
    case COM_TYPE_UART:
        return COM_UART_MAX_PACK_PER_TIME;
    default:break;
    }

    return 0;
}

/*
功      能： 底层接收数据
参      数： 无 
返  回  值： 无
调 用 位置： 在循环中调用
注意 事 项：   
所在文件名：    
*/
void COM_AL_RcvHandle(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        m_sCanInterface.pf_CAN_RcvHandle(m_ucCanNode);
        break;
    case COM_TYPE_ENET:
        m_sEnetInterface.pf_ENET_RcvHandle();
        break;
    case COM_TYPE_UART:
        m_sUartInterface.pf_UART_RecvHandler();
        break;
    default:break;
    }
    
}
