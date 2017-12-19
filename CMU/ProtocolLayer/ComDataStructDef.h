#ifndef COM_DATA_STRUCT_DEF_H
#define COM_DATA_STRUCT_DEF_H
#include "../UIN_GlobalDataDef.h"
#include "../../DataType/DataType.h"

#pragma pack(push)
#pragma pack(8)    

/***********************************************ID格式定义**************************************************/
typedef struct _hk_com_id{
    uBit32 ulOperType:              1;//Bit0        数据操作类型    
    //DATA_OPRE_TYPE_SET    设置数据
    //DATA_OPRE_TYPE_GET    获取数据

    uBit32 ulDataType:              3;//Bit1-Bit3 操作数据种类
    //DATA_TYPE_SYS         系统数据
    //DATA_TYPE_CRD         坐标系数据
    //DATA_TYPE_AXIS        轴数据
    //DATA_TYPE_IO          IO数据
    //DATA_TYPE_ADDA        ADDA数据
    //DATA_TYPE_REG         寄存器数据
    //DATA_TYPE_SERVO       伺服驱动器数据

    uBit32 ulCmdType:               1;//Bit4        指令类型        
    //SETCMD_TYPE_NOMAL(0)  常规设置指令
    //SETCMD_TYPE_INQ(1)    周期查询设置指令
    //GETCMD_TYPE_NOMAL(0)  常规获取指令
    //GETCMD_TYPE_INQ(1)    周期新数据获取指令

    uBit32 ulCmdIndex:              6;//Bit5-Bit10 指令序号        
    //系统常规设置指令列表
    //系统周期查询设置指令列表

    //坐标系常规设置指令列表
    //坐标系周期查询设置指令列表

    //轴常规设置指令列表
    //轴周期查询设置指令列表

    //IO常规设置指令列表
    //IO周期查询设置指令列表

    //ADDA常规设置指令列表
    //ADDA周期查询设置指令列表

    uBit32 ulDevNo:                 8;//Bit11-Bit18 设备编号(包括电机编号、IO板编号)、轴号、轴屏蔽字
    uBit32 ulCrdNo:                 4;//Bit19-Bit22 坐标系编号、屏蔽字

    uBit32 ulTransmitFrame:         2;//Bit23-bit24 数据传输帧类别    
    //TRANSMIT_FIRST_FRAME  该文件开始传送（第一帧）
    //TRANSMIT_BEING_FRAME  该文件传送中
    //TRANSMIT_SELF_FRAME   独立发送帧
    //TRANSMIT_VERIFY_FRAME 校验帧

    uBit32 ulPackIndex:             1;//Bit25       数据包序号        取值：0-BLOCK_INDEX_MAX - 1
    uBit32 ulErrFlag:               1;//Bit26       下位机出错标志，下位机收到数据包处理时出错回包时置位
    uBit32 ulPeriodFlag:            1;//Bit27       周期性数据标志，下位机置位
    uBit32 ulReceiveObj:            1;//Bit28       接收模块屏蔽字,该位为0--上位机接收，1--下位机接收
    uBit32 ulSendRetry:             3;//Bit29-Bit31 重发次数(CAN无效)
}HK_COM_ID, *P_HK_COM_ID;


