/*    
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：Can_DataStructDef.h

摘    要：     本文件是关于插补器和轴/IO模块之间数据通信的CAN数据包的定义，具体包括各种不同情况下CAN
           数据包的ID数据位的定义，及其CAN数据的数据结构的定义，本文件的前半部分的标题1-2是关于CAN
           数据包从插补器到轴/IO模块之间的相关数据定义，3-4是关于CAN数据包从轴/IO模块到插补器之间的
           相关数据定义，本文的相关数据结构没有经过会议讨论且并非专业人员不要轻易改动，否则会导致系统
           运行混乱。

运行环境：Windows2000/Xp/Win7/WinCe6.0 

修改描述：

当前版本：1.0

修改作者：4367 4232 3183 0562 855

修改了内容：

修改日期：

原 作 者：毛军

完成日期：2014年11月12日        
*/
#ifndef _CAN_DATA_STRUCT_DEF_H_
#define _CAN_DATA_STRUCT_DEF_H_

#include "CNC/DataStructDef/DataType.h"

#pragma pack(push)
#pragma pack(8)        


#define ENABLE_EX_PULSE_BIT                            //定义拓展脉冲数据位宽使能

#define CAN_DEV_MAX_COUNT                (8)            //CAN总线（单个通道上的）设备的最大数量
#define CAN_CHANNEL_MAX_COUNT            (2)            //CAN通道的最大数量
#define CAN_DATA_PACK_MAX_LEN            (8)            //CAN数据包数据区最大长度


//定义通信接收模块屏蔽字
#define CAN_MAINCTRL_RECEIVE            (0)            //主控接收帧
#define CAN_DEVICE_RECEIVE                (1)            //设备端接收帧

//定义CAN数据包类型
#define CAN_MSG_TYPE_PLUSE                (0)            //脉冲指令数据包
#define CAN_MSG_TYPE_CMD                (1)            //控制命令数据包
#define CAN_MSG_TYPE_PARM                (2)            //参数数据包


//定义脉冲指令包附加控制指令类型
#define CAN_ADD_CMD_COUNT                (8)    //附加指令种类数量
#define CAN_ADD_CMD_NULL                (0)    //无附加指令
#define CAN_ADD_CMD_FIL                    (1)    //CCW转矩限制
#define CAN_ADD_CMD_RIL                    (2)    //CW转矩限制
#define CAN_ADD_CMD_CLR_ALM                (3)    //报警清除
#define CAN_ADD_CMD_ENABLE_SV            (4)    //使能电机
#define CAN_ADD_CMD_SET_HOME            (5)    //回零
#define CAN_ADD_CMD_SET_IO_PWM_MODE        (6)    //设置IO进入PWM模式
#define CAN_ADD_CMD_SET_IO_PWM_DUTY        (7)    //设置IO进入PWM占空比
#define CAN_ADD_CMD_SET_IO_PWM_FREQ        (8)    //设置IO进入PWM频率


//----------------------------------------轴进给脉冲/IO输出指令数据包数据结构定义------------------------------------------
#define MAINCTRL_PULSE_PACK_MAX_LIMIT  (512)        //进给指令最大值


