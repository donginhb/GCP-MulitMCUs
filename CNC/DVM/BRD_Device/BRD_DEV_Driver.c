//#include "stdafx.h"
//#include "version.h"
#include <string.h>
#include "BRD_AxisDev.h"
#include "BRD_DEV_Driver.h"
#include "BRD_DEV_Interface.h"
#include "CNC/ErrorCodeDef/DVM_ErrorCodeDef.h"

#define BRD_DEV_ID_BASE     (0XF0000000)
#define BRD_AXIS_DEV_COUNT  (1)
#define BRD_IO_DEV_COUNT    (0)
#define BRD_ADDA_DEV_COUNT  (0)
#define BRD_DEV_INVALID_INDEX   (0XFF)

#define BRD_DEV_COUNT   (BRD_AXIS_DEV_COUNT+BRD_IO_DEV_COUNT+BRD_ADDA_DEV_COUNT)

DEVICE_INFO m_pBrdDevInfo[BRD_DEV_COUNT] = {0}; //板载设备资源信息
uBit8 m_pBrdLastCmdType[BRD_DEV_COUNT] = {0};   //上次指令代码
uBit8 m_pBrdLastCmdRes[BRD_DEV_COUNT] = {0};    //上次指令代码执行结果

uBit8 m_uBrdDevLockFlag = 0;                    //设备锁定标志
uBit8 m_pBrdDevNoIndexMap[DEV_MAX_COUNT] = {0}; //设备编号映射表，存放设备编号对应的设备序号，0XFF表示无效序号
uBit8 m_uBrdCodedDevCount=0;
uBit8 m_uBrdFeedFlag=0;

//------------------------CAN总线驱动数据定义---------------------------------------------------
SOFTWARE_VERSION            m_sBrdBusDriverSoftVer;             //板载设备驱动库软件版本
const AXIS_PARM             *m_pBrdDevAxisParm = NULL;          //电机控制参数数据区
const AXIS_PITCH_CMP_PARM   *m_pBrdDevAxisPitchCmpParm = NULL;  //电机螺距补偿参数数据区
const SYS_CTRL_PARM         *m_pBrdSysCtrlParm=NULL;            //系统控制参数，设置系统控制参数时更新
DEVICE_STATE                *m_pBrdDeviceState = NULL;          //设备状态数据区
DEVICE_INFO                 *m_pBrdDeviceInfo = NULL;           //当前系统设备资源信息数据区
uBit8                       m_uBrdExsitDevCount = 0;            //系统已经存在的设备数量(上次扫描系统所有设备数量)


//----------------------------------------------------------------------------------------------

#ifdef RS_MONITOR_ENALBE
BUS_RS_MS_DATA m_sBrdBusRsMsData;    //

BUS_RS_MS_DATA BRD_DEV_GetBusRsMsData(void)
{
    return m_sBrdBusRsMsData;
}
#endif


