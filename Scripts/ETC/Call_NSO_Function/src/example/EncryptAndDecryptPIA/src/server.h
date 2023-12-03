#ifndef __SERVER_H_
#define __SERVER_H_

extern int IS_RANDOM_AND_KEY_WRONG;
extern unsigned int RANDOM;
extern char KEY[16];

typedef enum ServerCommand
{
    kStop = 0,
    kSetRandomAndKey,
    kGetRandomAndKey,
    kDecrypt,
    kEncrpyt,
    kNumOfCommands
} ServerCommand;

typedef enum ServerRespones
{
    kOk = 0,
    kFail = 0xff
} ServerRespones;

void ServerStart(const int port);

void SetRandomAndkey(const unsigned int random, const char key[16]);

int DoDecrypt(unsigned char* out, unsigned char* pia_packet, int packet_len);

int DoEncrypt(unsigned char* out, unsigned char* pia_packet, int packet_len);

#endif