//轴进给脉冲/IO输出指令数据包ID数据结构体
typedef struct _MAINCTRL_PULSE_PACK_ID_STR{
    uBit32 ulMsgType:        3;    //CAN数据包类型
    uBit32 ulCmdExist:        1;  //标记是否有附带控制指令，0-无控制指令，1-有控制指令
    uBit32 ulCmdType:        5;    //附带控制指令类型

    uBit32 ulData09:        8;    //当没有附带控制指令、且接收设备为轴时，用于定义轴脉冲进给的第09位数据，第1位对应第0轴，第8位对应第7轴；
                                //当没有附带控制指令、且接收设备为IO时，ulData09和ulData10标示输出IO组序号
                                //当没有附带控制指令、且接收设备为ADDA时，ulData09和ulData10标示DA通道号
                                //当有附带控制指令时，ulData09和ulData10中对应的位组2个BIT的数据，表示控制指令数据序号

    uBit32 ulData10:        8;    //当没有附带控制指令、且接收设备为轴时，用于定义轴的方向信号，第一位对应第0轴，第二位对应第7轴；0：表示正方向，1：表示负方向
                                //当没有附带控制指令、且接收设备为IO时，ulData09和ulData10标示输出IO组序号
                                //当没有附带控制指令、且接收设备为ADDA时，ulData09和ulData10标示DA通道号
                                //当有附带控制指令时，ulData09和ulData10中对应的位组2个BIT的数据，表示控制指令数据序号
    uBit32 ulDevComNO:        3;  //设备通信编号
                                //当有附带控制指令时，该编号表示接收控制指令的设备通信编号，也是发送应答包的设备通信编号
                                //当没有附带控制指令时，该编号表示发送应答包的设备通信编号
    uBit32 ulReceiveObj:    1;    //接收屏蔽字
    uBit32 ulReserve:        3;    //保留
}MAINCTRL_PULSE_PACK_ID;


//----------------------------------------设备控制指令数据包数据结构定义------------------------------------------
//控制命令数据包ID数据结构体
typedef struct _MAINCTRL_CMD_PACK_ID_STR {
    uBit32 ulMsgType:            3;    //CAN数据包类型
    uBit32 ulCmdType:            8;  //指令类型，取值见轴/IO设备指令列表      5
    uBit32 ulResData:           14;    //预留
     uBit32 ulDevComNO:            3;  //设备通信编号   
     uBit32 ulReceiveObj:        1;    //接收屏蔽字   
    uBit32 ulReserve:            3;    //Bit29-Bit31 预留
}MAINCTRL_CMD_PACK_ID;


//控制命令数据包数据区结构
typedef union
{
    struct _SET_NO                //设置设备编号指令数据区结构定义
    {
        uBit32 ulID;            //设备ID
        uBit16 nDeviceNO;        //设备编号
#ifdef ENABLE_EX_PULSE_BIT
        uBit8 uAxisCount;        //总线上轴数量
        uBit8 uPulseBitLen;        //轴脉冲数据位宽(数据区位宽，轴脉冲数据的总位宽=2+轴脉冲数据位宽)
#else
        uBit16 nReserve;
#endif
    }SetNO;

    struct _SET_IO_PWM_DUTY     //
    {
        uBit32 ulDuty;            //占空比，ulDuty = 实际占空比 * 1000
        uBit32 ulReserve;        //预留
    }SetIoPwmDuty;

    struct _SET_IO_PWM_FREQ
    {
        uBit32 ulFreq;            //频率
        uBit32 ulReserve;        //预留
    }SetIoPwmFreq;

    uBit32 ulData[2];

}MAINCTRL_CMD_PACK_DATA;


//----------------------------------------设备参数数据包数据结构定义-----------------------------------------

//定义参数数据传输帧的状态（MAINCTRL_PARM_PACK_ID_STR.ulFrameStatus）
#define CAN_TRANS_SINGLE_FRAME        (00)        //独立发送帧
#define CAN_TRANS_BEGIN_FRAME         (01)        //该文件开始传送（第一帧）
#define CAN_TRANS_MIDDLE_FRAME         (02)        //该文件传送中
#define CAN_TRANS_VERIFY_FRAME        (03)        //校验帧(不包含正文)

#define CAN_SEND_PARA_FRAME_INDEX_MAX  (255)    //最大发送CAN帧的ID顺序号，与ulFrameIndexBit21-Bit28 定义的位数密切相关

typedef struct MAINCTRL_PARM_PACK_ID_STR {  
    uBit32 ulMsgType:        3;    //CAN包类型
    uBit32 ulParaType:        8;    //参数数据的类型，取值见设备参数类型列表    
    uBit32 ulFrameStatus:    2;  //数据包的传输状态
    uBit32 ulFrameIndex:    8;    //定义参数指令数据包时存放参数指令包的顺序，0-255;
    uBit32 ulResData:        4;    //预留
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //接收模块屏蔽字,该位为0--主控接收，1--轴端接收
    uBit32 ulReserve:        3;    //Bit29-bi31预留
}MAINCTRL_PARM_PACK_ID;


