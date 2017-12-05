/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： Alan

修改了内容：1.添加获取电机设备数量接口
                
修改日期： 

原 作 者：毛军

完成日期：2015年08月22日        
*/
#include "string.h"
#include "DEV_Abstract.h"
#include "DEV_Interface.h"
#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"
#include "CNC/ErrorCodeDef/DVM_ErrorCodeDef.h"
#include "CNC/ErrorCodeMan/ECM_ErrorCodeMan.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"

//#include "../GPIO/GPIOCtrl.h"

#define DEV_INVALID_BUS_INDEX        (0XFF)


BUS_INFO        m_RegBusInfo[DEV_BUS_MAX_COUNT];        //总线信息
DEVICE_INFO        m_DeviceInfo[DEV_MAX_COUNT];            //设备信息
DEVICE_STATE    m_DeviceState[DEV_MAX_COUNT];            //设备状态
uBit8            m_DevNoBusIndexMap[DEV_MAX_COUNT];        //设备编号所属总线映射表，m_DevNoBusIndexMap[0],表示设备0所属的总线序号，-1表示不属于任何总线，扫描总线时清除，设置硬件配置信息后刷新
uBit8            m_DevInfoBusIndexMap[DEV_MAX_COUNT];        //设备信息所属总线映射表，分别表示设备信息区域存放的设备信息所属总线序号，-1表示不属于任何总线，扫描总线时清除并刷新        
uBit8            m_uRegBusCount = 0;
uBit8            m_uScanDevCount = 0;        //扫描到的设备总数
uBit8            m_uMotorDevCount = 0;        //扫描后电机设备总数
uBit8            m_uCodedDevCount    =    0;  //保存已经编号设备总数
uBit8            m_uCodedAxisCount = 0;      //已编号的轴设备总数，设置硬件配置信息成功后设置
uBit8           m_uMIIIRegFlag = 0;         //MIII总线注册标志


uBit32 DEV_GetError(uBit32 ulErrorCode, uBit32 ulDevNo, uBit32 ulCmdType, uBit32 ulErrorLevel)
{
    if (ulErrorCode!=0)
        return ECM_GenerateErrorCode(SYS_SOFT_MODULE_DEV_MAN, ERR_CRD_NO_INVALID, ulErrorCode, ulCmdType, ulDevNo, ulErrorLevel);

    return DVM_ERR_SUCCESS;
}


//
#ifdef RS_MONITOR_ENALBE

uBit32 DEV_GetBusRsMsData(uBit32 ulBusType, BUS_RS_MS_DATA *pBusTestData)
{
    uBit8 i;

    memset(pBusTestData, 0, sizeof(BUS_RS_MS_DATA));
    
    for (i=0; i<m_uRegBusCount; i++)
    {
        if (m_RegBusInfo[i].nBusType==ulBusType)
        {
            *pBusTestData = m_RegBusInfo[i].BusDevDrv.pf_DEV_GetBusRsMsData();
            break;
        }
    }
    
    return DVM_ERR_SUCCESS;
}

#endif

/*
函 数 名：DEV_SynCycleService
功    能：同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：在插补中断中调用
注意事项：
*/
uBit32 DEV_SynCycleService(void)
{
    uBit8 i;
    uBit32 ulRet = DVM_ERR_SUCCESS;
    uBit32 ulResult = DVM_ERR_SUCCESS;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_FUN_SynCycleService);
#endif
    for (i=0; i<m_uRegBusCount; i++)
    {
        ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_SynCycleService();

        if (ulRet)
            ulResult = ulRet;
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_FUN_SynCycleService);
#endif
    return ulResult;
}

/*
函 数 名：DEV_MIIISynCycleService
功    能：MIII总线同步周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：在非插补中断中执行
注意事项：本接口在插补中断关闭时调用，因为MIII总线必须每个周期都必须收发，否则会导致同步错误
*/
uBit32 DEV_MIIISynCycleService(void)
{
    if (m_uMIIIRegFlag == 0)//MIII总线未注册
        return DVM_ERR_SUCCESS;

    //MIII总线放在其他总线之前
    return m_RegBusInfo[0].BusDevDrv.pf_DEV_SynCycleService();
}

/*
函 数 名：DEV_AsynCycleService
功    能：非同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：(大循环中调用)
注意事项：
*/
uBit32 DEV_AsynCycleService(void)
{
    uBit8 i;
    uBit32 ulRet = DVM_ERR_SUCCESS;
    uBit32 ulResult = DVM_ERR_SUCCESS;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_FUN_AsynCycleService);
#endif

    for (i=0; i<m_uRegBusCount; i++)
    {
        ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_AsynCycleService();

        if (ulRet)
            ulResult = ulRet;
    }
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_FUN_AsynCycleService);
#endif
    return ulResult;
}

/*
函 数 名：DEV_DrvReg
功    能：总线驱动注册
参    数：ulBusType --总线类型
          ulBusChannelMask --总线通道号掩码，bit0-对应通道1，bit1-对应通道2
          pBusDeviceDrive --总线设备驱动接口地址
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时调用
注意事项：
*/
uBit32 DEV_DrvReg(uBit32 ulBusType, uBit32 ulBusChannelMask, BUS_DEVICE_DRIVE* pBusDeviceDrive)
{
    uBit8 i;

    if (m_uRegBusCount==DEV_BUS_MAX_COUNT)
        return DEV_GetError(DVM_ERR_DIV_LIMIT,ERR_DEVICE_NO_INVALID,DVM_CMD_DRVREG,ERR_LEVEL_NORMAL);

    for (i=0; i<m_uRegBusCount; i++)
    {
        if (m_RegBusInfo[i].nBusType==ulBusType)
        {
            m_RegBusInfo[m_uRegBusCount].nBusChannelMask |= ulBusChannelMask;
            return DVM_ERR_SUCCESS;
        }
    }
    
    //安川总线放前面
    if(ulBusType==DEV_BUS_TYPE_MECHALINK_III)
    {
        m_uMIIIRegFlag = 1;

        for(i=m_uRegBusCount; i>0; i--)
            m_RegBusInfo[i] = m_RegBusInfo[i-1];

        m_RegBusInfo[0].nBusType = ulBusType;
        m_RegBusInfo[0].nBusChannelMask = ulBusChannelMask;
        memcpy(&m_RegBusInfo[0].BusDevDrv, pBusDeviceDrive, sizeof(BUS_DEVICE_DRIVE));
 
    }else
    {
        m_RegBusInfo[m_uRegBusCount].nBusType = ulBusType;
        m_RegBusInfo[m_uRegBusCount].nBusChannelMask = ulBusChannelMask;
        memcpy(&m_RegBusInfo[m_uRegBusCount].BusDevDrv, pBusDeviceDrive, sizeof(BUS_DEVICE_DRIVE));     
    }
    
    m_uRegBusCount++;  
    return DVM_ERR_SUCCESS;
}