typedef struct _ADDA_COM_ID
{
    uBit32 ulOperType:              1;//Bit0        数据操作类型    DATA_OPRE_TYPE_SET   设置数据
    uBit32 ulDataType:              3;//Bit1-Bit3   操作数据种类    DATA_TYPE_SYS        系统数据
    uBit32 ulCmdType:               1;//Bit4        指令类型        SETCMD_TYPE_NOMAL(0)    常规设置指令
    uBit32 ulCmdIndex:              3;//Bit5-Bit7   指令序号        系统常规设置指令列表
    uBit32 ulADDANo:                8;//Bit8-Bit15  ADDA卡编号
    uBit32 ulChMask:                6;//Bit16-Bit21 通道屏蔽字
    uBit32 ulReverse0:              1;//Bit22       保留
    uBit32 ulTransmitFrame:         2;//Bit23-bit24 数据传输帧类别    TRANSMIT_FIRST_FRAME 该文件开始传送（第一帧）
    uBit32 ulPackIndex:             1;//Bit25       数据包序号    取值：0-BLOCK_INDEX_MAX - 1
    uBit32 ulErrFlag:               1;//Bit26       下位机出错标志，下位机收到数据包处理时出错回包时置位
    uBit32 ulPeriodFlag:            1;//Bit27       周期性数据标志，下位机置位
    uBit32 ulReceiveObj:            1;//Bit28       接收模块屏蔽字,该位为0--上位机接收，1--下位机接收
    uBit32 ulSendRetry:             3;//Bit29-Bit31 重发次数(CAN无效)
}ADDA_COM_ID,*P_ADDA_COM_ID;

typedef struct _REG_COM_ID
{
    uBit32 ulOperType:              1;//Bit0        数据操作类型    DATA_OPRE_TYPE_SET   设置数据
    uBit32 ulDataType:              3;//Bit1-Bit3   操作数据种类    DATA_TYPE_SYS        系统数据
    uBit32 ulCmdType:               1;//Bit4        指令类型        SETCMD_TYPE_NOMAL(0)    常规设置指令
    uBit32 ulCmdIndex:              3;//Bit5-Bit7   指令序号        系统常规设置指令列表
    uBit32 ulRegIndex:              10;//Bit8-Bit17 0-1023
    uBit32 ulRegCount:              5;//Bit18-Bit22 寄存器个数，最大同时可读取31个寄存器数据
    uBit32 ulTransmitFrame:         2;//Bit23-bit24 数据传输帧类别    TRANSMIT_FIRST_FRAME 该文件开始传送（第一帧）
    uBit32 ulPackIndex:             1;//Bit25       数据包序号        取值：0-BLOCK_INDEX_MAX - 1
    uBit32 ulErrFlag:               1;//Bit26       下位机出错标志，下位机收到数据包处理时出错回包时置位
    uBit32 ulPeriodFlag:            1;//Bit27       周期性数据标志，下位机置位
    uBit32 ulReceiveObj:            1;//Bit28       接收模块屏蔽字,该位为0--上位机接收，1--下位机接收
    uBit32 ulSendRetry:             3;//29-Bit31    重发次数(CAN无效)
}REG_COM_ID, *P_REG_COM_ID;

typedef union _COM_DATA_ID{
    HK_COM_ID    ulComDataID;
    ADDA_COM_ID  ulADDADataID;
    REG_COM_ID   ulRegDataID;
    uBit32      ulFrameID;
}COM_DATA_ID,*P_COM_DATA_ID;

#define SAME_TYPE_PACK_MASK                 (0X007FFFFF)//同类型数据包屏蔽字，BIT0-BIT22(该字段是固定不变数据，可确定唯一数据)

//ulOperType:Bit0 数据操作类型定义----------------------------------
#define DATA_OPRE_TYPE_SET                  (0)     //设置指令
#define DATA_OPRE_TYPE_GET                  (1)     //查询指令

//ulDataType:Bit1-Bit3 操作数据种类----------------------------------
#define DATA_TYPE_SYS                       (0)     //系统数据
#define DATA_TYPE_CRD                       (1)     //坐标系数据
#define DATA_TYPE_AXIS                      (2)     //轴数据
#define DATA_TYPE_IO                        (3)     //IO数据
#define DATA_TYPE_ADDA                      (4)     //ADDA数据
#define DATA_TYPE_SERVO                     (5)     //伺服数据
#define DATA_TYPE_REG                       (6)     //寄存器数据
#define DATA_TYPE_CUSTOM                    (7)     //自定义指令


