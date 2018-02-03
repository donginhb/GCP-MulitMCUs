 /**
   ******************************************************************************
   * @file    ucom.h
   * @author  杜公子寒枫
   * @version V1.0 寄存器版本
   * @date    2017.08.08
   * @brief   串口指令解析函数
   ******************************************************************************
   * @attention
   * 
   * 
   * 
   ******************************************************************************
   */
   
   
/***********************************<INCLUDES>**********************************/
#include "CMU_Unpack.h"
#include <stdlib.h>
#include <string.h>
#include "DataType/DataType.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//帧数据长度定义(字节)
#define PACK_HEAD_STR                           "HCZN"      //帧头
#define PACK_SIZE_HEAD                          (4)         //帧头长度
#define PACK_SIZE_LENSEG                        (2)         //帧长度字段长度
#define PACK_SIZE_CHECKNUM                      (2)         //校验字长度

#define PACK_PACK_MAX_LEN                       (512+128)   //单次发送的最大总长度
//单次发送允许最大的发送数据区长度          
#define PACK_DATA_MAX_LENGTH                    (PACK_PACK_MAX_LEN-PACK_SIZE_HEAD-PACK_SIZE_LENSEG-PACK_SIZE_CHECKNUM)

//状态机工作步骤
#define UNPACK_STEP_GET_HEAD                    (0)         //获取包头
#define UNPACK_STEP_GET_LEN                     (1)         //获取帧长度
#define UNPACK_STEP_GET_DATA                    (2)         //获取数据区
#define UNPACK_STEP_GET_CHECKSUM                (3)         //获取校验字


static uBit8    *m_pHead = PACK_HEAD_STR;                   //指向帧头
static uBit8     m_bGetUnpackStep=UNPACK_STEP_GET_HEAD;     //当前状态机的步数
static uBit16    m_nReadPos=0;                              //已经处理的数据位置 从接收缓冲区头开始计数
static uBit16    m_nWritePos=0;                             //已经接收到的数据位置 从接收缓冲区头开始计数
static uBit16    m_nDataLen=0;                              //数据区长度
static uBit8     m_pRcvBuf[PACK_PACK_MAX_LEN] = {0};        //接收数据缓冲区


static uBit32 CMU_RecvNullEntry(uBit8 *pRBuff, uBit32 ulSize){ return 0; }          //空入口
static uBit32 (*pf_CmuRecvBuff)(uBit8 *pRBuff, uBit32 ulSize) = CMU_RecvNullEntry;   //数据接收接口指针


/**
  * @brief  数据接收
  * @param  pRcvBuf 接收数据缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval 实际上接收到的数据长度
  */
static uBit32 CMU_RecvHardwareBuff(uBit8 *pRBuff, uBit32 ulSize)
{
    
    return pf_CmuRecvBuff(pRBuff, ulSize);
}


/*****************************************************************************
 * CMU 数据解包接口
 ****************************************************************************/