/*
函 数 名：DEV_SetAxisParmReadAddr
功    能：设置轴控制参数读取地址
参    数：pAxisParm --轴控参数地址
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时，总线注册完成后调用
注意事项：
*/
uBit32 DEV_SetAxisParmReadAddr(const AXIS_PARM *pAxisParm)
{
    uBit8 i;

    for (i=0; i<m_uRegBusCount; i++)
    {
        m_RegBusInfo[i].BusDevDrv.pf_DEV_SetAxisParmReadAddr(pAxisParm);
    }

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_SetAxisParmReadAddr
功    能：设置系统控制参数读取地址
参    数：pSysCtrlParm --系统控制参数地址
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时，总线注册完成后调用
注意事项：
*/
uBit32 DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm)
{
    uBit8 i;

    for (i=0; i<m_uRegBusCount; i++)
    {
        m_RegBusInfo[i].BusDevDrv.pf_DEV_SetSysCtrlParmReadAddr(pSysCtrlParm);
    }

    return DVM_ERR_SUCCESS;
}


/*
函 数 名：DEV_SendSysCtrlParm
功    能：发送系统控制参数,更新所有轴设备控制参数
参    数：
          pSysCtrlParm --系统控制参数数据存储地址
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_SendSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
{
    uBit8 ulDevNo;
    uBit32 ulRet;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_FUN_SendSysCtrlParm);
#endif

    //没有注册的轴设备，则成功退出
    if (m_uCodedAxisCount==0)
        return DVM_ERR_SUCCESS;

    //设置所有轴系统参数
    for (ulDevNo=0; ulDevNo<m_uCodedAxisCount; ulDevNo++)
    {
        ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNo]].BusDevDrv.pf_DEV_SetParm(ulDevNo,
            DEV_PARM_TYPE_SYS_CTRL_PARM,
            (uBit8*)pSysCtrlParm,
            sizeof(SYS_CTRL_PARM));

        if (ulRet)
            return DEV_GetError(ulRet,ulDevNo,DVM_CMD_SEND_SYS_PARM,ERR_LEVEL_NORMAL);
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_FUN_SendSysCtrlParm);
#endif
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_BusInit
功    能：设备总线初始化
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时调用
注意事项：本函数初始化已注册的全部总线，如果总线初始化不成功，则移除挂接的总线
*/
uBit32 DEV_BusInit(void)
{
    uBit8 i;
    uBit32 ulRet;
    uBit8 uCurIndex=0;
    uBit32 ulInitFlag=0;

    m_uScanDevCount    = 0;    
    m_uMotorDevCount = 0;
    m_uMIIIRegFlag = 0;
    
    //扫描到的设备总数
    for (i=0; i<DEV_MAX_COUNT; i++)
    {//设备所属总线映射表，m_DevNoBusIndexMap[0],表示设备1所属的总线编号，0表示不属于任何编号，扫描总线时清除，设置硬件配置信息后刷新
        m_DevInfoBusIndexMap[i] = DEV_INVALID_BUS_INDEX;
        m_DevNoBusIndexMap[i] = DEV_INVALID_BUS_INDEX;
    }

    memset(m_DeviceState,0,sizeof(DEVICE_STATE)*DEV_MAX_COUNT);    //初始化设备状态
    memset(m_DeviceInfo,0,sizeof(DEVICE_INFO)*DEV_MAX_COUNT);        //初始化设备状态

    for (i=0; i<m_uRegBusCount; i++)
    {
        ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_BusInit(m_RegBusInfo[i].nBusChannelMask);
        m_RegBusInfo[i].BusDevDrv.pf_DEV_SetDeviceStateAddr(m_DeviceState,DEV_MAX_COUNT);
        m_RegBusInfo[i].BusDevDrv.pf_DEV_SetDeviceInfoAddr(m_DeviceInfo, 0);  
        
    
        if (ulRet==DVM_ERR_SUCCESS)
        {
            ulInitFlag |= (1<<i);

            if(m_RegBusInfo[i].nBusType == DEV_BUS_TYPE_MECHALINK_III)
                m_uMIIIRegFlag = 1;
        }
    }

    //移除初始化失败的总线
    for (i=0; i<m_uRegBusCount; i++)
    {
        if (ulInitFlag&(1<<i))//存在
        {
            if (uCurIndex != i)
                m_RegBusInfo[uCurIndex] = m_RegBusInfo[i];

            uCurIndex++;
        }
    }

    m_uRegBusCount = uCurIndex;
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_CheckBusInitSuccess
功    能：检测总线是否初始化成功
参    数：ulBusType --总线类型
返 回 值：成功返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 DEV_CheckBusInitSuccess(uBit32 ulBusType)
{
    if(ulBusType==DEV_BUS_TYPE_MECHALINK_III)
    {
       if(m_uMIIIRegFlag==0)
         return 0;
    }

    return 1;
}

/*
函 数 名：DEV_SetComCycle
功    能：设置通讯周期
参    数：ulTimeCycle --通信周期，单位：微秒
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：目前用于M-III总线的通信周期设置，根据系统插补周期设置M-III总线的通信周期
*/
uBit32 DEV_SetComCycle(uBit32 ulTimeCycle)
{
    //Alan???
    return DVM_ERR_SUCCESS;
}


/*
函 数 名：DEV_Scan
功    能：扫描设备
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：扫描所有总线所有设备，将重置设备信息数据区,并更新编号设备所属总线编号表
*/
uBit32 DEV_Scan(void)
{    
    uBit8 i,j,k;
    uBit32 ulRet;
    uBit32 ulCurIndex=0;
    uBit32 ulCurBusTotalDevCount=0;                        //当前总线扫描到的设备总数(总线上实际扫到的设备数量)
    uBit32 ulCurBusRecDevCount=0;                            //当前总线已经保存到缓冲区的设备数量(总线上实际保存到缓冲区设备数量)
    DEVICE_INFO pBusScanInfo[DEV_MAX_COUNT];        //保存所有总线扫描到的设备信息
    uBit8 pDevInfoBusIndexMap[DEV_MAX_COUNT];        //新扫描后的设备信息所属总线表

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_FUN_Scan);
#endif

    //只要总线忙
    for (i=0; i<m_uRegBusCount; i++)
    {
        if(m_RegBusInfo[i].BusDevDrv.pf_DEV_IsBusFree()==0)
            return DEV_GetError(DVM_ERR_BUS_BUSY,ERR_DEVICE_NO_INVALID,DVM_CMD_SCAN,ERR_LEVEL_NORMAL);
    }

    memset(pBusScanInfo,0, sizeof(DEVICE_INFO)*DEV_MAX_COUNT);

    for (i=0; i<DEV_MAX_COUNT; i++)
    {
        pDevInfoBusIndexMap[i] = DEV_INVALID_BUS_INDEX;
        pBusScanInfo[i].nDeviceNO = DEV_INVALID_NO;
        pBusScanInfo[i].nDeviceNOLast = DEV_INVALID_NO;
    }


    for (i=0; i<m_uRegBusCount; i++)
    {
        ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_Scan(&pBusScanInfo[ulCurIndex],
            DEV_MAX_COUNT-ulCurIndex,
            &ulCurBusRecDevCount, 
            &ulCurBusTotalDevCount);

        if (ulRet != DVM_ERR_SUCCESS)
            continue;

        //总线未记录任何设备
        if (ulCurBusRecDevCount==0)
        {
            //删除设备编号对应的总线编号表中已经不存在的设备
            for (int j=0; j<m_uCodedDevCount; j++)
            {
                if (m_DevNoBusIndexMap[j] == i)
                    m_DevNoBusIndexMap[j] =  DEV_INVALID_BUS_INDEX;
            }

            continue;
        }
        

        //删除设备编号对应的总线编号表中已经拔掉的编号设备
        for (j=0;  j<m_uScanDevCount;  j++)
        {
            if (m_DevInfoBusIndexMap[j]==i)//先前属于该总线的设备
            {
                //查看先前设备是否还存在(比较ID)
                for (k=0; k<ulCurBusRecDevCount; k++)
                {
                    if (pBusScanInfo[ulCurIndex+k].nDevieID == m_DeviceInfo[j].nDevieID)//旧设备在新扫描的设备中，保存现有编号
                    {
                        if(m_RegBusInfo[i].nBusType != DEV_BUS_TYPE_MECHALINK_III)
                        {
                            pBusScanInfo[ulCurIndex+k].nDeviceNO = m_DeviceInfo[j].nDeviceNO;
                        }
                        
                        break;
                    }
                }

                //新设备中找不到原有已编号设备则从总线编号表中移除该设备
                if (k==ulCurBusRecDevCount)
                {
                    if (m_DeviceInfo[j].nDeviceNO<m_uCodedDevCount)//先前的设备编号有效
                    {
                        if (m_DevNoBusIndexMap[m_DeviceInfo[j].nDeviceNO]==i)//先前有效编号设备确实属于本总线
                            m_DevNoBusIndexMap[m_DeviceInfo[j].nDeviceNO] =  DEV_INVALID_BUS_INDEX;
                    }
                }
            }
        }

        //重建设备信息所属总线映射表
        //for (j=ulCurIndex; j<ulCurIndex+ulCurBusDevCount; j++)
        for (j=ulCurIndex; j<ulCurIndex+ulCurBusRecDevCount; j++)
            pDevInfoBusIndexMap[j] = i;

        //ulCurIndex += ulCurBusDevCount;
        ulCurIndex += ulCurBusRecDevCount;

        if (ulCurBusRecDevCount<ulCurBusTotalDevCount)
        {//总线有多余的设备没有记录
            ;
        }

        if (ulCurIndex==DEV_MAX_COUNT)//所以总线设备超过系统允许的最大设备数量时
            break;
    }
    
    //设备排序，轴设备按编号大小排在前面，后面依次是IO设备、ADDA设备等其他设备
    m_uScanDevCount = 0;
    memset(m_DeviceInfo,0,sizeof(DEVICE_INFO)*DEV_MAX_COUNT);

    for (i=0; i<DEV_MAX_COUNT; i++)
    {
        m_DevInfoBusIndexMap[i] = DEV_INVALID_BUS_INDEX;
        m_DeviceInfo[i].nDeviceNO = DEV_INVALID_NO;
    }

    //伺服和轴设备放在前面
    for (i=0; i<ulCurIndex; i++)
    {
        if (pBusScanInfo[i].nDeviceType==DEV_TYPE_SV ||
            pBusScanInfo[i].nDeviceType==DEV_TYPE_AXIS)
        {
            m_DeviceInfo[m_uScanDevCount] = pBusScanInfo[i];
            m_DevInfoBusIndexMap[m_uScanDevCount] = pDevInfoBusIndexMap[i];
            m_uScanDevCount++;
        }
    }

    //保存电机设备数量
    m_uMotorDevCount = m_uScanDevCount;

    //其他设备放后面(OUTPUT+INPUT+ADDA)
    for (i=0;  i<=DEV_TYPE_COUNT; i++)
    {
        if (i==DEV_TYPE_SV ||i==DEV_TYPE_AXIS)
            continue;

        for (j = 0; j<ulCurIndex; j++)
        {
            if (pBusScanInfo[j].nDeviceType == i)
            {
                m_DeviceInfo[m_uScanDevCount] = pBusScanInfo[j];
                m_DevInfoBusIndexMap[m_uScanDevCount] = pDevInfoBusIndexMap[j];
                m_uScanDevCount++;
            }

            if (m_uScanDevCount==ulCurIndex)
                break;
        }

        if (m_uScanDevCount==ulCurIndex)
            break;
    }
/*
    for (i=0; i<ulCurIndex; i++)
    {
        if (pBusScanInfo[i].nDeviceType!=DEV_TYPE_SV &&
            pBusScanInfo[i].nDeviceType!=DEV_TYPE_AXIS)
        {
            m_DeviceInfo[m_uScanDevCount] = pBusScanInfo[i];
            m_DevInfoBusIndexMap[m_uScanDevCount] = pDevInfoBusIndexMap[i];
            m_uScanDevCount++;
        }
    }*/



    //设置总线设备资源信息地址-----------
    for(i=0; i<m_uRegBusCount; i++)
    {
        m_RegBusInfo[i].BusDevDrv.pf_DEV_SetDeviceInfoAddr(m_DeviceInfo, m_uScanDevCount);    
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_FUN_Scan);
#endif

    //清除设备状态信息
    //memset(m_DeviceState, 0, sizeof(DEVICE_STATE)*DEV_MAX_COUNT);
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_GetCount
功    能：获取设备总数量
参    数：无
返 回 值：返回设备总数量(已经扫描到的设备数量)
调用位置：
注意事项：
*/
uBit32 DEV_GetCount(void)
{
    return m_uScanDevCount;
}

/*
函 数 名：DEV_GetMotorCount
功    能：获取电机设置总数
参    数：无
返 回 值：返回电机总数量(已经扫描到的设备数量)
调用位置：
注意事项：电机设备包括轴卡、CAN伺服、MIII伺服
*/
uBit32 DEV_GetMotorCount(void)
{
    return m_uMotorDevCount;
}

/*
函 数 名：DEV_GetInfo
功    能：获取设备信息
参    数：ulDevIndex --设备序号，表示获取扫描到的第几个设备的信息
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_GetInfo(uBit32 ulDevIndex, DEVICE_INFO* pDeviceInfo)
{
    if (ulDevIndex>=m_uScanDevCount)
        return DEV_GetError(DVM_ERR_UNKNOW_DEV, ulDevIndex, DVM_CMD_GET_DEV_INFO, ERR_LEVEL_NORMAL);

    *pDeviceInfo = m_DeviceInfo[ulDevIndex];
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_GetRegDevInfo
功    能：获取已经编号的设备信息
参    数：ulDevNO --设备编号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_GetRegDevInfo(uBit32 ulDevNO, DEVICE_INFO* pDeviceInfo)
{
    uBit8 i;
    
    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_INFO,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_INFO,ERR_LEVEL_NORMAL);

    for (i=0; i<m_uScanDevCount; i++)//查找设备
    {
        if (m_DeviceInfo[i].nDeviceNO==ulDevNO)
        {
            *pDeviceInfo = m_DeviceInfo[i];
            return DVM_ERR_SUCCESS;
        }
    }

    return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_INFO,ERR_LEVEL_NORMAL);
}

/*
函 数 名：DEV_BlinkLED;
功    能：获取设备信息
参    数：ulDeviceID --设备ID
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：依次在个总线上发送，直到有总线回应
*/
uBit32 DEV_BlinkLED(uBit32 ulDeviceID)
{
    uBit8 i;
    uBit32 ulRet=DVM_ERR_UNKNOW_DEV;

    for (i=0; i<m_uScanDevCount; i++)
    {
        if (m_DeviceInfo[i].nDevieID == ulDeviceID)
        {
            ulRet = m_RegBusInfo[m_DevInfoBusIndexMap[i]].BusDevDrv.pf_DEV_SetCmd(ERR_DEVICE_NO_INVALID, 
                DEV_CMD_BLINK_LED, ulDeviceID, m_DeviceInfo[i].nBusNO, 1);
            break;
        }
    }

    return DEV_GetError(ulRet,0,DVM_CMD_BLINK_LED,ERR_LEVEL_NORMAL);
}

/*
函 数 名：DEV_SetConfig
功    能：配置设备编号
参    数：pDeviceConfig --设备配置数据存储地址
          ulDeviceCount --要配置的设备数量
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：要求:1、一次性设置所有设备编号（便于查错）；2、轴编号在前，其他设备编号在后，编号从1开始
*/
uBit32 DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount)
{
    uBit8 i,j;
    uBit32 ulRet;        
    uBit32 ulDevExistMask = 0;                    //设备存在屏蔽码，1表示该位置扫描设备存在，是一个低位连续为1的数
    uBit32 ulDevCodeFlag=0;                    //设备编号位置，1表示分配编号 0表示未分配编号
    uBit8 uAxisCount=0;                            //轴设备总数
    uBit8 uDevExist=0;                            //设备存在标志
    uBit8 uAxisFlag=1;                            //轴设备标识        
    DEVICE_INFO *pCurDevInfo;
    DEVICE_CONFIG pDevCofig[DEV_MAX_COUNT];    //按设备编号顺序保存设备配置信息

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_FUN_SetConfig);
#endif

    //只要有总线忙则不执行
    for (i=0; i<m_uRegBusCount; i++)
    {
        if(m_RegBusInfo[i].BusDevDrv.pf_DEV_IsBusFree()==0)
            return DEV_GetError(DVM_ERR_BUS_BUSY,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);
    }

    //数量检查----------
    //if (ulDeviceCount != m_uScanDevCount)
    //    return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);
    if (ulDeviceCount > m_uScanDevCount)
        return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);

    //编号有效性检查，编号大于0小于等于设备总数，编号必须连续，且不能重复
    for (i=0; i<ulDeviceCount; i++, pDeviceConfig++)
    {
        ulDevExistMask  |= 0x01<<i;

        if (pDeviceConfig->nDeviceNO>=ulDeviceCount)
            return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);

        if (ulDevCodeFlag & (0x01<<(pDeviceConfig->nDeviceNO)))//有相同编号
            return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);

        ulDevCodeFlag |= 0x01<<(pDeviceConfig->nDeviceNO);
        pDevCofig[i] = *pDeviceConfig;    //顺序保存硬件配置信息
    }

    if (ulDevExistMask != ulDevCodeFlag)//设备编号不连续
        return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_NORMAL);
    //----------------------------------------------------------------------

    //轴编号检查和ID检查，轴编号必须位于前面，ID标识的设备必须存在
    for (i=0; i<ulDeviceCount; i++)
    {
        //根据配置信息ID，查找该设备是否存在
        uDevExist = 0;

        for (j=0; j<m_uScanDevCount; j++)
        {
            if(m_DeviceInfo[j].nDevieID != pDevCofig[i].nDevieID)
                continue;

            pCurDevInfo = m_DeviceInfo+j;
            uDevExist = 1;
            m_DevNoBusIndexMap[pDevCofig[i].nDeviceNO]=m_DevInfoBusIndexMap[j];//保存设备编号所在的总线编号
            break;
        }

        if (uDevExist==0)    //设备不存在
        {
            for (j=0; j<DEV_MAX_COUNT; j++)//清除设备编号对应总线映射表
                m_DevNoBusIndexMap[j] = DEV_INVALID_BUS_INDEX;

            return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_SYS);
        }
        
        //检查是否还是轴设备
        if (uAxisFlag==1)
        {
            if (pCurDevInfo->nDeviceType == DEV_TYPE_AXIS || pCurDevInfo->nDeviceType == DEV_TYPE_SV)//还是轴设备
            {
                uAxisCount++;
                continue;
            }
            
            uAxisFlag = 0;    //设置轴设备结束标志
        }else
        {
            if (pCurDevInfo->nDeviceType == DEV_TYPE_AXIS|| pCurDevInfo->nDeviceType == DEV_TYPE_SV)//轴连续编号后还有轴设备则报错
            {
                for (j=0; j<DEV_MAX_COUNT; j++)//清除设备编号对应总线映射表
                    m_DevNoBusIndexMap[j] = DEV_INVALID_BUS_INDEX;

                return DEV_GetError(DVM_ERR_CONF_DISMATCH,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_SYS);
            }
        }
    }
    //----------------------------------------------------------------------

    //发送硬件配置信息，只要有总线硬件配置不成功，则硬件配置信息不可用-----------
    for (i=0; i<m_uRegBusCount; i++)
    {
        ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_SetConfig(pDevCofig, ulDeviceCount);

        if (ulRet != DVM_ERR_SUCCESS)//有总线配置硬件信息失败
        {
            m_uCodedAxisCount = 0;        
            m_uCodedDevCount = 0;

            for (j=0; j<DEV_MAX_COUNT; j++)//清除设备编号对应总线映射表
                m_DevNoBusIndexMap[j] = DEV_INVALID_BUS_INDEX;

            return DEV_GetError(ulRet,ERR_DEVICE_NO_INVALID,DVM_CMD_SET_COFIG,ERR_LEVEL_SYS);
        }
    }

    //记录已经编号的轴设备总数
    m_uCodedAxisCount = uAxisCount;
    m_uCodedDevCount = ulDeviceCount;
    //----------------------------------------------------------------------
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_FUN_SetConfig);
#endif
    return DVM_ERR_SUCCESS;
}