//ulCmdType:Bit4 指令类型---------------------------------------------        
#define SETCMD_TYPE_NOMAL                   (0)     //常规设置指令
#define SETCMD_TYPE_INQ                     (1)     //周期查询设置指令
#define GETCMD_TYPE_NOMAL                   (0)     //常规获取指令
#define GETCMD_TYPE_INQ                     (1)     //周期性数据获取指令


//ulCmdIndex:Bit5-Bit10 指令序号--------------------------------------
/*****************************系统数据操作指令序号**************************/
//系统数据常规设置指令
#define SYS_SETCMD_COM_STYLE                (1)     //通信类型
#define SYS_SETCMD_BLINK_DEV_LED            (2)     //测试板卡
#define SYS_SETCMD_DEVICE_CONF              (3)     //设置硬件配置
#define SYS_SETCMD_CTRL_PARM                (4)     //设置系统控制参数
#define SYS_SETCMD_UPDATE_SLC               (7)     //SLC升级数据
#define SYS_SETCMD_UPDATE_IPO               (8)     //IPO升级数据
#define SYS_SETCMD_UPDATE_DEV               (9)     //设备升级数据
#define SYS_SETCMD_UPDATE_SLCIPO_BOOT       (10)    //SLCIPObootloader升级数据
#define SYS_SETCMD_UPDATE_DEV_BOOT          (11)    //设备bootloader升级数据
#define SYS_SETCMD_IPR_ENABLE               (12)    //使能解释器
#define SYS_SETCMD_DEV_SCAN                 (13)    //系统扫描
#define SYS_SETCMD_SAVE_PARM                (14)    //控制器保存控制参数
#define SYS_SETCMD_SAVE_CONF                (15)    //控制器保存配置信息
#define SYS_SETCMD_SYS_RESET                (16)    //系统复位    
#define SYS_SETCMD_MACHINE_TYPE             (17)    //设备类型          //AutoSale

//2017.12.19 新增 Duhanfeng
#define SYS_SETCMD_APP_FLAG_CLEAR           (20)    //APP存在标志清除
#define SYS_SETCMD_SUB_APP_FLAG_CLEAR       (21)    //副APP存在标志清除


//系统数据常规获取指令(获取无周期性数据区的数据)
#define SYS_GETCMD_SLC_VER                  (1)     //获取SLC版本
#define SYS_GETCMD_IPO_VER                  (2)     //获取IPO版本
#define SYS_GETCMD_DEVICE_VER               (3)     //获取AXIS\IO\ADDA版本
#define SYS_GETCMD_BUS_DRV_VER              (4)     //获取AXIS\IO\ADDA版本
#define SYS_GETCMD_DEVICE_COUNT             (5)     //获取设备总数
#define SYS_GETCMD_DEVICE_INFO              (6)     //获取硬件配置
#define SYS_GETCMD_CTRL_PARM                (7)     //获取系统控制数据
#define SYS_GETCMD_CTRL_RS_MS_DATA          (8)     //获取主控通信监测数据
#define SYS_GETCMD_CSM_VER                  (9)     //获取CSM版本
#define SYS_GETCMD_HQEIPOS                  (10)    //获取高速编码器位置
#define SYS_GETCMD_HQEISPD                  (11)    //获取高速编码器速度

//2017.12.18 新增 Duanfeng
#define SYS_GETCMD_CNC_ALARM                (20)    //获取系统报警

/************************************************************************/


