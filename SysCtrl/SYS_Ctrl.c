/**
  ******************************************************************************
  * @file    SysCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   System control interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SYS_Ctrl.h"
#include "DataType/DataType.h"
#include "Version/Version.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * 系统版本相关控制接口
 ****************************************************************************/

#if !defined(BOOTLOADER)
/**
  * @brief  系统逻辑控制版本获取
  * @param  pVersion 版本结构指针
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion)
{
    memcpy(pVersion, APP_VER_STR, sizeof(SOFTWARE_VERSION));
    pVersion->cSoftVer[0] = APP_VER_NO0;
    pVersion->cSoftVer[1] = APP_VER_NO1;
    pVersion->cPart = APP_VER_PART;
    pVersion->cFlag = APP_VER_FLAG;
    
    return 0;
}
#endif 


/**
  * @brief  通用控制平台版本获取
  * @param  pVersion 版本结构指针
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_GetGCPVersion(SOFTWARE_VERSION* pVersion)
{
    memcpy(pVersion, GCP_VER_STR, sizeof(SOFTWARE_VERSION));
    pVersion->cSoftVer[0] = GCP_VER_NO0;
    pVersion->cSoftVer[1] = GCP_VER_NO1;
    pVersion->cPart = GCP_VER_PART;
    pVersion->cFlag = GCP_VER_FLAG;
    
    return 0;
}


/*****************************************************************************
 * 系统时间相关控制接口
 ****************************************************************************/

/**
  * @brief  系统滴答时间获取
  * @param  None
  * @retval 滴答值
  * @note   在SysTimer之中已经有完善的接口以满足系统的需求,
  *         本函数的作用是兼容CNC及CMU中系统时基管理的接口;
  */
double SYS_GetTickCount(void)
{
    
    return SysTime_GetTickCount();
}