/*
函 数 名：DEV_GetVersion
功    能：获取设备软件版本信息
参    数：ulDevNO --设备编号
          *pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion)
{
    uBit32 ulRet;

    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_VER,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulDevNO]== DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_VER,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_GetVersion(ulDevNO, pVersion);
    return DEV_GetError(ulRet,ulDevNO,DVM_CMD_GET_DEV_VER,ERR_LEVEL_NORMAL);
}

/*
函 数 名：DEV_GetBusDriverVersion
功    能：获取总线驱动软件版本信息
参    数：ulBusType --总线类型
          *pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_GetBusDriverVersion(uBit32 ulBusType, SOFTWARE_VERSION* pVersion)
{
    uBit8 i;
    uBit32 ulRet = DVM_ERR_UNKNOW_BUS;

    for (i=0; i<m_uRegBusCount; i++)
    {
        if (m_RegBusInfo[i].nBusType==ulBusType)
        {
            ulRet = m_RegBusInfo[i].BusDevDrv.pf_DEV_GetDriverVersion(pVersion);
            break;
        }
    }

    return DEV_GetError(ulRet,0,DVM_CMD_GET_DIV_VER,ERR_LEVEL_NORMAL);
}

/*
函 数 名：DEV_GetState
功    能：获取设备状态
参    数：ulDevNO --设备编号
          *pDeviceState --设备状态数据（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：设置设备编号前不能获取设备状态
*/
uBit32 DEV_GetState(uBit32 ulDevNO, DEVICE_STATE* pDeviceState)
{
    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_STATA,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_DEV_STATA,ERR_LEVEL_NORMAL);

    *pDeviceState = m_DeviceState[ulDevNO];

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：DEV_CheckNonAxisDevAlarm
功    能：检测所有非轴卡设备的报警
参    数：无
返 回 值：报警状态
                #define IO_STATUS_OVER_CURRENT                    (0x01)    //IO模块过流报警
                #define IO_STATUS_OFF_LINE                                (0X02)    //IO或ADDA脱离总线
                #define IO_STATUS_SEND_CYCLE_DATA_FAIL      (0x04)    //发送周期数据失败
