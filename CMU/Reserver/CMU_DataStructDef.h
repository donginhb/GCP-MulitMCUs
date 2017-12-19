#ifndef  __CMU_DATA_STRUCT_DEF_H
#define  __CMU_DATA_STRUCT_DEF_H

#include "DataType/DataType.h"

#pragma pack(push)
#pragma pack(8)    

/*****************************************************************************
 * COM_ID及数据帧类型定义
 ****************************************************************************/

typedef struct 
{
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
}COM_ID, *P_COM_ID;


//帧ID类型定义
typedef union 
{
    COM_ID      ulComDataID;
    uBit32      ulFrameID;
}COM_DATA_ID,*P_COM_DATA_ID;


//数据包结构类型定义
typedef struct 
{
    COM_DATA_ID ulID;
    uBit32 ulDataLen;
    uBit8 *pDataBuf;
}COM_DATA_PACK;


//ulOperType:Bit0 数据操作类型定义
#define DATA_OPRE_TYPE_SET                  (0)     //设置指令
#define DATA_OPRE_TYPE_GET                  (1)     //查询指令

//ulDataType:Bit1-Bit3 操作数据种类
#define DATA_TYPE_SYS                       (0)     //系统数据
#define DATA_TYPE_CRD                       (1)     //坐标系数据
#define DATA_TYPE_AXIS                      (2)     //轴数据
#define DATA_TYPE_IO                        (3)     //IO数据
#define DATA_TYPE_ADDA                      (4)     //ADDA数据
#define DATA_TYPE_SERVO                     (5)     //伺服数据
#define DATA_TYPE_REG                       (6)     //寄存器数据
#define DATA_TYPE_CUSTOM                    (7)     //自定义指令       // AutoSale

//ulCmdType:Bit4 指令类型        
#define SETCMD_TYPE_NOMAL                   (0)     //常规设置指令
#define SETCMD_TYPE_INQ                     (1)     //周期查询设置指令
#define GETCMD_TYPE_NOMAL                   (0)     //常规获取指令
#define GETCMD_TYPE_INQ                     (1)     //周期性数据获取指令


//ulCmdIndex:Bit5-Bit10 指令序号
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
#define SYS_SETCMD_SET_SLC_FLAG             (18)    //置位SLC标志

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

//接收对象
#define SCR_RECEIVE                         (0)     //人机界面接收帧
#define SLC_RECEIVE                         (1)     //主控接收帧

//数据传输帧类别
#define TRANSMIT_FIRST_FRAME                (00)    //该文件开始传送（第一帧）
#define TRANSMIT_BEING_FRAME                (01)    //该文件传送中
#define TRANSMIT_SELF_FRAME                 (02)    //独立发送帧
#define TRANSMIT_VERIFY_FRAME               (03)    //校验帧


/*****************************************************************************
* 下位机错误编号定义
 ****************************************************************************/
#define CMU_ERR_SUCCESS                             (0)     //成功
#define CMU_ERR_CONNECT                             (1)     //表示通信连接出错(接收和发送抽象层硬件出错)
#define CMU_ERR_SEND_FAIL                           (2)     //发送数据失败
#define CMU_ERR_OUTMODED                            (3)     //过时的数据
#define CMU_ERR_DEV_NOT_EXIST                       (4)     //设备不存在，或未编号
#define CMU_ERR_CRD_NOT_EXIST                       (5)     //通道不存在
#define CMU_ERR_DATA_WRONG                          (6)     //数据错误,接口参数错误
#define CMU_ERR_SEND_OVERFLOW                       (7)     //发送缓冲区溢出
#define CMU_ERR_CHECK_FAIL                          (8)     //接收数据校验错误
#define CMU_ERR_TIME_OUT                            (9)     //接收回应包超时
#define CMU_ERR_DATA_LOST                           (10)    //接收数据存在数据丢失
#define CMU_ERR_RCV_OVERFLOW                        (11)    //接收缓冲区溢出
#define CMU_ERR_INVALID_CMD                         (12)    //非法指令
#define CMU_ERR_UNBAND                              (13)    //未绑定外部指令接口
#define CMU_ERR_INQ_OUTRANG                         (14)    //周期性查询数据已经到达系统允许最大值
#define CMU_ERR_GET_DATA_FAIL                       (15)    //获取数据失败
#define CMU_ERR_FLASH_OPER                          (16)    //Flash操作失败
#define CMU_ERR_UPDATE_FILE_CHECK                   (17)    //升级文件校验错误
#define CMU_ERR_SYS_BUSY                            (18)    //系统忙
#define CMU_ERR_IPR_ERROR                           (19)    //解释器错误
#define CMU_ERR_JUMPTO_BOOT                         (20)    //正在跳转到BOOTLOADER
#define CMU_ERR_CMD_DISMATCH                        (21)    //指令执行结果检查指令不匹配
#define CMU_ERR_INVALID_COMTYPE                     (22)    //无效通信方式
#define CMU_ERR_UREGNO_RANGOUT                      (23)    //寄存器编号超限
#define CMU_ERR_IPO_BUSY                            (24)    //插补器不空闲
#define CMU_ERR_CSM_BUSY                            (25)    //通道不空闲
#define CMU_ERR_RS_MALLOC_FAIL                      (26)    //接收缓冲区申请空间失败
#define CMU_ERR_IPR_ENABLE                          (27)    //解释器已使能
#define CMU_ERR_NO_ACK                              (28)    //不应答


/*****************************************************************************
* 通信类型定义
 ****************************************************************************/

#define COM_TYPE_NULL                               (0)     //没有连接
#define COM_TYPE_CAN                                (1)     //CAN 通信模式
#define COM_TYPE_ENET                               (2)     //ENET通信模式
#define COM_TYPE_UART                               (4)     //UART通信模式
#define COM_TYPE_MAX                                (4)     //最大通信模式序号




#pragma pack(pop)

#endif /* __CMU_DATA_STRUCT_DEF_H */
