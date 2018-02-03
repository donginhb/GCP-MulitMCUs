#ifndef  __SYS_PARM_MAN_H
#define  __SYS_PARM_MAN_H

#include "DataType/DataType.h"
#include "Version/Version.h"

/*****************************************************************************
 * CMU相关数据结构定义
 ****************************************************************************/

//CAN/UART参数定义
typedef struct
{
    uBit32 ulComNode;               //通信节点选择
    uBit32 ulBaudRate;              //波特率选择
    
}CMU_CAN_UART_PARM;


//ENET参数定义
typedef struct
{
    uBit8 uIPAddress[4];            //IP地址
    uBit8 uNetMaskAddr[4];          //掩码地址
    uBit8 uGwWayAddr[4];            //网关地址
    uBit8 uMacAddr[6];              //MAC地址
    uBit8 uReserver[2];             //预留位(字节对齐)
    uBit32 ulNetPort;               //网络端口
    
}CMU_ENET_PARM;


//CMU相关参数定义
typedef struct
{
    uBit32          ulComMode;      //通信模式选择
    
    union 
    {
        CMU_CAN_UART_PARM CmuCanUartParm;   //CAN与串口的配置参数
        CMU_ENET_PARM     CmuEnetParm;      //以太网的配置参数
    }ComParm;   
    
    uBit32 ulCrcValue;              //CRC16(考虑字节对齐,用32位变量存储)
    
}SYS_USER_CMU_DATA;     //CMU通信配置参数


/*****************************************************************************
 * Bootloader相关数据结构定义
 ****************************************************************************/

//Bootloader相关参数定义
typedef struct
{
    //Bootloader 相关
    uBit32 ulMainAppFlag;               //主APP存在标志
    uBit32 ulSubAppFlag;                //副APP存在标志
    uBit32 ulIgnoreSubAppFlag;          //忽略副APP标志
    
    uBit32 ulMainAppFlashLen;           //主APP FLASH长度
    uBit32 ulMainAppFlashCheckSum;      //主APP FALSH校验和
    uBit32 ulSubAppFlashLen;            //副APP FLASH长度
    uBit32 ulSubAppFlashCheckSum;       //副APP FALSH校验和
    
    SOFTWARE_VERSION BootloaderVersion; //Bootloader版本
    
    uBit32 ulCrcValue;                  //CRC16(考虑字节对齐,用32位变量存储)
    
}SYS_USER_BOOTLOADER_DATE;  //用户Bootloader数据


/*****************************************************************************
 * 系统参数结构定义
 ****************************************************************************/

//系统参数定义
typedef struct 
{
    SYS_USER_BOOTLOADER_DATE BootloaderData;    //Bootloader数据
    SYS_USER_CMU_DATA        CmuData;           //CMU数据
    
}SYS_USER_PARM;

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * SYS参数管理相关控制接口
 ****************************************************************************/

/**
  * @brief  系统参数初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitSysParm(void);


/**
  * @brief  默认参数设置
  * @param  None
  * @retval None
  */
void SYS_SetDefaultParm(void);


/**
  * @brief  参数存储
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_StoreParmToFLash(void);


/**
  * @brief  系统升级数据获取
  * @param  None
  * @retval 系统升级数据地址
  */
SYS_USER_BOOTLOADER_DATE *SYS_GetSysUpadateParm(void);


/**
  * @brief  CMU配置参数获取
  * @param  None
  * @retval CMU配置参数地址
  */
SYS_USER_CMU_DATA *SYS_GetCmuConfigParm(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_PARM_MAN_H */