调用位置：
注意事项：检测的是所有非轴设备的报警状态，只要有一个非轴模块报警则报警
*/
uBit8 DEV_CheckNonAxisDevAlarm(void)
{
    uBit8 uDevNo;
    uBit8 uDevSysStatus=0;

    if (m_uCodedDevCount<=m_uCodedAxisCount)
        return 0;

    for (uDevNo=m_uCodedAxisCount;    uDevNo<m_uCodedDevCount;  uDevNo++)
    {
        switch(m_DeviceInfo[uDevNo].nDeviceType)
        {
        case DEV_TYPE_OUTPUT:
        case DEV_TYPE_INPUT:
        case DEV_TYPE_IO:
            {
                uDevSysStatus |= m_DeviceState[uDevNo].IOStateData.cSysStatus;
                break;
            }
        case DEV_TYPE_ADDA:
            {
                uDevSysStatus |= m_DeviceState[uDevNo].ADDAStateData.cSysStatus;
                break;
            }
        default:
            break;
        }
    }

    return uDevSysStatus&0x07;
}

/*
函 数 名：DEV_GetStateReadAddr
功    能：获取设备状态读地址
参    数：ulDevNO --设备编号
返 回 值：设备地址
调用位置：
注意事项：
*/
const DEVICE_STATE* DEV_GetStateReadAddr(uBit32 ulDevNO)
{
    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return NULL;

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return NULL;

    return &m_DeviceState[ulDevNO];
}

