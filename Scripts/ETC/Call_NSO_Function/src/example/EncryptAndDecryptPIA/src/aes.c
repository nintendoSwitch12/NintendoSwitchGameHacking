#include "aes.h"

#include <stdlib.h>

#include "utils.h"

void (*__DecryptPiaPacket)(char* out_data, char* data, unsigned int len, char* authentication_tag, unsigned int authentication_tag_size, char* wtf, void* dummy1, void* dummy2, int* error) = (void*)(0x80004000 + 0x1547b9c);
void (*__EncryptPiaPacket)(char* out_data, char* data, unsigned int len, char* authentication_tag, unsigned int authentication_tag_size, char* wtf, void* dummy1, void* dummy2, int* error) = (void*)(0x80004000 + 0x1547a3c);

int CallDecrypt(char* out_data, char* data, unsigned int len, char* auth_tag, unsigned int tag_len, char* wtf)
{
    int error;
    int* perror = &error;
    __asm__ __volatile__(
        "mov x19, %[dec]\n"
        "mov x18, %[err]\n"
        "mov x15, %[wtf]\n"
        "mov x14, #0x10\n"
        "mov x13, %[auth_tag]\n"
        "mov x12, %[len]\n"
        "mov x11, %[data]\n"
        "mov x10, %[out_data]\n"
        "mov x9, x19\n"
        "mov x8, x18\n"
        "mov x5, x15\n"
        "mov x4, x14\n"
        "mov x3, x13\n"
        "mov x2, x12\n"
        "mov x1, x11\n"
        "mov x0, x10\n"
        "blr x9\n"
        : 
        : [out_data] "r" (out_data), [data] "r" (data), 
        [len] "r" (len), [auth_tag] "r" (auth_tag), 
        [tag_len] "r" (tag_len), [wtf] "r" (wtf), 
        [err] "r" (perror), [dec] "r" (__DecryptPiaPacket)
        : "memory"
    );
    return error;
}

int CallEncrypt(char* out_data, char* data, unsigned int len, char* auth_tag, unsigned int tag_len, char* wtf)
{
    int error;
    int* perror = &error;
    __asm__ __volatile__(
        "mov x19, %[dec]\n"
        "mov x18, %[err]\n"
        "mov x15, %[wtf]\n"
        "mov x14, #0x10\n"
        "mov x13, %[auth_tag]\n"
        "mov x12, %[len]\n"
        "mov x11, %[data]\n"
        "mov x10, %[out_data]\n"
        "mov x9, x19\n"
        "mov x8, x18\n"
        "mov x5, x15\n"
        "mov x4, x14\n"
        "mov x3, x13\n"
        "mov x2, x12\n"
        "mov x1, x11\n"
        "mov x0, x10\n"
        "blr x9\n"
        : 
        : [out_data] "r" (out_data), [data] "r" (data), 
        [len] "r" (len), [auth_tag] "r" (auth_tag), 
        [tag_len] "r" (tag_len), [wtf] "r" (wtf), 
        [err] "r" (perror), [dec] "r" (__EncryptPiaPacket)
        : "memory"
    );
    return error;
}

int DecryptPia(unsigned char* out_data, unsigned char* packet, int packet_len, unsigned int random, unsigned char key[16])
{
    PiaHeader57to510* pia_hdr = (PiaHeader57to510*)(packet);
    char* tag = pia_hdr->aes_gcm_authentication_tag;
    AesGcmRelated aes_gcm_related = {
        .random32 = random,
        .nonce = *(unsigned long long*)pia_hdr->aes_gcm_nonce
    };
    aes_gcm_related.connection_id = pia_hdr->connection_id;
    
    unsigned long long wtf[] = {1, (unsigned long long)&aes_gcm_related, 0x210000000C, (unsigned long long)key, 0x10};

    const int error = CallDecrypt(out_data+0x24, pia_hdr->packet, packet_len - 0x24, tag, 16, wtf);
    memcpy(out_data, packet, 0x24);
    return error;
}

int EncryptPia(unsigned char* out_data, unsigned char* packet, int packet_len, unsigned int random, unsigned char key[16])
{
    PiaHeader57to510* pia_hdr = (PiaHeader57to510*)(packet);
    char* tag = pia_hdr->aes_gcm_authentication_tag;
    AesGcmRelated aes_gcm_related = {
        .random32 = random,
        .nonce = *(unsigned long long*)pia_hdr->aes_gcm_nonce
    };
    aes_gcm_related.connection_id = pia_hdr->connection_id;
    
    unsigned long long wtf[] = {1, (unsigned long long)&aes_gcm_related, 0xC, (unsigned long long)key, 0x10};

    const int error = CallEncrypt(out_data+0x24, pia_hdr->packet, packet_len - 0x24, tag, 16, wtf);
    memcpy(out_data, packet, 0x24);
    return error;
}
