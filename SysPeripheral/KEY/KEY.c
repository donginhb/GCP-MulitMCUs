/**
  ******************************************************************************
  * @file    KEY.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   Key Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "KEY.h"
#include "SysCtrl/SysConfig.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include <string.h>


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define KEY_MAX_SCAN_NUM        (SYS_KEY_SCAN_GROUP)    //最大支持扫描的IO数

static uBit32 m_ulSacnPinGourp[KEY_MAX_SCAN_NUM] = {0}; //按键扫描组
static uBit8  m_uGroupLen = 0;                          //组长度


/*****************************************************************************
 * 按键扫描相关接口
 ****************************************************************************/

/**
  * @brief  按键扫描引脚组设置
  * @param  pScanPinGroup 要扫描引脚的编号数组
  * @param  uGroupLen     组长度
  * @retval 0-成功 非0-失败
  */
uBit32 KEY_SetScanPinGroup(uBit32 *pScanPinGroup, uBit8 uGroupLen)
{
    if (uGroupLen > KEY_MAX_SCAN_NUM)
    {
        return 1;
    }
    
    memcpy(m_ulSacnPinGourp, pScanPinGroup, uGroupLen*sizeof(uBit32));
    m_uGroupLen = uGroupLen;
    
    return 0;
}


/**
  * @brief  按键扫描
  * @param  当前键值,不需要时可以入参NULL
  * @retval 触发源
  */
uBit32 KEY_Scan(uBit32 *pKeyValue)
{
    static uBit32 s_ulOldKey = 0; //键值,按键按下时对应的位置一
    uBit32 ulCurKey = 0;          //本次扫描到的键值
    uBit32 ulCurTrg = 0;          //本次的触发源
    
    if (m_uGroupLen)
    {
        //获取按键键值
        for (int i = 0; i < m_uGroupLen; i++)
        {
            ulCurKey |= (GPIO_MAN_GetInputPinState(m_ulSacnPinGourp[i])) << i;
        }
        
        //参数计算
        ulCurTrg = ulCurKey & (ulCurKey ^ s_ulOldKey);  //计算和上次扫描之间改变的按键位,如果按键没有变化,则结果为0
        s_ulOldKey = ulCurKey;                          //存储键值
        
        if (pKeyValue != NULL)
        {
            *pKeyValue = ulCurKey;
        }
    }
    
    return ulCurTrg;
}