/*
函 数 名：const DEVICE_INFO* DEV_GetDevInfoReadAddr(void)
功    能：获取设备资源信息起始地址
参    数：无
返 回 值：资源设备信息起始地址
调用位置：
注意事项：
*/
const DEVICE_INFO* DEV_GetDevInfoReadAddr(void)
{
    return m_DeviceInfo;
}

/*
函 数 名：DEV_SendUpdateAppData
功    能：发送设备应用程序升级数据
参    数：ulDevNO --设备编号
          pUpdateData --应用程序升级数据缓冲区
          ulDataLen --应用程序升级数据长度
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_SendUpdateAppData(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 ulDataLen)
{
    uBit32 ulRet;

    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_UPDATE_DEV_APP,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_UPDATE_DEV_APP,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_SetParm(ulDevNO,
        DEV_PARM_TYPE_UPDATE_APP,
        pUpdateData,
        ulDataLen);

    return DEV_GetError(ulRet,ulDevNO,DVM_CMD_UPDATE_DEV_APP,ERR_LEVEL_NORMAL);
}

/*
函 数 名：DEV_SendUpdateBootData
功    能：发送设备BOOTLOADER程序升级数据
参    数：ulDevNO --设备编号
          pUpdateData --BOOTLOADER程序升级数据缓冲区
          iDataLen --BOOTLOADER程序升级数据长度
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 DEV_SendUpdateBootData(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 iDataLen)
{
    uBit32 ulRet;

    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_UPDATE_DEV_BOOT,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_UPDATE_DEV_BOOT,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_SetParm(ulDevNO,
        DEV_PARM_TYPE_UPDATE_BOOT,
        pUpdateData,
        iDataLen);

    return DEV_GetError(ulRet,ulDevNO,DVM_CMD_UPDATE_DEV_BOOT,ERR_LEVEL_NORMAL);
}


//
/*
函 数 名：DEV_SendCmd
功    能：设置设备控制指令
参    数：ulDevNO --设备编号
          ulCmdType --指令类型，取值为：
              DEV_CMD_INQ_INFO                    (0)    //查询设备信息
              DEV_CMD_SET_NO                    (1)    //设置设备编号
              DEV_CMD_BLINK_LED                (2)    //设置LED闪烁（设备测试）
              DEV_CMD_INQ_VER                    (3)    //查询版本信息
              DEV_CMD_CLR_ALM                    (4)    //报警清除
              DEV_CMD_ENABLE_SV                (5)    //使能电机
              DEV_CMD_SET_HOME                (6)    //回零
              DEV_CMD_SET_LOCK                    (7)    //轴锁定(表示轴正在进行插补运动) （待完善）
              DEV_CMD_RESET                        (8)    //系统复位
              DEV_CMD_SET_CMD_POS            (9)    //设置指令位置
              DEV_CMD_SET_QEI_POS            (10)    //设置编码器位置
              DEV_CMD_SET_VERIFY_MODE        (11)    //设置进入校验模式
              DEV_CMD_SET_IO_PWM_MODE    (12)    //设置IO进入PWM模式
              DEV_CMD_SET_IO_PWM_DUTY    (13)    //设置IO进入PWM占空比
              DEV_CMD_SET_IO_PWM_FREQ    (14)    //设置IO进入PWM频率
              DEV_CMD_SET_FEED                    (15)    //设置进给
              DEV_CMD_SET_OUTPUT                (16)    //设置IO输出
              DEV_CMD_SET_FSTP                    (17)   //CCW驱动禁止                              
              DEV_CMD_SET_RSTP                    (18)   //CW驱动禁止                                
              DEV_CMD_SET_CLESC1ZERO        (19)   //偏差计算清零/速度选择1/零速箝位      
              DEV_CMD_SET_INH_SC2            (20)   //指令脉冲禁止/速度选择2              
              DEV_CMD_SET_FIL                    (21)   //CCW转矩限制                            
              DEV_CMD_SET_RIL                    (22)   //CW转矩限制 
              DEV_CMD_INQ_SV_ALM                (23)   //查询伺服报警
              DEV_CMD_SET_IPO_CYCLE            (24)   //设置插补周期
              DEV_CMD_GET_SV_PARM            (25)    //获取伺服参数

              lCmdPara1，lCmdPara2 --控制参数，对应各控制指令其意义为：
              DEV_CMD_BLINK_LED                --lCmdPara1：设备ID；lCmdPara2：无效
              DEV_CMD_INQ_VER                    --lCmdPara1：无效；lCmdPara2：无效
              DEV_CMD_CLR_ALM                    --lCmdPara1：无效；lCmdPara2：无效
              DEV_CMD_ENABLE_SV                --lCmdPara1：0-断使能，1-使能；lCmdPara2：无效
              DEV_CMD_SET_HOME                --lCmdPara1：0-回零停止，1-回零开始；lCmdPara2：无效
              DEV_CMD_SET_LOCK                    --lCmdPara1：0-解除锁定，1-锁定；lCmdPara2：无效
              DEV_CMD_RESET                        --lCmdPara1：无效；lCmdPara2：无效
              DEV_CMD_SET_CMD_POS            --lCmdPara1：指令位置；lCmdPara2：无效
              DEV_CMD_SET_QEI_POS            --lCmdPara1：编码器位置；lCmdPara2：无效
              DEV_CMD_SET_VERIFY_MODE        --lCmdPara1：0-关闭校验模式，1-打开校验模式；lCmdPara2：无效
              DEV_CMD_SET_IO_PWM_MODE    --lCmdPara1：要设置PWM模式的IO点编号；lCmdPara2：0-关闭PWM模式，1-开启PWM模式；
              DEV_CMD_SET_IO_PWM_DUTY    --lCmdPara1：IO点编号；lCmdPara2：占空比
              DEV_CMD_SET_IO_PWM_FREQ    --lCmdPara1：IO点编号；lCmdPara2：频率
              DEV_CMD_SET_OUTPUT                --当为OUTPUT输出设备时，lCmdPara1：输出状态值；lCmdPara2：输出位掩码
              --当为DA输出设备时，lCmdPara1：DA输出值；lCmdPara2：DA通道编号
              DEV_CMD_SET_FSTP                    --lCmdPara1：0-CCW驱动禁止关闭，1-CCW驱动禁止打开；lCmdPara2：无效
              DEV_CMD_SET_RSTP                    --lCmdPara1：0-CW驱动禁止关闭，1-CW驱动禁止打开；lCmdPara2：无效
              DEV_CMD_SET_CLESC1ZERO        --lCmdPara1：无效；lCmdPara2：无效
              DEV_CMD_SET_INH_SC2            --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
              DEV_CMD_SET_FIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
              DEV_CMD_SET_RIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
              DEV_CMD_INQ_SV_ALM                --lCmdPara1：无效；lCmdPara2：无效
              DEV_CMD_GET_SV_PARM            --lCmdPara1：要获取的伺服参数编号 lCmdPara2:无效
        iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：内部限制同一总线上的设备同一时间只有一条控制指令在执行
*/
uBit32 DEV_SendCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return DVM_ERR_UNKNOW_DEV;

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DVM_ERR_UNKNOW_DEV;

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_SetCmd(ulDevNO,
        ulCmdType,
        lCmdPara1,
        lCmdPara2,
        iWaitExeResult);

    return ulRet;
}


