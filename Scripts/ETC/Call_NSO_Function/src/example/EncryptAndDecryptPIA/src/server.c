#include "server.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "aes.h"
#include "utils.h"

int IS_RANDOM_AND_KEY_WRONG = 1;
unsigned int RANDOM = 0;
char KEY[16] = {0,};

void ServerStart(const int port)
{
    int sock;
    struct sockaddr_in addr, client_addr;
    char send_buffer[0x600];
    char packet[0x100];
    int addr_len;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket ");
        return;
    }

    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
 
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind ");
        close(sock);
        return;
    }

    addr_len = sizeof(client_addr);
    while(1)
    {
        int recv_len = recvfrom(sock, packet, 0x100, 0, (struct sockaddr *)&client_addr, &addr_len);
        int error = 0, do_send = 0, send_len = 0;
        char* ptr_packet = packet;
        ServerCommand cmd = *ptr_packet++;
        switch(cmd)
        {
            case kStop:
                close(sock);
                return;
                break;

            case kSetRandomAndKey:
                SetRandomAndkey(*(unsigned int*)ptr_packet, ptr_packet+4);
                break;

            case kGetRandomAndKey:
                *(unsigned int*)(send_buffer+1) = RANDOM;
                memcpy(send_buffer+5, KEY, 16);
                do_send = 1;
                send_len = 1+0x14;
                break;

            case kDecrypt:
                error = DoDecrypt(send_buffer + 1, ptr_packet, recv_len - 1);
                do_send = 1;
                send_len = recv_len;
                break;

            case kEncrpyt:
                error = DoEncrypt(send_buffer + 1, ptr_packet, recv_len - 1);
                do_send = 1;
                send_len = recv_len;
                break;

            default:
                break;
        }

        if(!do_send)
            continue;
        if(!error)
        {
            *send_buffer = kOk;
        }
        else
        {
            *send_buffer = kFail;
            send_len = 1;
        }
        sendto(sock, send_buffer, send_len, 0, (struct sockaddr*)&client_addr, addr_len);
    }
}

void SetRandomAndkey(const unsigned int random, const char key[16])
{
    RANDOM = random;
    memcpy(KEY, key, 16);
    IS_RANDOM_AND_KEY_WRONG = 0;
}

int DoDecrypt(unsigned char* out, unsigned char* pia_packet, int packet_len)
{
    if(IS_RANDOM_AND_KEY_WRONG)
        return -1;

    return DecryptPia(out, pia_packet, packet_len, RANDOM, KEY);
}

int DoEncrypt(unsigned char* out, unsigned char* pia_packet, int packet_len)
{
    if(IS_RANDOM_AND_KEY_WRONG)
        return -1;

    return EncryptPia(out, pia_packet, packet_len, RANDOM, KEY);
}