/*
函 数 名：BRD_DEV_BusInit
功    能：板载设备驱动初始化
参    数：iBusNO --总线编号
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时调用，只调用一次
注意事项：
*/
uBit32 BRD_DEV_BusInit(uBit32 ulBusChMask)
{
    int i;
    
#ifdef RS_MONITOR_ENALBE
    memset(&m_sBrdBusRsMsData, 0, sizeof(m_sBrdBusRsMsData));
#endif

    m_uBrdDevLockFlag = 0;
    memset(m_pBrdLastCmdType, 0, BRD_DEV_COUNT*sizeof(uBit8));
    memset(m_pBrdLastCmdRes, 0, BRD_DEV_COUNT*sizeof(uBit8));

    memcpy(m_sBrdBusDriverSoftVer.cType, "BRD0", 4);
    m_sBrdBusDriverSoftVer.cSoftVer[0] = 1;
    m_sBrdBusDriverSoftVer.cSoftVer[1] = 0;
    m_sBrdBusDriverSoftVer.cPart = 10;
    m_sBrdBusDriverSoftVer.cFlag = 'T';

    m_pBrdDevAxisParm = NULL;                //电机控制参数数据区
    m_pBrdDevAxisPitchCmpParm = NULL;        //电机螺距补偿参数数据区
    m_pBrdDeviceState = NULL;                //设备状态数据区
    m_pBrdSysCtrlParm = NULL;
    m_pBrdDeviceInfo    = NULL;                //设备资源信息数据区
    m_uBrdExsitDevCount = 0;                //最大设备资源信息数量
    m_uBrdCodedDevCount = 0;

    //初始化板载设备-------------------------------------------------
    for (i=0;  i<BRD_AXIS_DEV_COUNT;  i++)
    {
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusType = DEV_BUS_TYPE_ONBOARD;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDevieID = BRD_DEV_ID_BASE+m_uBrdExsitDevCount;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusNO = 0;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceType = DEV_TYPE_AXIS;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNO = DEV_INVALID_NO;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNOLast = DEV_INVALID_NO;   
        m_uBrdExsitDevCount++;
    }
    
    for(i=0; i<BRD_IO_DEV_COUNT; i++)
    {
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusType = DEV_BUS_TYPE_ONBOARD;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDevieID = BRD_DEV_ID_BASE+m_uBrdExsitDevCount;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusNO = 0;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceType = DEV_TYPE_IO;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNO = DEV_INVALID_NO;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNOLast = DEV_INVALID_NO; 
        m_pBrdDevInfo[m_uBrdExsitDevCount].IOResource.nInputCount = 32;
        m_pBrdDevInfo[m_uBrdExsitDevCount].IOResource.nOutputCount = 32;   
        m_pBrdDevInfo[m_uBrdExsitDevCount].IOResource.nPWMOutPutStartPos = 0;
        m_pBrdDevInfo[m_uBrdExsitDevCount].IOResource.ulPWMOutputMask = 0;         
        m_uBrdExsitDevCount++;      
    }
    
    for(i=0; i<BRD_ADDA_DEV_COUNT; i++)
    {
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusType = DEV_BUS_TYPE_ONBOARD;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDevieID = BRD_DEV_ID_BASE+m_uBrdExsitDevCount;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nBusNO = 0;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceType = DEV_TYPE_IO;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNO = DEV_INVALID_NO;
        m_pBrdDevInfo[m_uBrdExsitDevCount].nDeviceNOLast = DEV_INVALID_NO; 
        m_pBrdDevInfo[m_uBrdExsitDevCount].ADDAResource.nADCount = 0;
        m_pBrdDevInfo[m_uBrdExsitDevCount].ADDAResource.nDACount = 2;   
        m_pBrdDevInfo[m_uBrdExsitDevCount].ADDAResource.nDABitLen = 10;
        m_pBrdDevInfo[m_uBrdExsitDevCount].ADDAResource.nADBitLen = 0;         
        m_uBrdExsitDevCount++;      
    }
 
    for(i=0; i<DEV_MAX_COUNT; i++)
    {
      m_pBrdDevNoIndexMap[i] = BRD_DEV_INVALID_INDEX;
    }

    BRD_AxisInit();
    
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：BRD_DEV_SetConfig
功    能：配置设备编号
参    数：pDeviceConfig --设备配置数据存储地址
          ulDeviceCount --要配置的设备数量
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount)
{
    uBit8 i;
    uBit8 uDevIndex;
    DEVICE_STATE *pDevState;
    const AXIS_PARM   *pAxisParm;
    
    //设备空闲检查
    if(m_uBrdDevLockFlag)
        return DVM_ERR_BUS_BUSY;
    
    //清除编号->序号映射表
    for(i=0; i<DEV_MAX_COUNT; i++)
    {
        m_pBrdDevNoIndexMap[i] = BRD_DEV_INVALID_INDEX;
    }
    
    m_uBrdCodedDevCount = 0;
    
    for (uDevIndex=0; uDevIndex<ulDeviceCount; uDevIndex++, pDeviceConfig++)
    {
        //检查设备是否属于该总线设备-----------
        for (i=0; i<BRD_DEV_COUNT; i++)
        {
            if (m_pBrdDevInfo[i].nDevieID != pDeviceConfig->nDevieID)
                continue;
            
            m_uBrdCodedDevCount++;
            m_pBrdDevNoIndexMap[pDeviceConfig->nDeviceNO] = i; 
            m_pBrdDevInfo[i].nDeviceNO = pDeviceConfig->nDeviceNO;
            m_pBrdDevInfo[i].nDeviceNOLast = m_pBrdDevInfo[i].nDeviceNO;
            
            if (m_pBrdDevInfo[i].nDeviceType==DEV_TYPE_AXIS || m_pBrdDevInfo[i].nDeviceType==DEV_TYPE_SV)
            {
                //m_pBrdDeviceState[pDeviceConfig->nDeviceNO].AxisStateData.ulRunningStatus |= AXIS_STATUS_ZERO_SPEED|AXIS_STATUS_FSTOP;
                //设置轴控制参数                
                BRD_AxisSetAxisParm(m_pBrdDevAxisParm + pDeviceConfig->nDeviceNO);
                //更新轴插补周期                
                BRD_AxisSetSysCtrlParm(m_pBrdSysCtrlParm);
                //重新设置指令位置                
                pDevState = m_pBrdDeviceState + pDeviceConfig->nDeviceNO;
                pAxisParm = m_pBrdDevAxisParm + pDeviceConfig->nDeviceNO; 
                BRD_AxisGetAxisState(&pDevState->AxisStateData);
                
                pDevState->AxisStateData.dPulsePos = pDevState->AxisStateData.lQeiPos*pAxisParm->AxisScaleParm.lInternalPulseScale/pAxisParm->AxisScaleParm.lQEIPulseScale;
                pDevState->AxisStateData.dCmdPos = pDevState->AxisStateData.dPulsePos*pAxisParm->AxisScaleParm.lGearRatioPosScale/pAxisParm->AxisScaleParm.lGearRatioPlusScale;
            }      
            
            break;
        }
    }
    
    
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：BRD_DEV_SetAxisParmReadAddr
功    能：设置轴参数读取起始地址
参    数：pAxisParm --轴参数读取起始地址
返 回 值：无
调用位置：
注意事项：发送轴进给指令前必须先设置轴参数
*/
void BRD_DEV_SetAxisParmReadAddr(const AXIS_PARM* pAxisParm)
{
    m_pBrdDevAxisParm = pAxisParm;
}

/*
函 数 名：BRD_DEV_SetSysCtrlParmReadAddr
功    能：设置系统控制参数读取起始地址
参    数：pSysCtrlParm --系统控制参数读取起始地址
返 回 值：无
调用位置：
注意事项：发送轴进给指令前必须先设置轴参数
*/
void BRD_DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm)
{
    m_pBrdSysCtrlParm = pSysCtrlParm;
}