/*
函 数 名：DEV_CheckFree
功    能：检测设备是否空闲（没有指令在执行）
参    数：ulDevNO --设备编号
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 DEV_CheckFree(uBit32 ulDevNO)
{
    if (ulDevNO>=m_uCodedDevCount)//无效编号
        return 1;

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 1;

    return m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_CheckFree(ulDevNO);
}


/*
函 数 名：DEV_CheckLastCmdExeState
功    能：检测上一次的指令执行状态
参    数：ulDevNO            --设备编号
                *pCmdType        --指令类型（出参）
                pExeRes            --执行结果(出参)
返 回 值：执行完成返回0，1-正在执行
调用位置：
注意事项：
*/
uBit32 DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType, uBit32 *pExeRes)    
{
    uBit32 ulRet;

    if (ulDevNO>=m_uCodedDevCount)//无效编号
    {
        *pExeRes = DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_LAST_EXE_STATE,ERR_LEVEL_NORMAL);
        return 0;
    }

    if (m_DevNoBusIndexMap[ulDevNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
    {
        *pExeRes = DEV_GetError(DVM_ERR_UNKNOW_DEV,ulDevNO,DVM_CMD_GET_LAST_EXE_STATE,ERR_LEVEL_NORMAL);
        return 0;
    }

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulDevNO]].BusDevDrv.pf_DEV_CheckLastCmdExeState(ulDevNO, pCmdType);

    if (ulRet==DVM_ERR_EXECUTTING)
        return 1;

    *pExeRes = DEV_GetError(ulRet,ulDevNO,DVM_CMD_GET_LAST_EXE_STATE,ERR_LEVEL_NORMAL);

    return 0;
}


//----------------------------------------------轴相关操作接口（电机轴）----------------------------------------------
/*
函 数 名：PAX_GetRegCount
功    能：获取已编号的轴（物理轴）数量
参    数：
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_GetRegCount(void)
{
    return m_uCodedAxisCount;
}

/*
函 数 名：PAX_SendAxisParm
功    能：发送轴参数
参    数：ulAxisNO --轴编号
          pAxisParm --轴参数数据存储地址
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_SendAxisParm(uBit32 ulAxisNO, AXIS_PARM* pAxisParm)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SEND_AXIS_PARM,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SEND_AXIS_PARM,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulAxisNO]].BusDevDrv.pf_DEV_SetParm(ulAxisNO,
        DEV_PARM_TYPE_AXIS_PARM,
        (uBit8*)pAxisParm,
        sizeof(AXIS_PARM));

    return DEV_GetError(ulRet,ulAxisNO,DVM_CMD_SEND_AXIS_PARM,ERR_LEVEL_NORMAL);
}

/*
函 数 名：PAX_SendSvParm
功    能：设置伺服参数
参    数：ulAxisNO        --轴编号
                pParmData    --伺服参数数据区（2字节保存到FLASH标志+2字节参数编号+4字节参数数据）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_SendSvParm(uBit32 ulAxisNO, uBit8 *pParmData)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SEND_SV_PARM,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SEND_SV_PARM,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulAxisNO]].BusDevDrv.pf_DEV_SetParm(ulAxisNO, 
        DEV_PARM_TYPE_SV, pParmData, 8);

    return DEV_GetError(ulRet,ulAxisNO,DVM_CMD_SEND_SV_PARM,ERR_LEVEL_NORMAL);   
}


/*
函 数 名：PAX_SendFeed
功    能：设置轴进给
参    数：ulAxisNO --轴编号
          IPOAxisState --轴插补状态数据
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState)
{
      uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_FEED,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_FEED,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulAxisNO]].BusDevDrv.pf_DEV_SendFeed(ulAxisNO, IPOAxisState);
    return DEV_GetError(ulRet,ulAxisNO,DVM_CMD_SET_AXIS_FEED,ERR_LEVEL_NORMAL);     
}


/*
函 数 名：PAX_Enable
功    能：设置轴使能
参    数：ulAxisNO --轴编号
          iEnable --使能标记，0-断使能，1-加使能
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_Enable(uBit32 ulAxisNO, Bit32 iEnable, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_ENABLE_AXIS,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_ENABLE_AXIS,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulAxisNO,DEV_CMD_ENABLE_SV,iEnable,0,iWaitExeResult);
    return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_ENABLE_AXIS,ERR_LEVEL_NORMAL);
}

/*
函 数 名：PAX_Reset
功    能：轴复位
参    数：ulAxisNO --轴编号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_Reset(uBit32 ulAxisNO)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_RESET,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_RESET,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulAxisNO,DEV_CMD_RESET,0,0,0);
    return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_RESET, ERR_LEVEL_NORMAL);
}


/*
函 数 名：PAX_Home
功    能：设置轴回零
参    数：ulAxisNO --轴编号
          iHomeFlag --回零标记，0-回零停止，1-回零开始
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：本函数回零停止执行成功表示回零停止指令成功发送，不表示回零动作已经停止，从回零停止指令成功发送到回零动作完全停止有一个过程；
*/
uBit32 PAX_Home(uBit32 ulAxisNO, Bit32 iHomeFlag, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_HOME,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_HOME,ERR_LEVEL_NORMAL);

    if (m_DeviceState[ulAxisNO].AxisStateData.ulRunningStatus&AXIS_STATUS_ENABLE)
    {
        ulRet = DEV_SendCmd(ulAxisNO,DEV_CMD_SET_HOME,iHomeFlag,0,iWaitExeResult);
        return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_HOME,ERR_LEVEL_NORMAL);
    }

    return DEV_GetError(DVM_ERR_MOTOR_DISABLE, ulAxisNO, DVM_CMD_HOME,ERR_LEVEL_NORMAL);
}