/**
  * @brief  数据接收接口设置
  * @param  pf_RecvBuff 数据接口函数指针
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_SetUnpackRecvInterface(uBit32 (*pf_RecvBuff)(uBit8 *, uBit32))
{
    if (pf_RecvBuff != NULL)
    {
        pf_CmuRecvBuff = pf_RecvBuff;
        return 0;
    }
    
    return 1;
}


/**
  * @brief  数据解包
  * @param  pRcvBuf 接收数据缓冲区
  * @param  pRcvLen 接收到的数据(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UnPack(uBit8* pRcvBuf, uBit32* pRcvLen)
{
    int iRecvCount;
    uBit16  nRestCount;
    uBit16  nRcvCheckSum = 0;                    //校验字(数据包中的校验字)
    uBit16  nCheckSum=0;                         //计数得到的校验字

    while(1)
    {
        switch(m_bGetUnpackStep)
        {
            case UNPACK_STEP_GET_HEAD:      //获取文件头(找头)
                {
                    if(m_nReadPos!=0)
                    {
                        //剔除无效的指令头
                        memcpy(m_pRcvBuf, &m_pRcvBuf[m_nReadPos],m_nWritePos-m_nReadPos);
                        m_nWritePos -= m_nReadPos;
                        m_nReadPos=0;
                    }

                    //从串口外设中获取接收到的串口数据,并返回实际上接收到的字节数 -- 仅注释 杜寒枫 2017.06.28
                    iRecvCount = CMU_RecvHardwareBuff(&m_pRcvBuf[m_nWritePos] , PACK_PACK_MAX_LEN-m_nWritePos);
                    
                    if (iRecvCount<=0 && m_nWritePos==0)
                        return 1;
    
                    m_nWritePos += iRecvCount;

                    //查找包头
                    for (int i=0; i<m_nWritePos; i++)
                    {
                        if (m_pRcvBuf[i]==m_pHead[0])//找到可能是包头
                        {
                            nRestCount = m_nWritePos-i; //nRestCount为当前未查找的数据包长度

                            if (nRestCount>=PACK_SIZE_HEAD)//检查包头长度
                            {
                                if (memcmp(&m_pRcvBuf[i],m_pHead,PACK_SIZE_HEAD)==0)//找到文件头
                                {
                                    m_bGetUnpackStep = UNPACK_STEP_GET_LEN;    //将接下来的工作步骤设置为获取其数据帧长度 -- 仅注释 杜寒枫 2017.06.28
                                    m_nReadPos = PACK_SIZE_HEAD;
                                    break;
                                }
                            }
                            //包头长度不够,则将疑似包头及之后的数据移至缓冲区起始位置
                            else
                            {
                                //例如,将"xxxVxxxxx"--->"Vxxxxx"
                                //并将其m_nUcomWritePos位置变量移到相应的位置
                                // -- 仅注释 杜寒枫 2017.06.28
                                memcpy(m_pRcvBuf, &m_pRcvBuf[i], m_nWritePos-i);
                                m_nWritePos -= i;
                                return 1;
                            }
                        }
                    }

                    if (m_bGetUnpackStep==UNPACK_STEP_GET_HEAD)//包头接收不完整则继续接收数据
                    {
                        m_nWritePos = 0; 
                        return 1;               
                    }
                }
            case UNPACK_STEP_GET_LEN: //获取包长度
                {
                    if (m_nWritePos-m_nReadPos >= PACK_SIZE_LENSEG)//已经读取到数据长度
                    {
                        m_nDataLen = *(uBit16 *)&m_pRcvBuf[PACK_SIZE_HEAD];

                        //如果长度数据错误，则重新找包头，丢弃不正确数据包
                        if (m_nDataLen < (PACK_SIZE_HEAD+PACK_SIZE_LENSEG+PACK_SIZE_CHECKNUM) ||
                            m_nDataLen > PACK_DATA_MAX_LENGTH)
                        {
                            m_bGetUnpackStep = UNPACK_STEP_GET_HEAD;
                            memcpy(m_pRcvBuf, &m_pRcvBuf[m_nReadPos],m_nWritePos-m_nReadPos);
                            m_nWritePos -= m_nReadPos;
                            m_nReadPos=0;
                        }
                        //否则,则计算出接来下要获取的数据包的长度,并将其工作步骤设置为获取数据区
                        //同时,将其m_nUcomReadPos的位置进行偏移UCOM_SIZE_LENSEG位
                        // -- 仅注释 杜寒枫 2017.06.28
                        else
                        {
                            m_nDataLen -= (PACK_SIZE_HEAD+PACK_SIZE_LENSEG+PACK_SIZE_CHECKNUM);//获取数据包长度
                            m_bGetUnpackStep = UNPACK_STEP_GET_DATA; 
                            m_nReadPos += PACK_SIZE_LENSEG;
                        }
                    }
                    //未读到数据长度，继续读数据区
                    else
                    {
                        iRecvCount = CMU_RecvHardwareBuff(&m_pRcvBuf[m_nWritePos] , PACK_PACK_MAX_LEN-m_nWritePos);
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nWritePos += iRecvCount;
                    }

                    break;
                }

            case UNPACK_STEP_GET_DATA: //获取数据区
                {
                    if (m_nWritePos-m_nReadPos >= m_nDataLen)//数据区获取完毕
                    {
                        m_bGetUnpackStep = UNPACK_STEP_GET_CHECKSUM;  
                        m_nReadPos += m_nDataLen;   
                    }
                    //没有获取完整数据区数据,继续读数据区
                    else
                    {
                        iRecvCount = CMU_RecvHardwareBuff(&m_pRcvBuf[m_nWritePos] , PACK_PACK_MAX_LEN-m_nWritePos);
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nWritePos += iRecvCount;
                    }

                    break;
                }

            case UNPACK_STEP_GET_CHECKSUM: //获取校验字
                {
                    if (m_nWritePos-m_nReadPos >= PACK_SIZE_CHECKNUM)//校验字获取完毕
                    {
                        nRcvCheckSum = *(uBit16 *)&m_pRcvBuf[m_nReadPos];

                        //计数校验字
                        for (uBit16  i=0; i<m_nReadPos; i++)
                        {
                            nCheckSum += m_pRcvBuf[i];
                        }

                        //比较校验字
                        if (nRcvCheckSum==nCheckSum)//成功获取数据包
                        {
                            *pRcvLen = m_nDataLen;

                            //剔除掉帧头和数据帧长度信息,将剩余的数据拷贝给pRcvBuf
                            //例如,"VDMACH12Axxxxxxx"--->"Axxxxxxx",不包含帧头,数据帧长度位以及校验位的数据在里面
                            //-- 仅注释 杜寒枫 2017.06.28
                            memcpy(pRcvBuf, &m_pRcvBuf[PACK_SIZE_HEAD+PACK_SIZE_LENSEG],m_nDataLen);
                            
                            //将多余接收到的数据包保存移动到起始位置
                            m_nWritePos -= (m_nReadPos + PACK_SIZE_CHECKNUM);
                            memcpy(m_pRcvBuf, &m_pRcvBuf[m_nReadPos+PACK_SIZE_CHECKNUM],m_nWritePos);
                            m_bGetUnpackStep = UNPACK_STEP_GET_HEAD; 
                            m_nReadPos = 0; 
                            return 0;
                        }else//校验出错
                        {
                            //从包头后面重新开始找包头
                            m_nWritePos -= PACK_SIZE_HEAD;
                            memcpy(m_pRcvBuf, &m_pRcvBuf[PACK_SIZE_HEAD],m_nWritePos);
                            m_bGetUnpackStep = UNPACK_STEP_GET_HEAD;  
                            m_nReadPos = 0; 
                            return 1;
                        }   
                    }
                    //没有获取完整校验字,继续读数据区
                    else
                    {
                        iRecvCount = CMU_RecvHardwareBuff(&m_pRcvBuf[m_nWritePos] , PACK_PACK_MAX_LEN-m_nWritePos);                      
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nWritePos += iRecvCount;
                    }
                    break;
                }

            default:break;
        }
        
        break;
    }

    return 1;   
}