/*
函 数 名：BRD_DEV_SetDeviceInfoAddr
功    能：设置设备信息数据的存储地址
参    数：pDeviceInfo    --设备信息数据存储起始地址
          ulLen            --设备信息数据缓冲区长度（表示之前已经扫描到的有效设备信息数量）
返 回 值：无
调用位置：
注意事项：
*/
void BRD_DEV_SetDeviceInfoAddr(DEVICE_INFO* pDeviceInfo, uBit32 ulLen)
{
    m_pBrdDeviceInfo = pDeviceInfo;
    m_uBrdExsitDevCount = ulLen;
}

/*
函 数 名：BRD_DEV_SetDeviceStateAddr
功    能：设置设备状态数据的存储地址
参    数：pDeviceState --设备状态数据存储起始地址
          ulLen --设备状态数据缓冲区长度（表示可存放多少个设备的状态数据）
返 回 值：无
调用位置：
注意事项：
*/
void BRD_DEV_SetDeviceStateAddr(DEVICE_STATE* pDeviceState, uBit32 ulLen)
{
    m_pBrdDeviceState = pDeviceState;
    //m_uMaxDevStateCount = ulLen;
}

//-------------------------------------------获取硬件信息---------------------------------------------------
/*
函 数 名：BRD_DEV_Scan
功    能：扫描总线上的设备
参    数：
          pDeviceInfo            --设备硬件信息存储缓冲区起始地址
          ulDevBufLen            --设备硬件信息存储缓冲区长度（表示缓存区最大可以存放多少个设备的硬件信息）
          *pRecordDevCount      --已经记录到硬件信息缓冲区的设备数量（出参）
          *pTotalDevCount        --扫描发现的设备总数量（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_Scan(DEVICE_INFO* pDeviceInfo, uBit32 ulDevBufLen, uBit32* pRecordDevCount, uBit32* pTotalDevCount)
{
    //检查总线空闲
    if (BRD_DEV_IsBusFree()==0)
        return DVM_ERR_BUS_BUSY;
    
    if(ulDevBufLen<BRD_DEV_COUNT)
    {
        *pRecordDevCount = ulDevBufLen;     
    }
    else
    {
        *pRecordDevCount = BRD_DEV_COUNT;
    }
    
    *pTotalDevCount = BRD_DEV_COUNT;  
    memcpy(pDeviceInfo, m_pBrdDevInfo, sizeof(DEVICE_INFO)*(*pRecordDevCount));
    return DVM_ERR_SUCCESS;
}

//-------------------------------------------执行控制指令---------------------------------------------------

/*
函 数 名：BRD_DEV_SendFeed
功    能：设置轴进给
参    数：ulAxisNO --轴编号(设备编号)
         IPOAxisState --插补状态数据
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState)
{
    uBit32 ulRet;
    
    DEVICE_STATE *pAxisState;
    
    pAxisState = m_pBrdDeviceState + ulAxisNO;
    pAxisState->AxisStateData.dCmdPos = IPOAxisState.dCmdPos;
    pAxisState->AxisStateData.dPulsePos = IPOAxisState.dPulsePos;
    pAxisState->AxisStateData.dCmdSpeed = IPOAxisState.dCmdSpeed;
    pAxisState->AxisStateData.dAcc = IPOAxisState.dAcc;
    pAxisState->AxisStateData.lPinc = IPOAxisState.lPinc;
    
    if (pAxisState->AxisStateData.ulRunningStatus&AXIS_STATUS_VERIFY)//校验模式
        pAxisState->AxisStateData.lPinc = 0;
    
    ulRet = BRD_AxisSetFeed(pAxisState->AxisStateData.lPinc);
    
    if(ulRet==DVM_ERR_SUCCESS)
    {
        m_uBrdFeedFlag=2;//连续两个周期没有进给则检查精确停止信号，并清除指令速度
        pAxisState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_ZERO_SPEED;
        pAxisState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FSTOP;  
    }
    
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：BRD_DEV_SetCmd
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
注意事项：内部限制同一总线上的设备同一时间只有一条控制指令在执行,如果是设置IO输出，则直接设置设备状态信息中的指令输出后
          返回，如果是设置ADDA的DA输出，则先判断该是否正在设置DA值，如果是则设置指令位置后清除DA的脉冲输出控制
*/
uBit32 BRD_DEV_SetCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult)
{
    //uBit32 ulRet;
    //DEVICE_INFO *pDevInfo;
    uBit8 uDevIndex;
    DEVICE_STATE *pDevState;
    const AXIS_PARM *pAxisParm;  
    
    uDevIndex = m_pBrdDevNoIndexMap[ulDevNO];
    
    //设备有效性检查
    if(uDevIndex==BRD_DEV_INVALID_INDEX)
        return DVM_ERR_UNKNOW_DEV;
    
    //检查设备是否锁定
    if(m_uBrdDevLockFlag&(1<<uDevIndex))
        return DVM_ERR_BUS_BUSY;
    
    //Alan???添加设备指令处理代码
    m_pBrdLastCmdType[uDevIndex] = ulCmdType;
    
    
    //设置轴的指令位置
    if (ulCmdType==DEV_CMD_SET_CMD_POS)
    {
        pDevState = m_pBrdDeviceState + ulDevNO;
        
        if (m_pBrdDevInfo[uDevIndex].nDeviceType!=DEV_TYPE_AXIS &&
            m_pBrdDevInfo[uDevIndex].nDeviceType!=DEV_TYPE_SV )//非轴设备的设置指令位置
        {
            m_pBrdLastCmdRes[uDevIndex] = DVM_ERR_UNKNOW_CMD;
            return DVM_ERR_UNKNOW_CMD;
        }
        
        //修改轴指令位置
        pAxisParm = m_pBrdDevAxisParm + ulDevNO;
        pDevState->AxisStateData.dCmdPos = lCmdPara1;
        //修改轴脉冲指令位置
        pDevState->AxisStateData.dPulsePos= lCmdPara1 * pAxisParm->AxisScaleParm.lGearRatioPlusScale / pAxisParm->AxisScaleParm.lGearRatioPosScale;
        m_pBrdLastCmdRes[uDevIndex] = BRD_AxisSetCmd(ulCmdType,(long)pDevState->AxisStateData.dPulsePos,lCmdPara2);
        return m_pBrdLastCmdRes[uDevIndex];           
    }    
    
    switch(m_pBrdDevInfo[uDevIndex].nDeviceType)
    {
    case DEV_TYPE_AXIS:
    case DEV_TYPE_SV:      
        m_pBrdLastCmdRes[uDevIndex] = BRD_AxisSetCmd(ulCmdType, lCmdPara1,lCmdPara2);  
        break;
    default:
        m_pBrdLastCmdRes[uDevIndex] = DVM_ERR_UNKNOW_CMD;
        break;
    }
    
    return m_pBrdLastCmdRes[uDevIndex];   
}