/*
函 数 名：PAX_LockForIPO
功    能：轴锁定（标记轴是否正在进行插补运动）
参    数：ulAxisNO --轴编号
          iLockFlag --轴锁定标记，0-解锁，1-锁定
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：插补器启动时需要锁定轴，插补器停止后需要解锁轴
*/
uBit32 PAX_LockForIPO(uBit32 ulAxisNO, Bit32 iLockFlag, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_LOCK_AXIS,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_LOCK_AXIS,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulAxisNO]].BusDevDrv.pf_DEV_LockForIPO(ulAxisNO,iLockFlag,iWaitExeResult);
    return DEV_GetError(ulRet,ulAxisNO,DVM_CMD_LOCK_AXIS,ERR_LEVEL_NORMAL);
}

/*
函 数 名：PAX_SetCmdPos
功    能：设置轴指令位置
参    数：ulAxisNO --轴编号
          lCmdPos --指令位置，单位：um（旋转轴为0.001度）
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_SetCmdPos(uBit32 ulAxisNO, Bit32 lCmdPos, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_CMDPOS,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_CMDPOS,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulAxisNO,DEV_CMD_SET_CMD_POS, lCmdPos,0,iWaitExeResult);
    return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_SET_AXIS_CMDPOS,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：PAX_SetQeiPos
功    能：设置编码器位置
参    数：ulAxisNO --轴编号
          lQeiPos --编码器位置
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_SetQeiPos(uBit32 ulAxisNO, Bit32 lQeiPos, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_QEIPOS,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_QEIPOS,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulAxisNO,DEV_CMD_SET_QEI_POS, lQeiPos, 0, iWaitExeResult);
    return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_SET_AXIS_QEIPOS,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：PAX_Verify
功    能：设置轴校验模式
参    数：ulAxisNO --轴编号
          ulVerifyFlag --校验模式标记，0-关闭，1-开启
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_Verify(uBit32 ulAxisNO, uBit32 ulVerifyFlag, Bit32 iWaitExeResult)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_VERIFY,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_SET_AXIS_VERIFY,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulAxisNO, DEV_CMD_SET_VERIFY_MODE, ulVerifyFlag, 0, iWaitExeResult);
    return DEV_GetError(ulRet, ulAxisNO, DVM_CMD_SET_AXIS_VERIFY,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：PAX_GetStateData
功    能：获取轴状态
参    数：ulAxisNO --轴编号
          *pAxisStateData --轴状态数据（出参）
返 回 值：成功返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 PAX_GetStateData(uBit32 ulAxisNO, AXIS_STATE_DATA* pAxisStateData)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_GET_AXIS_STATE,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_GET_AXIS_STATE,ERR_LEVEL_NORMAL);

    *pAxisStateData = m_DeviceState[ulAxisNO].AxisStateData;

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：PAX_GetCtrlStatus
功    能：获取轴控制状态字
参    数：ulAxisNO --轴编号
返 回 值：轴控制状态字
调用位置：
注意事项：
*/
uBit32 PAX_GetCtrlStatus(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.ulCtrlStatus;
}

/*
函 数 名：PAX_GetRunningStatus
功    能：获取轴运行状态字
参    数：ulAxisNO --轴编号
返 回 值：运行状态字
调用位置：
注意事项：
*/
uBit32 PAX_GetRunningStatus(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.ulRunningStatus;
}

/*
函 数 名：PAX_GetCmdPos
功    能：获取轴指令位置
参    数：ulAxisNO --轴编号
返 回 值：轴指令位置
调用位置：
注意事项：
*/
double64 PAX_GetCmdPos(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.dCmdPos;
}

/*
函 数 名：PAX_GetPlusPos
功    能：获取轴脉冲位置
参    数：ulAxisNO --轴编号
返 回 值：轴脉冲位置
调用位置：
注意事项：
*/
double64 PAX_GetPlusPos(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.dPulsePos;
}

/*
函 数 名：PAX_GetQeiPos
功    能：获取轴编码器位置
参    数：ulAxisNO --轴编号
返 回 值：轴编码器位置
调用位置：
注意事项：
*/
Bit32 PAX_GetQeiPos(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.lQeiPos;
}

/*
函 数 名：PAX_GetCmdSpeed
功    能：获取轴指令速度
参    数：ulAxisNO --轴编号
返 回 值：轴指令速度
调用位置：
注意事项：
*/
float32 PAX_GetCmdSpeed(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.dCmdSpeed;
}

/*
函 数 名：PAX_GetQeiSpeed
功    能：获取轴编码器速度
参    数：ulAxisNO --轴编号
返 回 值：轴编码器速度
调用位置：
注意事项：
*/
float32 PAX_GetQeiSpeed(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.dQeiSpeed;
}

/*
函 数 名：PAX_GetAccSpeed
功    能：获取轴加速度
参    数：ulAxisNO --轴编号
返 回 值：轴加速度
调用位置：
注意事项：
*/
float32 PAX_GetAccSpeed(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.dAcc;
}

/*
函 数 名：PAX_GetSvStatus
功    能：获取伺服系统状态
参    数：ulAxisNO --轴编号
返 回 值：伺服系统状态
调用位置：
注意事项：

uBit32 PAX_GetSvStatus(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.ulSvStatus;
}*/

/*
函 数 名：PAX_GetSvAlarm
功    能：获取伺服报警状态
参    数：ulAxisNO --轴编号
返 回 值：伺服报警状态
调用位置：
注意事项：

uBit32 PAX_GetSvAlarm(uBit32 ulAxisNO)
{
    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulAxisNO].AxisStateData.ulSvAlarm;
}*/

