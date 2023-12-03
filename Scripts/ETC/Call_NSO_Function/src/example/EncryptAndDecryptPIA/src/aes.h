#ifndef __SWITCH_AES_H_
#define __SWITCH_AES_H_

extern void (*__DecryptPiaPacket)(char* out_data, char* data, unsigned int len, char* authentication_tag, unsigned int authentication_tag_size, char* wtf, void* dummy1, void* dummy2, int* error);
extern void (*__EncryptPiaPacket)(char* out_data, char* data, unsigned int len, char* authentication_tag, unsigned int authentication_tag_size, char* wtf, void* dummy1, void* dummy2, int* error);

typedef struct __attribute__((__packed__)) AesGcmRelated
{
    union 
    {
        unsigned int random32;
        struct
        {
            unsigned char random8[3];
            unsigned char connection_id;
        };
    };
    unsigned long long nonce;
} AesGcmRelated;

typedef struct PiaHeader57to510
{
    unsigned int magic;
    unsigned char encrypted;
    unsigned char connection_id;
    unsigned short packet_id;
    unsigned short session_timer;
    unsigned short rtt_timer;
    unsigned char aes_gcm_nonce[8];
    unsigned char aes_gcm_authentication_tag[16];
    unsigned char packet[0];
} PiaHeader57to510;

int CallDecrypt(char* out_data, char* data, unsigned int len, char* auth_tag, unsigned int tag_len, char* wtf);

int CallEncrypt(char* out_data, char* data, unsigned int len, char* auth_tag, unsigned int tag_len, char* wtf);

int DecryptPia(unsigned char* out_data, unsigned char* packet, int len, unsigned int random, unsigned char key[16]);

int EncryptPia(unsigned char* out_data, unsigned char* packet, int len, unsigned int random, unsigned char key[16]);

#endif