/*
函 数 名：BRD_DEV_CheckFree
功    能：检测设备是否空闲（没有指令在执行）
参    数：ulDevNO --设备编号
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 BRD_DEV_CheckFree(uBit32 ulDevNO)
{
    //检查设备是否锁定
    if(m_uBrdDevLockFlag&(1<<m_pBrdDevNoIndexMap[ulDevNO]))
        return 0;

    return 1;
}

/*
函 数 名：BRD_DEV_IsBusFree
功    能：检查总线是否空闲(总线所有有效通道都空闲，且没有要等待的指令回包，没有正在发生数据)
参    数：
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 BRD_DEV_IsBusFree(void)        
{
    if(m_uBrdDevLockFlag)
        return 0;

    return 1;
}

/*
函 数 名：BRD_DEV_LockForIPO
功    能：锁定(解锁)总线设备
参    数：
        ulDevNo            ---设备编号
        ulLockFlag        ---锁定标志 0-解锁 1-锁定
        ulWaitExeResult    ---是否获取回应包再返回 0-立即返回 1-获取回应包再返回
返 回 值：空闲返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_LockForIPO(uBit32 ulDevNo, uBit32 ulLockFlag, uBit32 ulWaitExeResult)
{
    if (ulLockFlag==1)    //锁定    
    {
        m_uBrdDevLockFlag |= (1<<m_pBrdDevNoIndexMap[ulDevNo]);
    }
    else    //解锁    
    {
        m_uBrdDevLockFlag &= ~(1<<m_pBrdDevNoIndexMap[ulDevNo]);
    }
    
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：BRD_DEV_CheckLastCmdExeState
功    能：检测上一次的指令执行状态
参    数：ulDevNO --设备编号
          *pCmdType --指令类型（出参）
返 回 值：执行成功返回0，否则返回错误码（执行失败、正在执行）
调用位置：
注意事项：
*/
uBit32 BRD_DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType)
{
    *pCmdType = m_pBrdLastCmdType[m_pBrdDevNoIndexMap[ulDevNO]];
    return m_pBrdLastCmdRes[m_pBrdDevNoIndexMap[ulDevNO]];
}