/*****************************坐标系数据操作指令序号**************************/
//坐标系常规设置指令
#define CRD_SETCMD_AXISMAP                  (1)     //设置轴映射表
#define CRD_SETCMD_CTRL_PARM                (2)     //设置坐标系控制参数
#define CRD_SETCMD_RAPID_OVERRIDE           (3)     //设置坐标系快移修调
#define CRD_SETCMD_FEED_OVERRIDE            (4)     //设置坐标系进给修调
#define CRD_SETCMD_SPINDLE_OVERRIDE         (5)     //设置坐标系主轴修调
#define CRD_SETCMD_VERIFY_ENABLE            (6)     //校验运行使能控制
#define CRD_SETCMD_CTRL_MODE                (7)     //设置坐标系工作模式
#define CRD_SETCMD_ESTOP                    (8)     //设置指定的坐标系急停
#define CRD_SETCMD_RESET                    (9)     //坐标系复位
#define CRD_SETCMD_SPINDLE_RUN              (10)    //启动主轴
#define CRD_SETCMD_SPINDLE_STOP             (11)    //停止主轴
#define CRD_SETCMD_PROG_MODE                (12)    //设置程序缓冲区工作模式
#define CRD_SETCMD_CLEAR_PROG_BUF           (13)    //清除坐标系程序缓冲区
#define CRD_SETCMD_SINGLE_SEG_MODE          (14)    //设置系统单段方式
#define CRD_SETCMD_LOOKAHEAD                (15)    //前瞻控制控制
#define CRD_SETCMD_PROG_RUN                 (16)    //运行坐标系程序
#define CRD_SETCMD_PROG_HOLD                (17)    //暂停坐标系程序
#define CRD_SETCMD_PROG_STOP                (18)    //暂停坐标系程序
#define CRD_SETCMD_PROG_CANCEL              (19)    //取消坐标系程序
#define CRD_SETCMD_SEGEDN_STOP              (20)    //段结束停止坐标系程序
#define CRD_SETCMD_USERDATABUF_MODE         (21)    //设置用户缓冲区工作模式
#define CRD_SETCMD_CLEAR_USERDATABUF        (22)    //清除用户自定义数据区
#define CRD_SETCMD_CTRL_PARM_EX             (23)    //设置坐标系控制参数单项数据


#define CRD_SETCMD_LOAD_USERDATA            (30)    //下载用户数据
#define CRD_SETCMD_LOAD_GCODE               (31)    //下载G代码数据
#define CRD_SETCMD_LOAD_CANNDED             (32)    //下载固定循环数据
#define CRD_SETCMD_LOAD_BP_DATA             (33)    //下载断点数据
#define CRD_SETCMD_TOOL_DATA                (34)    //设置刀库数据
#define CRD_SETCMD_CUR_TOOL_NO              (35)    //设置当前刀具
#define CRD_SETCMD_BASE_TOOL_NO             (36)    //设置基准刀具
#define CRD_SETCMD_IPR_RDN_START            (37)    //任意行启动
#define CRD_SETCMD_IPR_SET_COORD            (38)    //重新设置坐标零点


#define CRD_PROG_CMD_BASE                   (50)                        //运动指令起始
#define CRD_PROG_CMD_COUNT                  (11)                        //运动指令总数
#define CRD_SETCMD_PROG_DWELL               (CRD_PROG_CMD_BASE)         //设置坐标系程序延时
#define CRD_SETCMD_M_CODE                   (CRD_PROG_CMD_BASE+1)       //设置坐标系M代码
#define CRD_SETCMD_S_CODE                   (CRD_PROG_CMD_BASE+2)       //设置坐标系S代码
#define CRD_SETCMD_T_CODE                   (CRD_PROG_CMD_BASE+3)       //设置坐标系T代码
#define CRD_SETCMD_B_CODE                   (CRD_PROG_CMD_BASE+4)       //设置坐标系B代码
#define CRD_SETCMD_IO_CODE                  (CRD_PROG_CMD_BASE+5)       //
#define CRD_SETCMD_RAPID                    (CRD_PROG_CMD_BASE+6)       //单个轴或者多个轴同时执行快速移动
#define CRD_SETCMD_LINE                     (CRD_PROG_CMD_BASE+7)       //多轴直线插补运动
#define CRD_SETCMD_ARC_C                    (CRD_PROG_CMD_BASE+8)       //圆弧插补圆心，终点模式
#define CRD_SETCMD_ARC_R                    (CRD_PROG_CMD_BASE+9)       //圆弧插补半径，终点模式
#define CRD_SETCMD_ARC_PT                   (CRD_PROG_CMD_BASE+10)      //圆弧插补中间点、终点模式