//----------------------------------------主控数据包数据结构定义----------------------------------------------

//主控数据包ID结构体定义
typedef union 
{
    uBit32                        ulFrameID;        //报文ID
    MAINCTRL_PULSE_PACK_ID        PulseID;        //脉冲指令数据包
    MAINCTRL_CMD_PACK_ID        CmdID;            //命令控制数据包
    MAINCTRL_PARM_PACK_ID        ParmID;            //参数数据包
}MAINCTRL_FRAME_ID;


//主控数据包数据区结构体定义
typedef union 
{
    uBit8                        cDataBuf[CAN_DATA_PACK_MAX_LEN];    //报文数据场以8位无符号字符型数据定义
    uBit32                        ulDataBuf[2];                        //报文数据场以32位无符号长整型定义
    MAINCTRL_CMD_PACK_DATA        CmdData;                            //控制指令数据
}MAINCTRL_FRAME_DATA;


//主控数据包结构体
typedef struct _MAINCTRL_COM_DATA_PACK {

    MAINCTRL_FRAME_ID        ID;        //报文ID
    MAINCTRL_FRAME_DATA        Data;    //数据区

}MAINCTRL_COM_DATA_PACK;


//---------------------------------------------设备输出指令（轴脉冲指令/IO输出指令）应答包数据结构定义-------------------------


//轴脉冲指令回应包ID定义
typedef struct _AXIS_PULSE_ANS_ID_STR_        
{
    uBit32 ulMsgType:        3;    //用于存放CAN包类型
    uBit32 ulCmdExist:        1;  //标记是否有附带控制指令，0-无控制指令，1-有控制指令
    uBit32 ulCheckSun:        8;  //存放指令位置校验和
    uBit32 ulQeiSpeedDir:   1;    //编码器速度方向，0-正方向，1-负方向
    uBit32 ulQeiSpeed:       10; //编码器速度值
    uBit32 ulSvDataFlag:    2;    //伺服状态数据标志，0:表示传轴状态，1:表示传伺服状态，2：表示传伺服报警
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //定义接收模块屏蔽字,该位为0--主控接收，1--轴端接收
    uBit32 ulReserve:        3;    //Bit29-Bit31 预留
}AXIS_PULSE_ANS_ID;

//IO脉冲指令回应包ID定义
typedef struct  _IO_PULSE_ANS_ID_STR_
{
    uBit32 ulMsgType:        3;    //用于存放CAN包类型
    uBit32 ulCmdExist:        1;  //标记是否有附带控制指令，0-无控制指令，1-有控制指令
    uBit32 ulSysStatus:     8;  //IO设备系统状态
    uBit32 ulResData:       13;    //预留
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //定义接收模块屏蔽字,该位为0--主控接收，1--轴端接收
    uBit32 ulReserve:        3;    //Bit29-Bit31 预留
}IO_PULSE_ANS_ID;

//ADDA脉冲指令回应包ID定义
typedef struct _ADDA_PULSE_ANS_ID_STR_
{
    uBit32 ulMsgType:        3;    //用于存放CAN包类型
    uBit32 ulCmdExist:        1;  //标记是否有附带控制指令，0-无控制指令，1-有控制指令
    uBit32 ulADCh1:            3;  //反馈的第一个AD参数对应的通道索引
    uBit32 ulADCh2:            3;  //反馈的第二个AD参数对应的通道索引
    uBit32 ulDACh1:            3;  //反馈的第一个DA参数对应的通道索引
    uBit32 ulDACh2:            3;  //反馈的第一个DA参数对应的通道索引
    uBit32 ulSysStatus:        8;  //ADDA设备系统状态
    uBit32 ulResData:        1;    //预留
      uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //定义接收模块屏蔽字,该位为0--主控接收，1--轴端接收
    uBit32 ulReserve:        3;    //Bit29-Bit31 预留
}ADDA_PULSE_ANS_ID;