//-------------------------------------------设置参数---------------------------------------------------
/*
函 数 名：BRD_DEV_SetParm
功    能：设置设备参数
参    数：ulDevNO --设备编号
          ulParmType --参数类型，取值为为：
                        DEV_PARM_TYPE_SV                        //伺服参数
                        DEV_PARM_TYPE_UPDATE_APP                //更新应用程序数据
                        DEV_PARM_TYPE_UPDATE_BOOT                //更新BOOTLOADER数据
                        DEV_PARM_TYPE_AXIS_PARM                    //发送轴控参数
                        DEV_PARM_TYPE_SYS_CTRL_PARM                //发送系统控制参数
          pParmBuf --参数数据缓冲区
          ulParmLen --参数数据长度
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_SetParm(uBit32 ulDevNO, uBit32 ulParmType, uBit8* pParmBuf, uBit32 ulParmLen)
{
    uBit32 ulRet;
    uBit8 uDevIndex;
    
    if (BRD_DEV_CheckFree(ulDevNO)==0)
        return DVM_ERR_BUS_BUSY;
    
    uDevIndex = m_pBrdDevNoIndexMap[ulDevNO];
    
    if(m_pBrdDevInfo[uDevIndex].nDeviceType!=DEV_TYPE_AXIS)
        return DVM_ERR_UNKNOW_CMD;    
    
    switch(ulParmType)
    {
    case DEV_PARM_TYPE_AXIS_PARM:
        {
            BRD_AxisSetAxisParm((AXIS_PARM*)pParmBuf);
            //Alan???添加重新设置指令位置及脉冲指令位置代码            
            ulRet = 0;
        }
        break;
    case DEV_PARM_TYPE_SYS_CTRL_PARM:
        {
            ulRet = BRD_AxisSetSysCtrlParm((SYS_CTRL_PARM*)pParmBuf);
        }
        break;
    default:
        ulRet = DVM_ERR_UNKNOW_CMD;
        break;
    }
    
    return ulRet;
}

//-------------------------------------------服务程序---------------------------------------------------
/*
函 数 名：BRD_DEV_SynCycleService
功    能：同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：在插补中断中调用
注意事项：
*/
uBit32 BRD_DEV_SynCycleService(void)
{
    //Alan???添加刷新设备状态代码
    uBit8 i=0;
    uBit8 uCount=0;
    DEVICE_STATE *pDevState;
    
    for(i=0; i<DEV_MAX_COUNT; i++)
    {
        if(m_pBrdDevNoIndexMap[i]==BRD_DEV_INVALID_INDEX)
            continue;
        
        pDevState = m_pBrdDeviceState + i;
        
        switch(m_pBrdDevInfo[m_pBrdDevNoIndexMap[i]].nDeviceType)
        {
        case DEV_TYPE_AXIS:
        case DEV_TYPE_SV:   
            {
                BRD_AxisGetAxisState(&pDevState->AxisStateData);
                
                if(m_uBrdFeedFlag==0)
                {
                    //检查停止信号
                    if(pDevState->AxisStateData.ulRunningStatus & AXIS_STATUS_ZERO_SPEED)
                    {
                        pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FSTOP;
                        pDevState->AxisStateData.dAcc = 0;
                        pDevState->AxisStateData.dCmdSpeed = 0;
                    }else
                    {
                        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FSTOP;              
                    }
                }else
                {
                    m_uBrdFeedFlag--;            
                }
                break;
            }
        default:
            break;
        }
        
        uCount++;
        
        if(uCount>=m_uBrdCodedDevCount)
            break;
    }
    
    return DVM_ERR_SUCCESS;
}