//坐标系常规获取指令
#define CRD_GETCMD_AXIS_MAP                 (1)     //获取坐标系映射表
#define CRD_GETCMD_CTRL_PARM                (2)     //获取坐标系控制数据
#define CRD_GETCMD_TOOL_DATA                (3)     //获取刀具数据
#define CRD_GETCMD_UPLOAD_PROG_DATA         (4)     //获取程序数据
#define CRD_GETCMD_UPLOAD_USER_DATA         (5)     //获取用户数据
#define CRD_GETCMD_UPLOAD_GCODE_DATA        (6)     //获取G代码数据
#define CRD_GETCMD_BP_DATA_LEN              (7)     //获取断点数据长度
#define CRD_GETCMD_BP_DATA                  (8)     //获取断点数据
#define CRD_GETCMD_IPR_CUR_LINENO           (9)     //获取当前解释行号
#define CRD_GETCMD_CRD_ZERO                 (10)    //获取坐标系零点
#define CRD_GETCMD_BASE_TOOL                (11)    //获取基本刀具号

/******************************************************************************/


/*****************************轴数据操作指令序号*******************************/
//轴(电机)常规设置指令
#define MOTOR_SETCMD_CTRL_PARM              (1)     //设置电机控制参数
#define MOTOR_SETCMD_PITCH_CMP_PARM         (2)     //设置电机螺距补偿
#define MOTOR_SETCMD_POS_MOTION             (3)     //设置位置控制运动数据
#define MOTOR_SETCMD_SPEED_MOTION           (4)     //设置速度控制运动数据
#define MOTOR_SETCMD_HOME                   (5)     //电机回零
#define MOTOR_SETCMD_STOP                   (6)     //电机停止
#define MOTOR_SETCMD_ESTOP                  (7)     //电机急停
#define MOTOR_SETCMD_CMD_POS                (8)     //设置电机当前位置
#define MOTOR_SETCMD_QEI_POS                (9)     //设置电机编码器当前位置
#define MOTOR_SETCMD_ENABLE                 (10)    //设置伺服使能0-断使能1-加使能
#define MOTOR_SETCMD_CTRL_PARM_EX           (11)    //设置电机控制参数单项数据()
#define MOTOR_SETCMD_RESET                  (12)    //电机复位
#define MOTOR_SETCMD_SV_PARM                (13)    //设置伺服参数

#define MOTOR_SETCMD_AXIS_POS_MOTION        (20)    //设置轴位置模式运动
#define MOTOR_SETCMD_AXIS_SPEED_MOTION      (21)    //设置轴速度模式运动
#define MOTOR_SETCMD_AXIS_HOME              (22)    //设置轴回零
#define MOTOR_SETCMD_AXIS_STOP              (23)    //设置轴停止
#define MOTOR_SETCMD_AXIS_ESTOP             (24)    //设置轴急停
#define MOTOR_SETCMD_AXIS_CMD_POS           (25)    //设置轴指令位置
#define MOTOR_SETCMD_AXIS_QEI_POS           (26)    //设置轴编码器位置
#define MOTOR_SETCMD_AXIS_ENABLE            (27)    //设置轴使能

//常规电机运动控制参数设置 2017.12.11 新增 --- 杜寒枫
//考虑到安卓端调用C结构不便,扩展相关接口,以兼容其他平台
#define MOTOR_SETCMD_BASE_CTRL_PARM         (30)    //设置基本控制参数(暴露给上位机的接口)
#define MOTOR_SETCMD_POS_MOTION_EX          (31)    //设置位置控制运动数据(扩展)  采用整型数据传输代替浮点数据传输,放大因子: 10000
#define MOTOR_SETCMD_SPEED_MOTION_EX        (32)    //设置速度控制运动数据(扩展)  采用整型数据传输代替浮点数据传输,放大因子: 10000
#define MOTOR_SETCMD_PULSE_RATE             (33)    //设置脉冲当量