/*
函 数 名：PAX_GetSvParm
功    能：获取伺服参数
参    数：ulAxisNO        --轴编号
                nParmNo    --伺服参数编号
                pParmData    --保存获取的参数
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 PAX_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8 *pParmData)
{
    uBit32 ulRet;

    if (ulAxisNO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_GET_SV_PARM,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulAxisNO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulAxisNO,DVM_CMD_GET_SV_PARM,ERR_LEVEL_NORMAL);

    ulRet = m_RegBusInfo[m_DevNoBusIndexMap[ulAxisNO]].BusDevDrv.pf_DEV_GetSvParm(ulAxisNO, nParmNo,pParmData);
    return DEV_GetError(ulRet,ulAxisNO,DVM_CMD_GET_SV_PARM,ERR_LEVEL_NORMAL);
}

//----------------------------------------------IO相关操作接口----------------------------------------------
/*
函 数 名：IO_SetOutputState
功    能：设置输出IO状态
参    数：ulIONO --IO设备编号
          ulState --IO输出状态
          ulMask  --IO输出状态位掩码
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 IO_SetOutputStatus(uBit32 ulIONO, uBit32 ulStatus, uBit32 ulMask)
{
    uBit32 ulRet;

    if (ulIONO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_SET_IO_OUTPUT,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO, DVM_CMD_SET_IO_OUTPUT,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulIONO, DEV_CMD_SET_OUTPUT, ulStatus, ulMask, 0);
    return DEV_GetError(ulRet, ulIONO, DVM_CMD_SET_IO_OUTPUT,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：IO_SetOutPutPWMMode
功    能：设置输出点位PWM模式
参    数：ulIONO --IO设备编号
          iIOBitNO --IO点编号，取值从1开始
          iMode --IO模式，0-普通模式，1-PWM模式
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 IO_SetOutPutPWMMode(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iMode)
{
    uBit32 ulRet;

    if (ulIONO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_SET_IO_PWM_MODE,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO, DVM_CMD_SET_IO_PWM_MODE,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulIONO, DEV_CMD_SET_IO_PWM_MODE, iIOBitNO, iMode, 0);
    return DEV_GetError(ulRet, ulIONO, DVM_CMD_SET_IO_PWM_MODE,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：IO_SetOutPutPWMDuty
功    能：设置PWM模式IO点的占空比
参    数：ulIONO --IO设备编号
          iIOBitNO --IO点编号，取值从1开始
          iDuty --占空比，该变量值 = 实际占空比 * 1000，目的是为了用整数传递浮点数据
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 IO_SetOutPutPWMDuty(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iDuty)
{
    uBit32 ulRet;

    if (ulIONO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_SET_IO_PWM_DUTY,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO, DVM_CMD_SET_IO_PWM_DUTY,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulIONO, DEV_CMD_SET_IO_PWM_DUTY, iIOBitNO, iDuty, 0);
    return DEV_GetError(ulRet, ulIONO, DVM_CMD_SET_IO_PWM_DUTY,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：IO_SetOutPutPWMFreq
功    能：设置PWM模式IO点的频率
参    数：ulIONO --IO设备编号
          iIOBitNO --IO点编号，取值从1开始
          iFreq --频率
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 IO_SetOutPutPWMFreq(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iFreq)
{
    uBit32 ulRet;

    if (ulIONO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_SET_IO_PWM_FREQ,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO, DVM_CMD_SET_IO_PWM_FREQ,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulIONO, DEV_CMD_SET_IO_PWM_FREQ, iIOBitNO, iFreq, 0);
    return DEV_GetError(ulRet, ulIONO, DVM_CMD_SET_IO_PWM_FREQ,ERR_LEVEL_NORMAL);
    //return ulRet;
}

/*
函 数 名：IO_GetStateData
功    能：获取IO设备状态
参    数：ulIONO --IO设备编号
          *pIOStateData --IO设备状态（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 IO_GetStateData(uBit32 ulIONO, IO_STATE_DATA* pIOStateData)
{
    if (ulIONO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_GET_IO_STATE,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulIONO,DVM_CMD_GET_IO_STATE,ERR_LEVEL_NORMAL);

    *pIOStateData = m_DeviceState[ulIONO].IOStateData;

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：IO_GetOutputStatus
功    能：获取IO输出状态
参    数：ulIONO --IO设备编号
返 回 值：IO输出状态
调用位置：
注意事项：
*/
uBit32 IO_GetOutputStatus(uBit32 ulIONO)
{
    if (ulIONO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulIONO].IOStateData.nOutputStatus;
}

/*
函 数 名：IO_GetInputStatus
功    能：获取IO输入状态
参    数：ulIONO --IO设备编号
返 回 值：IO输入状态
调用位置：
注意事项：
*/
uBit32 IO_GetInputStatus(uBit32 ulIONO)
{
    if (ulIONO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulIONO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    return m_DeviceState[ulIONO].IOStateData.nInputStatus;
}


/*
函 数 名：ADDA_SetDAState
功    能：设置DA输出状态
参    数：ulADDANO --ADDA设备编号
          iDAChNO --DA通道号，取值从1开始
          iDAVaule --DA值
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 ADDA_SetDAStatus(uBit32 ulADDANO, uBit32 iDAChNO, uBit32 iDAVaule)
{
    uBit32 ulRet;

    if (ulADDANO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulADDANO,DVM_CMD_SET_ADDA_DA,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulADDANO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulADDANO, DVM_CMD_SET_ADDA_DA,ERR_LEVEL_NORMAL);

    ulRet = DEV_SendCmd(ulADDANO, DEV_CMD_SET_OUTPUT, iDAChNO, iDAVaule, 0);
    return DEV_GetError(ulRet, ulADDANO, DVM_CMD_SET_ADDA_DA,ERR_LEVEL_NORMAL);
}

/*
函 数 名：ADDA_GetStateData
功    能：获取ADDA设备状态
参    数：ulADDANO --ADDA设备编号
          *pADDAStateData --ADDA设备状态（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 ADDA_GetStateData(uBit32 ulADDANO, ADDA_STATE_DATA* pADDAStateData)
{
    if (ulADDANO>=m_uCodedDevCount)//无效编号
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulADDANO,DVM_CMD_GET_ADDA_STATE,ERR_LEVEL_NORMAL);

    if (m_DevNoBusIndexMap[ulADDANO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return DEV_GetError(DVM_ERR_UNKNOW_DEV,ulADDANO,DVM_CMD_GET_ADDA_STATE,ERR_LEVEL_NORMAL);

    *pADDAStateData = m_DeviceState[ulADDANO].ADDAStateData;

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：ADDA_GetADStatus
功    能：获取AD输入状态
参    数：ulADDANO --ADDA设备编号
          iADChNO --AD通道号
返 回 值：AD输入状态
调用位置：
注意事项：
*/
uBit32 ADDA_GetADStatus(uBit32 ulADDANO, uBit32 iADChNO)
{
    if (ulADDANO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulADDANO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    if (iADChNO>=AD_CHANNEL_COUNT)
        return 0;

    return m_DeviceState[ulADDANO].ADDAStateData.nADStatus[iADChNO];
}

/*
函 数 名：ADDA_GetDAStatus
功    能：获取DA输出状态
参    数：ulADDANO --ADDA设备编号
          iDAChNO --DA通道号
返 回 值：DA输出状态
调用位置：
注意事项：
*/
uBit32 ADDA_GetDAStatus(uBit32 ulADDANO, uBit32 iDAChNO)
{
    if (ulADDANO>=m_uCodedDevCount)//无效编号
        return 0;

    if (m_DevNoBusIndexMap[ulADDANO]==DEV_INVALID_BUS_INDEX)//不属于任何总线
        return 0;

    if (iDAChNO>=DA_CHANNEL_COUNT)
        return 0;

    return m_DeviceState[ulADDANO].ADDAStateData.nDAStatus[iDAChNO];
}