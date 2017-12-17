#ifndef  __CMU_ERROR_MANA_H
#define  __CMU_ERROR_MANA_H

#include "DataType/DataType.h"


//定义系统组成模块编号
#define SYS_SOFT_MODULE_COM_WITH_UPCPU        (1)     //与上位机的通信控制模块
                                              
//定义错误等级                                
#define ERR_LEVEL_NORMAL                      (0)     //普通错误（操作性错误）
#define ERR_LEVEL_SYS                         (1)     //系统错误（系统不稳定产生的错误）
#define ERR_LEVEL_SYS_FATAL                   (2)     //系统致命错误（设计性错误：例如模块间调用不合理、系统程序配置不正确等）
                                              
#define ERR_CRD_NO_INVALID                    (0x0F)
#define ERR_DEVICE_NO_INVALID                 (0x3F)


//错误码数据定义
typedef struct _ErrorCode {
    uBit32 ulCodeNO:            8;  //错误编号    
    uBit32 ulCmdType:           8;  //指令类型    
    uBit32 ulDeviceNO:          6;  //设备编号或坐标轴编号，取值从0开始，取值为0x3F时表示数据无效    
                                    //当模块编号为通道管理模块时，表示坐标轴编号    
                                    //当模块编号为设备管理模块时，表示设备编号    
    
    uBit32 ulErrorLevel:        2;  //错误等级    
    uBit32 ulCrdNO:             4;  //通道编号，取值从0开始，取值为0xF时表示数据无效    
    uBit32 ulModuleNO:          4;  //模块编号    
}ERROR_CODE;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CMU 错误管理相关控制接口
 ****************************************************************************/

/**
  * @brief  错误码生成
  * @param  ulCodeNo 错误编码
  * @param  ulID 通信ID
  * @retval 错误码
  */
uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID);


#ifdef __cplusplus
}
#endif

#endif /* __CMU_ERROR_MANA_H */