/*
函 数 名：BRD_DEV_AsynCycleService
功    能：非同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：(大循环中调用)
注意事项：
*/
uBit32 BRD_DEV_AsynCycleService(void)
{
    //Alan???添加刷新设备状态代码
    return DVM_ERR_SUCCESS;
}


//-------------------------------------------获取状态---------------------------------------------------

/*
函 数 名：BRD_DEV_GetSvParm
功    能：获取伺服参数
参    数：ulAxisNO        --设备编号
                nParmNo        --伺服参数编号
                pParmData  --保存获取的伺服参数
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8* pParmData)
{
    return DVM_ERR_UNKNOW_CMD;
}

//-------------------------------------------获取版本信息---------------------------------------------------
/*
函 数 名：BRD_DEV_GetVersion
功    能：获取设备软件版本信息
参    数：ulDevNO --设备编号
          *pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion)
{
    return DVM_ERR_UNKNOW_CMD;
}


/*
函 数 名：BRD_DEV_GetDriverVersion
功    能：获取驱动库版本信息
参    数：*pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 BRD_DEV_GetDriverVersion(SOFTWARE_VERSION* pVersion)
{
    *pVersion = m_sBrdBusDriverSoftVer;
    return DVM_ERR_SUCCESS;
}


void BRD_DEV_GetBusDriver(BUS_DEVICE_DRIVE *pDeviceDrive)
{
    pDeviceDrive->pf_DEV_BusInit                = &BRD_DEV_BusInit              ;
    pDeviceDrive->pf_DEV_IsBusFree              = &BRD_DEV_IsBusFree            ;
    pDeviceDrive->pf_DEV_SetConfig              = &BRD_DEV_SetConfig            ;
    pDeviceDrive->pf_DEV_SetAxisParmReadAddr    = &BRD_DEV_SetAxisParmReadAddr  ;
    pDeviceDrive->pf_DEV_SetSysCtrlParmReadAddr = &BRD_DEV_SetSysCtrlParmReadAddr  ;
    pDeviceDrive->pf_DEV_SetDeviceInfoAddr      = &BRD_DEV_SetDeviceInfoAddr    ;
    pDeviceDrive->pf_DEV_SetDeviceStateAddr     = &BRD_DEV_SetDeviceStateAddr   ;
    pDeviceDrive->pf_DEV_Scan                   = &BRD_DEV_Scan                 ;
    pDeviceDrive->pf_DEV_SendFeed               = &BRD_DEV_SendFeed             ;
    pDeviceDrive->pf_DEV_SetCmd                 = &BRD_DEV_SetCmd               ;
    pDeviceDrive->pf_DEV_CheckFree              = &BRD_DEV_CheckFree            ;
    pDeviceDrive->pf_DEV_LockForIPO             = &BRD_DEV_LockForIPO           ;
    pDeviceDrive->pf_DEV_CheckLastCmdExeState   = &BRD_DEV_CheckLastCmdExeState ;
    pDeviceDrive->pf_DEV_SetParm                = &BRD_DEV_SetParm              ;
    pDeviceDrive->pf_DEV_SynCycleService        = &BRD_DEV_SynCycleService      ;
    pDeviceDrive->pf_DEV_AsynCycleService       = &BRD_DEV_AsynCycleService     ;
    pDeviceDrive->pf_DEV_GetSvParm              = &BRD_DEV_GetSvParm            ;
    pDeviceDrive->pf_DEV_GetVersion             = &BRD_DEV_GetVersion           ;
    pDeviceDrive->pf_DEV_GetDriverVersion       = &BRD_DEV_GetDriverVersion     ;
  //pDeviceDrive->pf_DEV_GetBusRsMsData         = &BRD_DEV_GetBusRsMsData       ;
    
} 

