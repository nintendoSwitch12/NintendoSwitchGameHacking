#include "utils.h"

#include <stdio.h>
#include <string.h>

void PrintHex(unsigned char* data, int len)
{
    for(int i=0; i<len; ++i)
    {
        printf("%02x", data[i]);
    }
}

unsigned char GetByteFromHex(unsigned char* data)
{
    unsigned char ret = 0;
    if(*data >= '0' && *data <= '9')
        ret |= *data - '0';
    else if(*data >= 'a' && *data <= 'f')
        ret |= *data - 'a' + 10;
    else
        ret |= *data - 'A' + 10;
    ret <<= 4;
    if(data[1] >= '0' && data[1] <= '9')
        ret |= data[1] - '0';
    else if(data[1] >= 'a' && data[1] <= 'f')
        ret |= data[1] - 'a' + 10;
    else
        ret |= data[1] - 'A' + 10;
    return ret;
}

int GetBytesFromHex(unsigned char* out, unsigned char* data, int len)
{
    for(int i=0; i<len; ++i)
        out[i] = GetByteFromHex(data + 2*i);
}