//带附加指令的脉冲指令回应包定义--目前只有轴设备和IO设备有可能有附加指令回包
typedef struct _DEV_PULSE_ADD_CMD_ANS_ID_STR_
{
    uBit32 ulMsgType:        3;    //用于存放CAN包类型
    uBit32 ulCmdExist:        1;  //标记是否有附带控制指令，0-无控制指令，1-有控制指令
    uBit32 ulCmdType:        5;    //附带控制指令类型
    uBit32 ulDataIndex:        2;    //控制指令数据序号
    uBit32 ulExeResult:        8;    //指令执行结果
    uBit32 ulResData:        6;    //预留
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //定义接收模块屏蔽字,该位为0--主控接收，1--轴端接收
    uBit32 ulReserve:        3;    //Bit29-Bit31 预留
}DEV_PULSE_ADD_CMD_ANS_ID;

//轴进给脉冲/IO输出指令应答包ID定义
typedef union 
{
    AXIS_PULSE_ANS_ID            AxisPulseAnsID;
    IO_PULSE_ANS_ID                IOPulseAnsID;
    ADDA_PULSE_ANS_ID            ADDAPulseAnsID;
    DEV_PULSE_ADD_CMD_ANS_ID    AddCmdAnsID;
}DEV_PULSE_ANS_PACK_ID;


//轴进给脉冲/IO输出指令应答包数据区结构体
#define AXIS_PULSE_AXIS_STATUS        (0)
#define AXIS_PULSE_SV_STATUS        (1)
#define AXIS_PULSE_SV_ALARM            (2)
typedef union  
{
    struct _AXIS_PULSE_ANS_DATA 
    {
        uBit32 ulStatus;        //轴状态ID中ulSvDataFlag决定数据类型：0表示轴状态，1表示伺服状态，2表示伺服报警
        Bit32  lQeiPos;            //轴编码器位置
    }AxisPulseAnsData;

    struct _IO_PULSE_ANS_DATA
    {
        uBit32 ulOutputStatus;  //输出状态
        uBit32 ulInputStatus;   //输入状态
    }IOPulseAnsData;

    struct _ADDA_PULSE_ANS_DATA
    {
        uBit16 nADCh1;            //第一个AD值
        uBit16 nADCh2;          //第二个AD值
        uBit16 nDACh1;            //第一个DA值
        uBit16 nDACh2;          //第二个DA值
    }ADDAPulseAnsData;
}DEV_PULSE_ANS_PACK_DATA;


//---------------------------------------------设备控制指令应答包数据结构定义----------------------------------

//设备控制指令应答包ID数据结构体
typedef struct DEV_CMD_ANS_PACK_ID_STR {
    uBit32 ulMsgType:        3;    //CAN包类型
    uBit32 ulCmdType:        8;    //指令类型，取值见轴/IO设备指令列表
    uBit32 ulExeResult:        8;    //执行结果
    uBit32 ulDevType:        3;    //设备类型
    uBit32 ulResData:        3;    //预留
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //接收模块屏蔽字    
    uBit32 ulReserve:        3;    //Bit29-Bit31 预留
}DEV_CMD_ANS_PACK_ID;


