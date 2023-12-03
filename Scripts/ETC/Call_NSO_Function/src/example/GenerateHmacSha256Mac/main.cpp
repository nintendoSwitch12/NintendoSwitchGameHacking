#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "random.h"
#include "sdk.h"

#define kDefaultSDKPath "./sdk"

void (*GenerateHmacSha256Mac)(void* out_hash, const size_t hash_size, const void* data_to_get_hash, const size_t data_size, const void* key, const size_t key_size) = (void*)(kSDKAddress + 0x173510);

int main(int argc, char** argv)
{
    char* sdk_name = kDefaultSDKPath;

    if(LoadSDK(sdk_name) != (void*)kSDKAddress)
    {
        puts("Failed to mmap at 0x7100000000");
        return -1;
    }

    uint32_t sead[4];
    for(int i=0; i<4; ++i)
        sead[i] = 0;

    Random r;
    RandomInit(&r, sead[0], sead[1], sead[2], sead[3]);

    uint32_t key[4];
    GenerateHashKey(key, &r);

    uint64_t hash[4];
    GenerateHmacSha256Mac(hash, 0x20, data, 0x1BF9C, key, 0x10);
    return 0;
}