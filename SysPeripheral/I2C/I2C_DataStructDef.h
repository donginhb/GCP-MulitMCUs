#ifndef  __I2C_DATA_STRUCT_DEF_H
#define  __I2C_DATA_STRUCT_DEF_H

#include "DataType/DataType.h"

//I2C错误码
#define I2C_ERR_OK                  (0)         //There is no error
#define I2C_ERR_RROR                (1)         //A generic error happens
#define I2C_ERR_TIMEOUT             (2)         //Timed out
#define I2C_ERR_FULL                (3)         //The resource is full
#define I2C_ERR_EMPTY               (4)         //The resource is empty
#define I2C_ERR_NOMEM               (5)         //No memory
#define I2C_ERR_NOSYS               (6)         //No system
#define I2C_ERR_BUSY                (7)         //Busy
#define I2C_ERR_IO                  (8)         //IO error
    
//I2C控制标志   
#define I2C_FLAG_WR                 (0x0000)    
#define I2C_FLAG_RD                 (0x0001)    
#define I2C_FLAG_DEV_ADDR_16BIT     (0x0002)    //this is a 16 bit chip address
#define I2C_FLAG_REG_ADDR_16BIT     (0x0004)    //this is a 16 bit reg  address
#define I2C_FLAG_NO_DEV_ADDR        (0x0008)    
#define I2C_FLAG_NO_REG_ADDR        (0x0010)    
#define I2C_FLAG_IGNORE_NACK        (0x0020)    

typedef enum
{
    I2C_DEV_CTRL_CLEAR_ALL_FLAGS = 0xA0,
    I2C_DEV_CTRL_DEV_ADDR_8BIT  ,
    I2C_DEV_CTRL_DEV_ADDR_16BIT ,
    I2C_DEV_CTRL_NO_DEV_ADDR    ,
    I2C_DEV_CTRL_REG_ADDR_8BIT  ,
    I2C_DEV_CTRL_REG_ADDR_16BIT ,
    I2C_DEV_CTRL_NO_REG_ADDR    ,
    I2C_DEV_CTRL_SET_TIMEOUT    ,
    I2C_DEV_CTRL_SET_RETRIES    ,
    I2C_DEV_CTRL_SET_DEV_ADDR   ,
    I2C_DEV_CTRL_SET_BIT_OPS    ,
    
}I2C_DEV_CTRL_TYPE;


//I2C BIT OPS
typedef struct
{
    void    (*pf_I2C_SetSDA)            (void *Data, uBit8 uState);
    void    (*pf_I2C_SetSCL)            (void *Data, uBit8 uState);
    uBit8   (*pf_I2C_GetSDA)            (void *Data);
    uBit8   (*pf_I2C_GetSCL)            (void *Data);
    void    (*pf_I2C_BitDelay)          (void *Data);
    
    uBit16  nTimeOut;       //单位: 毫秒
    
    void    (*Data);        //预留接口

}I2C_BIT_OPS_TYPE;


//I2C BUS BASE OPERATIONS
typedef struct
{
    void    (*pf_I2C_Start)            (I2C_BIT_OPS_TYPE *pBitOps);
    void    (*pf_I2C_Stop)             (I2C_BIT_OPS_TYPE *pBitOps);
    uBit8   (*pf_I2C_WaitAck)          (I2C_BIT_OPS_TYPE *pBitOps);
    void    (*pf_I2C_SendAckOrNack)    (I2C_BIT_OPS_TYPE *pBitOps, uBit8 uAck);
    uBit8   (*pf_I2C_SendByte)         (I2C_BIT_OPS_TYPE *pBitOps, uBit8 uByte);
    uBit8   (*pf_I2C_RecvByte)         (I2C_BIT_OPS_TYPE *pBitOps);
    
    I2C_BIT_OPS_TYPE    (BitOps);       //位操作接口
    
}I2C_BUS_BASE_OPS_TYPE;


//I2C BUS CTRL PARAMETER
typedef struct 
{
    uBit16      nFlags;             //标志位,标志设备地址8/16位模式,寄存器地址8/16位模式等
    uBit16      nDevAddr;           //设备地址
    
    uBit16      nTimeOut;           //超时时间
    uBit16      nRetries;           //出错重发次数
    
}I2C_CTRL_PARM;


//I2C BUS DEVICE OPERATIONS
typedef struct
{
    uBit32 (*pf_I2C_MST_SendBuff)       (I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                                         uBit16 nRegAddr, const uBit8 *pBuff, uBit32 ulCount);
    uBit32 (*pf_I2C_MST_RecvBuff)       (I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                                         uBit16 nRegAddr, uBit8 *pBuff, uBit32 ulCount);
    
    I2C_BUS_BASE_OPS_TYPE               (BaseOps);    //基本操作接口
    I2C_CTRL_PARM                       (CtrlParm);   //控制参数
    
}I2C_BUS_DEVICE_OPS_TYPE;


#endif /* __I2C_DATA_STRUCT_DEF_H */