//设备控制指令应答包数据区结构体
typedef union
{
    struct _INQ_INFO_AXIS_ANS_DATA
    {
        uBit32 ulID;                //设备ID
        uBit16 ulDevNO;                //设备上一次的编号
        uBit16 ulReserve;            //预留
    }InqInfoAxisAnsData;

    struct _INQ_INFO_IO_ANS_DATA
    {
        uBit32 ulID;                //设备ID
        uBit8  uDevNo;              //设备上一次的编号
        uBit8  uCountInfo;          //bit0-bit3 4位表示输入点组数，8个输入点为一组 Modified by Alan at 2015-10-15
                                          //bit4-bit7 4位表示输出点组数，8个输出点为一组
        uBit8 nPWMOutPutStartPos;    //可作为PWM输出的输出点起始位置
        uBit8 ulPWMOutputMask;        //可作为PWM输出的输出点从nPWMOutPutStartPos开始的掩码
    }InaInfoIOAnsData;

    struct _INQ_INFO_ADDA_ANS_DATA
    {
        uBit32 ulID;                    //设备ID
        uBit8 uDevNo;             //设备上一次的编号
        uBit8 uCountInfo;        //bit0-bit3 4位表示DA数量Modified by Alan at 2015-10-15
                                        //bit4-bit7 4位表示AD数量
        uBit8 nDABitLen;            //DA位长
        uBit8 nADBitLen;            //AD位长
    }InaInfoADDAAnsData;

    struct _INQ_VER_ANS_DATA
    {
        Bit8    cType[4];            //产品型号
        Bit8    cSoftVer[2];        //软件版本
        Bit8    cPart;                //组件号
        Bit8    cFlag;                //软件版本特殊标示
    }InqVerAnsData;

    struct _AXIS_CMD_ANS_DATA
    {
        uBit32 ulStatus;           //轴状态
        Bit32  lQeiPos;               //轴编码器位置
    }AxisCmdAnsData;

    struct _IO_CMD_ANS_DATA
    {
        uBit32 ulOutputStatus;     //输出状态
        uBit32 ulInputStatus;      //输入状态
    }IOCmdAnsData;

    struct _ADDA_CMD_ANS_DATA
    {
        uBit8 uDAChNo;
        uBit8 uADChNo;
        uBit8 uSysStatus;
        uBit8 uReserver;
        uBit16 uDAStatus;
        uBit16 uADStatus;
    }ADDACmdAnsData;
}DEV_CMD_ANS_PACK_DATA;


//---------------------------------------------设备参数传输应答包数据结构定义----------------------------------

//设备参数传输应答包ID数据结构体
typedef struct _DEV_PARM_ANS_PACK_ID_STR {  
    uBit32 ulMsgType:        3;    //CAN包类型
    uBit32 ulParaType:        8;    //参数数据的类型，取值见设备参数类型列表
    uBit32 ulExeResult:        8;    //执行结果
    uBit32 ulResData:        6;    //预留
    uBit32 ulDevComNO:        3;  //设备通信编号
    uBit32 ulReceiveObj:    1;    //接收模块屏蔽字
    uBit32 ulReserve1:        3;    //Bit29-Bit31 预留
}DEV_PARM_ANS_PACK_ID;



//----------------------------------------------设备数据包数据结构定义------------------------------------------

//设备发送数据包ID数据
typedef union 
{
    uBit32                        ulFrameID;                    //报文ID
    DEV_PULSE_ANS_PACK_ID       PulseAnsID;        //脉冲指令数据包
    DEV_CMD_ANS_PACK_ID         CmdAnsID;            //命令控制数据包
    DEV_PARM_ANS_PACK_ID        ParmAnsID;        //参数数据包
}DEVICE_FRAME_ID;


//设备发送数据包数据区
typedef union 
{
    uBit8                        cDataBuf[CAN_DATA_PACK_MAX_LEN];    //报文数据场以8位无符号字符型数据定义
    uBit32                        ulDataBuf[2];                        //报文数据场以32位无符号长整型定义
    DEV_PULSE_ANS_PACK_DATA        PulseAnsData;                        //脉冲/IO输出指令应答包数据
    DEV_CMD_ANS_PACK_DATA        CmdAnsData;                            //控制指令应答包数据
}DEVICE_FRAME_DATA;


//设备发送数据包数据结构体
typedef struct _DEV_COM_DATA_PACK
{
    DEVICE_FRAME_ID            ID;            //报文ID
    DEVICE_FRAME_DATA        Data;        //数据区
}DEV_COM_DATA_PACK;

#pragma pack(pop)

#endif