//轴(电机)常规获取指令
#define MOTOR_GETCMD_CTRL_PARM              (1)     //电机所有控制参数
#define MOTOR_GETCMD_SIGNAL_PARM            (2)     //信号量控制参数
#define MOTOR_GETCMD_SAFE_PARM              (3)     //安全控制参数
#define MOTOR_GETCMD_HOME_PARM              (4)     //回零控制参数
#define MOTOR_GETCMD_SCALE_PARM             (5)     //比例控制参数
#define MOTOR_GETCMD_MOVE_PARM              (6)     //运动控制参数
#define MOTOR_GETCMD_BACKLASH_CMP_PARM      (7)     //反向间隙补偿
#define MOTOR_GETCMD_PITCH_CMP_PARM         (8)     //螺距补偿参数
#define MOTOR_GETCMD_SV_PARM                (9)     //获取伺服参数

//轴(电机)常规获取指令 2017.12.16 新增 --- 杜寒枫
#define MOTOR_GETCMD_RUNNING_STATUS         (20)    //获取电机运行状态
#define MOTOR_GETCMD_CMD_POS                (21)    //获取指令位置(放大因子:10000)



/**************************************************************************/

/*****************************IO操作指令序号*******************************/
//IO常规设置指令
#define IO_SETCMD_STATE                     (1)     //设置IO板输出状态
#define IO_SETCMD_PWM_ENABLE                (2)     //设置IO板PWM功能
#define IO_SETCMD_PWM_DUTY_RATIO            (3)     //设置IO板IO口PWM占空比
#define IO_SETCMD_PWM_FRQ                   (4)     //设置IO板IO口PWM频率
#define IO_SETCMD_HSPD_STATE                (5)     //设置高速IO口输出状态

//以编号的方式对IO进行控制 2017.12.01 新增 -- 杜寒枫
#define IO_SETCMD_HSPD_OUT_IONO             (6)     //设置输出IO状态(编号方式控制)

//IO常规获取指令
#define IO_GETCMD_HSPD_OUT_STATUS           (1)     //获取高速IO输出口状态
#define IO_GETCMD_HSPD_IN_STATUS            (2)     //获取高速IO输入口状态   

//以编号的方式对IO进行控制 2017.12.01 新增 -- 杜寒枫
#define IO_GETCMD_HSPD_OUT_IONO             (3)     //获取输出IO状态(编号方式控制)
#define IO_GETCMD_HSPD_IN_IONO              (4)     //获取输入IO状态(编号方式控制)

/******************************************************************************/

/*****************************DA/AD操作指令序号*******************************/
//DA/AD常规设置指令
#define ADDA_SETCMD_DATA                    (1)     //设置DA板输出
#define ADDA_SETCMD_HDATA                   (2)     //设置高速DA输出
//ADDA常规获取指令
#define ADDA_GETCMD_HDATA                   (1)     //获取高速DA输出值
/******************************************************************************/

/**************************寄存器操作指令序号*******************************/
//常规设置指令
#define REG_SETCMD_DATA                     (1)     //设置寄存器数据
//常规获取指令
#define REG_GETCMD_DATA                     (1)     //获取寄存器数据

/**************************水果机操作指令序号*******************************/
//常规设置指令
#define VM_SETCMD_AISLE_MOTOR               (1)     //设置货道电机出货

//常规查询指令
#define VM_GETCMD_AISLE_MOTOR_STATUS        (1)     //查询货道电机运行状态

/******************************************************************************/


/******************************************************************************/

#define SCR_RECEIVE                         (0)     //人机界面接收帧
#define SLC_RECEIVE                         (1)     //主控接收帧

//数据传输帧类别------------------------------------------------
#define TRANSMIT_FIRST_FRAME                (0)     //该文件开始传送（第一帧）
#define TRANSMIT_BEING_FRAME                (1)     //该文件传送中
#define TRANSMIT_SELF_FRAME                 (2)     //独立发送帧
#define TRANSMIT_VERIFY_FRAME               (3)     //校验帧


