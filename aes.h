/**
 * @file aes.h
 */

#ifndef HEADER_AES_H
#define HEADER_AES_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************
 * AES declarations 
 **************************************************************************/


typedef struct
{
    unsigned short int w_Rounds;
    unsigned short int w_KeySize;
    unsigned long pdw_Key[120];
    unsigned char pb_IV[16];
} str_AES;


void AES_SetKey(str_AES *ps_AES, const unsigned char *KEY,const unsigned char *IV);
void AES_ConvertKey(str_AES *ps_AES);
void AES_CBC_Encrypt(str_AES *ps_AES, const unsigned char *pb_Array,unsigned char *pb_EncryptedArray, int i_Length);
void AES_CBC_Decrypt(str_AES *ps_AES, const unsigned char *pb_Array, unsigned char *pb_DecryptedArray, int i_Length);

#ifdef __cplusplus
}
#endif

#endif 
