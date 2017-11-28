#ifndef  __KEY_H
#define  __KEY_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 按键扫描相关接口
 ****************************************************************************/

/**
  * @brief  按键扫描引脚组设置
  * @param  pScanPinGroup 要扫描引脚的编号数组
  * @param  uGroupLen     组长度
  * @retval 0-成功 非0-失败
  */
uBit32 KEY_SetScanPinGroup(uBit32 *pScanPinGroup, uBit8 uGroupLen);


/**
  * @brief  按键扫描
  * @param  当前键值,不需要时可以入参NULL
  * @retval 触发源
  */
uBit32 KEY_Scan(uBit32 *pKeyValue);
    
    
#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
