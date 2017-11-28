#ifndef _CAN_DEV_COM_MANA_H_
#define _CAN_DEV_COM_MANA_H_

#include "CAN_DEV_DataStructDef.h"
#include "CAN_COM_DataStructDef.h"
#include "..\\DEV_DriveStructDef.h"
#include "..\\..\\DataStructDef\\SYS_DataStructDef.h"

//#include "..\\..\\CMU\\UIN_GlobalDataDef.h"
#include "..\\..\\..\\SysPeripheral\\CAN\\CanDataStructDef.h"


//-------------------CAN总线驱动数据结构定义---------------------------------------------
#define DVM_CH0_MASK        (0x01)    //CAN0通道屏蔽字
#define DVM_CH1_MASK        (0x02)    //CAN1通道屏蔽字


//CAN总线设置指令数据-----------------------------------------------
#define CMD_EXE_STEP_END        (0)//执行结束(接收到指令回应包后置位，并将执行结果保存到设置指令数据区)
#define CMD_EXE_STEP_NOT_START    (1)//未开始执行
#define CMD_EXE_STEP_BEING        (2)//执行中(指令尚未执行完)
#define CMD_EXE_STEP_WAIT_RES    (3)//正在等待执行结果


#define CAN_GETRES_MAX_TIME            (2)//回应包超时时间
#define CAN_CLRBUS_MAX_TIME         (0)//收光总线CAN数据包超时时间  
#define CAN_SCAN_TIME_OUT            (10)//设备扫描超时时间
#define CAN_FLASH_TIME_OUT          (200)//flash操作超时时间(设置编号、升级数据包、升级BOOTLOADER数据包等)
#define CAN_DEV_UNRES_MAX_TIME    (2)//设备连续无应答次数




typedef struct _can_set_cmd_data_
{
    uBit8 uDevNo;                    
    uBit8 uCmdType;                                            //指令类型
    uBit8 uExeStep;                                            //执行步 0--执行结束 1-未执行 2-执行中 3-等待执行结果
    uBit32 ulExeRes;                                            //执行结果,成功、错误码
    double dStartTick;                                        //指令开始执行时间
    
    //附加指令控制
    uBit8 uAddMaxIndex;                                        //有效数据区字节个数 0-表示1个字节 3-表示四个字节
    uBit8 uAddPreIndex;                                        //附加指令上次发送的序号
    uBit8 uAddCmdType;                                        //附加指令类型
    uBit8 uAddComNo;                                        //附加指令通信编号
    uBit8 pAddCmdData[4];                                    //附加指令数据
}CAN_SET_CMD_DATA;
//----------------------------------------------------------

//CAN总线设备编号映射数据定义-------------------------------
typedef struct _com_no_map_data_
{
    uBit8 uBusChIndex:    1;    //总线通道序号0-CAN0 1-CAN1
    uBit8 uComNo:    7;            //通道通信编号
}COM_NO_MAP_DATA;
//----------------------------------------------------------

//CAN总线通道设备数据定义-------------------------------
typedef struct _ch_pulse_data_ctrl_words_
{
    uBit8 nData9:1;            //对于轴，表示轴脉冲bit8数据，对于IO标示上次发送的序号位，对应ADDA标示上次发送的通道编号序号位
    uBit8 nData10:1;        //对于轴，表示轴脉冲bit9数据，即脉冲方向，对于IO标示上次发送的序号位，对应ADDA标示上次发送的通道编号序号位
    uBit8 nDAHBit:1;        //只对ADDA有效，标示上次发送的ADDA数据高位标示
    uBit8 nFeedFlag:2;        //只对轴有效，本次有无进给标志，设置进给时设置为2，发送刷新轴状态时清除
    uBit8 nTick:3;            //通道设备无响应次数，发送脉冲指令包时++，接收脉冲回包时清零        
}PL_CTRL_WORDS;


//CAN总线通道控制数据定义-----------------------------------
typedef struct _canbus_ch_data
{
    uBit8     bValidFlag;                                    //通道有效标志，总线初始化时根据通道屏蔽字置位或清零
    uBit8     bRsAllowFlag;                                //外部收发数据标志，由中断服务函数置位，外部接口调用清零
    uBit8     bPreRsAllowFlag;                            //上次外部允许标志
    uBit8     bRsAllowReqFlag;                            //外部发送数据请求标志，由外部置位，中断清0
    uBit8     uNextResponseComNo;                            //下次回应设备通信编号
    uBit8     uCurResponseComNo;                            //本次应该回应的设备通信编号

    uBit8    uCodedDevCount;                                //通道已编号设备总数,设置硬件配置信息时设置
    uBit8    uDevLockBit;                                //轴锁定位bit0-表示通信编号0锁定状态

#ifdef ENABLE_EX_PULSE_BIT
    //总线扫描时设置，用于拓展轴脉冲位宽
    uBit8    uMotorDevCount;                                //总线上轴设备总数
    uBit8    uPulseBitLen;                                //轴脉冲数据位宽(轴脉冲数据位于CAN数据包数据区位宽，总位宽由该值+2)
    uBit32    ulPulseBitMask;                                //轴数据位屏蔽字
    uBit32    ulMaxPulseInc;                                //单次最大发送脉冲数
#endif

    CAN_SET_CMD_DATA    sLastSetCmdData;                        //上次设置指令数据，新的有效指令来时设置
    DEVICE_INFO            *pDevInfo[CAN_DEV_MAX_COUNT];            //设备通信编号对应的设备信息，pDevInfo[0]表示通信编号0所对应设备信息，
                                                                //设置设备硬件配置信息成功后设置
    PL_CTRL_WORDS        pPulseCtrlWords[CAN_DEV_MAX_COUNT];     //通道设备脉冲通信控制数据
    Bit32                pAxisSentPulse[CAN_DEV_MAX_COUNT];        //轴已发送脉冲数，用于指令位置校验,设置硬件配置信息时清0，轴复位时清0
    double64            pAxisVerifyCmdPos[CAN_DEV_MAX_COUNT];    //校验指令位置，设置校验模式前记录，退出校验模式后恢复
    double64            pAxisScaleRatio[CAN_DEV_MAX_COUNT];        //电子此轮比 单位：用户单位/QEI脉冲，设置轴控参数成功后设置，初始化为1.0
}CANBUS_CH_DATA;
//----------------------------------------------------------------------------------------------