#define BLOCK_INDEX_MAX                     (2)     //数据块传送支持的最大数据包索引序号

//周期性数据标志(下位机控制，获取数据时有效)----------------------------------  
#define NOMAL_DATA_FLAGS                    (0)     //表示是查询数据
#define PERIOD_DATA_FLAGS                   (1)     //表示是周期数据，下位机主动发送



/***********************************************ID格式定义结束**************************************************/

//主机发送控制数据(必须等到全部数据发送完成才返回)
typedef struct _COM_MAIN_SEND_CTRL_DATA
{
    uBit16 ulSendLen;           //发送数据长度
    uBit8 *pSendDataBuf;        //发送数据区
    uBit16 ulRebackLen;         //回应数据区长度
    uBit8 *pRebackDataBuf;      //回应数据区
    uBit32 ulErrCode;           //错误编号(保存发送和获取回应过程中错误代码)
    COM_DATA_ID  ulSendID;      //发送数据ID
}COM_MAIN_SEND_CTRL_DATA;

//下位机发送控制数据
typedef struct _COM_SLAVE_SEND_CTRL_DATA
{
    uBit8  *pBuf;               //数据区(保存接收到的完整数据包数据)
    uBit32 ulFreeLen;           //发送缓冲区剩余空间
    uBit32 ulRestLen;           //数据区剩余数据长度
    uBit32 ulSentLen;           //已发送字节个数
    uBit8 cCheckNum;            //校验字
    uBit8 cLastIndex;           //上次发送数据的包序号
    COM_DATA_ID  ulSendID;      //发送数据ID
}COM_SLAVE_SEND_CTRL_DATA;

//接收控制数据
typedef struct _COM_RCV_CTRL_DATA
{
    //上下位机通用数据
    uBit8 *pRevBuf;             //数据区(保存接收到的完整数据包数据)
    uBit32 ulFreeLen;           //数据区剩余空间
    uBit32 ulRevLen;            //已接收的字节个数
    uBit8 cCheckNum;            //校验字
    uBit8 cReceiving;           //数据组接收中标志
    COM_DATA_ID  ulRevID;       //接收数据ID

    //下位机专用数据
    uBit32  ulWaitResultCode;   //等待结果标志
    uBit32  ulTimeOutCount;     //超时时间，当等待时间超过该时间时，等待结束
    double64 dStartTick;        //开始等待起始时间

    uBit8   uCmdType;           //等待指令类型
    uBit8   uCrdNo;             //坐标系号
    uBit8   uDevNo;             //设备编号（轴号）
    uBit8   uCmdModule;         //指令接收模块  0--设备管理模块 1--通道管理模块
}COM_RCV_CTRL_DATA;

//接收超时控制数据(下位机使用)
typedef struct _RCV_TIME_OUT_CTRL_DATA
{
    COM_DATA_ID  ulRevID;       //ID
    uBit8        cCheckNum;     //校验码
    uBit32       ulExeResult;   //执行结果
}RCV_TIME_OUT_CTRL_DATA;

//设置指令回应数据(由应用层调用，设置指令时接收回应包用)
#define COM_NOMAL_DATA_SIZE    (8)
typedef union _COM_NOMAL_DATA
{
    Bit8     cBuf8[COM_NOMAL_DATA_SIZE];
    uBit8    ucBuf8[COM_NOMAL_DATA_SIZE];
    Bit16    Buf16[COM_NOMAL_DATA_SIZE / 2]; 
    uBit16   uBuf16[COM_NOMAL_DATA_SIZE / 2];
    Bit32    iBuf32[COM_NOMAL_DATA_SIZE / 4];
    uBit32   uBuf32[COM_NOMAL_DATA_SIZE / 4];
    float32  dBuf32[COM_NOMAL_DATA_SIZE / 4];
    double64 dBuf64[COM_NOMAL_DATA_SIZE / 8];
}COM_NOMAL_DATA;


#pragma pack(pop)
#endif