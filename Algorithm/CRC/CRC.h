#ifndef  __CRC_H
#define  __CRC_H

#ifdef __cplusplus
extern "C"
{
#endif

    
/**
  * @brief  CRC16校验
  * @param  pBuf是要进行CRC校验的缓冲区
  * @param  iCount是缓冲区的长度
  * @retval 16位校验码
  */
unsigned char CRC8_GetValue(unsigned char *pBuf, unsigned long int iCount);

    
/**
  * @brief  CRC16校验
  * @param  pBuf是要进行CRC校验的缓冲区
  * @param  iCount是缓冲区的长度
  * @retval 16位校验码
  */
unsigned short int CRC16_GetValue(unsigned char *pBuf, unsigned long int iCount);


#ifdef __cplusplus
}
#endif

#endif /* __CRC_H */