//------------------------CAN总线驱动数据定义---------------------------------------------------
extern SOFTWARE_VERSION                m_sCanBusDriverSoftVer;                //CAN总线驱动库软件版本
extern const AXIS_PARM                *m_pCanDevAxisParm ;                //电机控制参数数据区
extern const AXIS_PITCH_CMP_PARM    *m_pCanDevAxisPitchCmpParm;            //电机螺距补偿参数数据区

extern DEVICE_STATE                    *m_pCanDeviceState;                    //设备状态数据区
//extern uBit8                        m_uMaxDevStateCount;                //最大设备状态数据数量

extern DEVICE_INFO                    *m_pCanDeviceInfo;                    //设备资源信息数据区
extern uBit8                        m_uCanExsitDevCount;                //最大设备资源信息数量

extern COM_NO_MAP_DATA                m_pComNoMap[DEV_MAX_COUNT];            //保存设备编号对应的通信编号，pDevNoMap[0]保存设备1对应的通信编号（从0开始）
extern CANBUS_CH_DATA                m_pChData[CAN_CHANNEL_MAX_COUNT];    //通道数据

extern CAN_INTERFACE m_sCanComInterface;
extern CAN_DEV_EX_INTERFACE m_sCanDevExInterface;

#ifdef RS_MONITOR_ENALBE
extern BUS_RS_MS_DATA m_sCanBusRsMsData;
#endif


//----------------------------------------------------------------------------------------------


//协议层内部调用函数-----------------------

//数据管理
void SetBusDevNoToDefault(void);                                        //设置总线设备编号为默认值0(未编号)
void SetConfigDataToDefault(void);                                        //设置配置信息为默认值
//BooLean IsDevExist(uBit32 ulDevID, DEVICE_INFO* pDevInfo, uBit8 uLen);        //检查设备ID是否存在

BooLean IsDevLocked(uBit8 uBusChIndex, uBit8 cComNo);                            //检查设备是否锁定
BooLean IsChLocked(uBit8 uBusChIndex);                                            //通道是否锁定
uBit32 AskForRsPermit(uBit8 uBusChIndex);                                    //外部请求通道发送权限
void ReleaseRsPermit(uBit8 uBusChIndex);                                    //外部接口释放收发权限

//接收数据包处理
BooLean IsRequiredRes(uBit32 ulSendID, uBit32 ulRcvID);
uBit32  DealWithRcvPack(uBit8 uBusChIndex, CanFrame *pPackData);            //处理接收数据包处理
uBit32  RcvProcess(uBit8 uBusChIndex, uBit8 uTimeLimit);                        //接收数据包处理

//数据发送处理
uBit32 SendSinglePack(uBit8 uBusChIndex, MAINCTRL_COM_DATA_PACK *pSendPack);                        //发送单包数据
uBit32 SendAxisParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen);        //发送轴控制参数
uBit32 SendSysCtrlParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen);    //发送轴系统控制参数
uBit32 SendSevorParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen);        //发送伺服参数
uBit32 SendAppUpdateData(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen); //发送设备升级数据
uBit32 SendBootUpdateData(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen);//发送设备BOOTLOADER升级数据

//指令处理
uBit32 HandleOutputCmd(uBit32 ulDevNO,Bit32 lCmdPara1, Bit32 lCmdPara2);
uBit32 HandleVerifyCmd(uBit32 ulDevNO,Bit32 lCmdPara1);
uBit32 HandleBlinkLedCmd(uBit32 ulDevID, uBit32 ulBusChIndex);
uBit32 HandleAddCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult);


//周期性服务函数
void SetPulseData9(MAINCTRL_PULSE_PACK_ID *pID,  uBit8 uBitIndex, uBit8 uValue);
void SetPulseData10(MAINCTRL_PULSE_PACK_ID *pID, uBit8 uBitIndex, uBit8 uValue);
uBit8 GetPulseIDDataIndex(MAINCTRL_PULSE_PACK_ID *pID, uBit8 uBitIndex); //获取脉冲ID中uData9和uData10代表的序号
uBit8 GetPulseCtrlDataIndex(PL_CTRL_WORDS sPlCtrlWords);                //获取脉冲控制字中uData9和uData10代表的序号
void SynCycleRcv(uBit32 uBusChIndex);            //周期性服务函数中的接收处理
void SynCycleUpdateState(uBit32 uBusChIndex);    //周期性服务函数状态刷新处理
uBit32 SysCycleSend(uBit32 uBusChIndex);        //周期性服务函数中的发送处理
#endif

