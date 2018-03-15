#ifndef  __TLC5615_H
#define  __TLC5615_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * TLC5615��ģת��IC���ƽӿ�
 ****************************************************************************/

/**
  * @brief  TM1617 �ӿڳ�ʼ��
  * @param  pf_SetDIO �������ſ��ƽӿ�
  * @param  pf_SetCLK ʱ�����ſ��ƽӿ�
  * @param  pf_SetCS Ƭѡ���ſ��ƽӿ�
  * @retval 0-�ɹ�  ��0-ʧ��
  */
uBit32 TLC5615_InitInterface(void (*pf_SetDIO)(bool bState), 
                             void (*pf_SetCLK)(bool bState), 
                             void (*pf_SetCS)(bool bState));


/**
  * @brief  TLC5615ģ�����������
  * @param  ulWriteData ����������,��ΧΪ[0,0x3FF],��Ӧ[0,2Vef]
  * @retval 0-�ɹ� ��0-ʧ��
  */
uBit32 TLC5615_SetAnalogOutput(uBit32 ulWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __TLC5615_H */
