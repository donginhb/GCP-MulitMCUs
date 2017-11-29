#ifndef _CMU_INTERFACE_H_
#define _CMU_INTERFACE_H_

#include "CMU_DataStructDef.h"

#include "CMU/AbstractionLayer/CMU_CanInterface.h"
#include "CMU/AbstractionLayer/CMU_EnetInterface.h"
#include "CMU/AbstractionLayer/CMU_UartInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
    /*
    函数名称：uBit32 CMU_SetRSBufAddr(uBit32 ulStartAddr, uBit32 ulLen);

    功    能：设置通信管理模块收发缓冲区缓冲区空间

    参    数：ulStartAddr---起始地址
                    ulLen           ---分配到缓冲区长度

    返 回 值：

    注意事项：必须在通信模块初始化之前调用，外部指定的缓冲区空间至少是两个COM_TRANSMIT_BUF_SIZE的长度,在初始化通信模块之前调用
    */
    uBit32 CMU_SetRSBufAddr(uBit32 ulStartAddr, uBit32 ulLen);

#if 0
    /*
    函数名称：void CMU_SetCanInterface(uBit16 nCanNode, CAN_INTERFACE sCanInterface);
    功    能：挂接CAN通信接口
    参    数：
              uComNode             --CAN通信节点
              sCanInterface         --CAN通信接口
    返 回 值：无
    注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持CAN通信方式，则需要挂接
    */
    void CMU_SetCanInterface(uBit16 nCanNode, CAN_INTERFACE sCanInterface);

    /*
    函数名称：void CMU_SetEnetInterface(ENET_INTERFACE sEentInterface);
    功    能：挂接CAN通信接口
    参    数：sEnetInterface         --要挂接以太网通信接口
    返 回 值：无
    注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持以太网通信方式，则需要挂接
    */
    void CMU_SetEnetInterface(ENET_INTERFACE sEnetInterface);

    /*
    函数名称：void CMU_SetUartInterface(UART_INTERFACE sUartInterface);
    功    能：挂接UART通信接口
    参    数：sUartInterface         --串口通信接口
    返 回 值：无
    注意事项：在初始化通信模块之前调用，如果系统跟上位机通信支持UART通信方式，则需要挂接
    */
    void CMU_SetUartInterface(UART_INTERFACE sUartInterface);
#endif
    
    
    /**
      * @brief  CMU CAN 接口设置
      * @param  None
      * @retval 0-成功 非0-失败
      */
    uBit32 CMU_CAN_SetInterface(uBit8 uCanNode, CMU_CAN_INTERFACE *pCanInterface);

    
    /**
      * @brief  CMU ENET 接口设置
      * @param  pEnetInterface 以太网接口指针
      * @retval 0-成功 非0-失败
      */
    uBit32 CMU_ENET_SetInterface(CMU_ENET_INTERFACE *pEnetInterface);
    
    
    /**
      * @brief  CMU UART 接口设置
      * @param  uCanNode CAN节点
      * @param  pCanInterface CAN接口指针
      * @retval 0-成功 非0-失败
      */
    uBit32 CMU_UART_SetInterface(uBit8 uUartNode, CMU_UART_INTERFACE *pUartInterface);
        

    /*
    函数名称：void CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE sCmuExternFunTable)
    功    能：挂接通信模块外部接口
    参    数：
              sCmuExternFunTable    --需挂接的外部操作接口
    返 回 值：无
    注意事项：在初始化通信模块之前调用
    */
    void CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE sCmuExternFunTable);

    /*
    函数名称：uBit32 CMU_Init(uBit8 uComType)
    功    能：初始化通信模块
    参    数：uComType         --- 通信默认采用的通信方式
    返 回 值：无
    注意事项：在挂接通信模块相应接口之后调用
    */
    uBit32 CMU_Init(uBit8 uComType);                    

    /*
    函数名称：uBit32 CMU_GetComType(void)
    功    能：获取当前通信模块使用的通信类型
    参    数：无
    返 回 值：当前的通信类型
    注意事项：
    */
    uBit32 CMU_GetComType(void);

    /*
    函数名称：uBit32 CMU_MainProc();
    功    能：通信模块主线程,管理和上位机的通信（收发数据包处理、周期性查询数据上传、下位机错误上传）
    参    数：无
    返 回 值：0--成功 非0--错误编码
    注意事项：系统初始化时调用，调用前必须先设置通信模块缓冲区、挂接通信接口、挂接其他外部接口
    */
    uBit32 CMU_MainProc(void);    
    
    void CMU_SetIprOutApi(ipr_out_api *pIprOutApi);
    
    
    /*
    函 数 名：uBit32 CMU_SetComType(uBit32 ulComType)
    功    能：设置控制器与上位机的通信方式
    参    数：ulComType  --通信方式
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：设置通信方式切换标志，并保存通信方式到EPPROM中
    */
    uBit32 CMU_SetComType(uBit32 ulComType);

#ifdef __cplusplus
}
#endif

#endif