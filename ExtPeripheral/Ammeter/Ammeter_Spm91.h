#ifndef  __AMMETER_SPM91_H
#define  __AMMETER_SPM91_H

#include "DataType/DataType.h"
#include <stdbool.h>

#pragma pack(push)
#pragma pack(8)	

    
/*****************************************************************************
 * 电表相关参数定义
 ****************************************************************************/

//电表相关参数定义
typedef enum
{
    SPM91_PARAM_TOTAL_ACTIVE_ENERGY     =   ( 1 - 1), //有功电能总和
    SPM91_PARAM_VOLTAGE                 =   ( 3 - 1), //电压
    SPM91_PARAM_CURRENT                 =   ( 4 - 1), //电流
    SPM91_PARAM_ACTIVE_POWER            =   ( 6 - 1), //有功功率
    SPM91_PARAM_APPARENT_POWER          =   ( 8 - 1), //视在功率
    SPM91_PARAM_REACTIVE_POWER          =   (10 - 1), //无功功率
    SPM91_PARAM_FREQUENCY               =   (12 - 1), //频率
    SPM91_PARAM_POWER_FACTOR            =   (13 - 1), //功率因子
    SPM91_PARAM_INPUT_ACTIVE_ENERGY     =   (14 - 1), //输入有功电量
    SPM91_PARAM_OUTPUT_ACTIVE_ENERGY    =   (16 - 1), //输出有功电量
    SPM91_PARAM_INPUT_REACTIVE_ENERGY   =   (20 - 1), //输入无功电量
    SPM91_PARAM_OUTPUT_REACTIVE_ENERGY  =   (22 - 1), //输出无功电量
    SPM91_PARAM_TOTAL_REACTIVE_ENERGY   =   (24 - 1), //总无功电量
}SPM91_AMMETER_PARAM;

    
//SPM91电表数据定义
typedef struct
{
    uBit8  uDevAddr;                    //设备地址
    uBit32 ulParamValue;                //(获取到的)电表参数数值
    SPM91_AMMETER_PARAM AmmeterParam;   //(要获取的)电表参数
    
}SPM91_DATA;


#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  SPM91电表初始化函数
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @param  ulDirPortNO RS485方向控制端口在输出IO表中的编号
  * @retval 0-成功  非0-失败
  */
uBit32 SPM91_Init(uBit8 uUartNode, uBit32 ulBaudRate, uBit32 ulDirPortNO);


/**
  * @brief  SPM91电表参数获取函数
  * @param  SPM91_DATA 接收到电表参数(出参)
  * @retval true - 接收到有效数据 false - 没接收到数据
  * @note   获取到的电表参数数据,都是乘以因子后放大100倍的整型值
  */
bool SPM91_RecvHandler(SPM91_DATA *pSpm91Data);


/**
  * @brief  SPM91电表参数请求函数
  * @param  uDevAddr 设备号
  * @param  SPM91_AMMETER_PARAM 要请求的电表参数
  * @retval None
  */
void SPM91_RequestAmmeterParam(uBit8 uDevAddr, SPM91_AMMETER_PARAM AmmeterParam);

#ifdef __cplusplus
}
#endif

#pragma pack(pop)



#endif /* __AMMETER_SPM91_H */
