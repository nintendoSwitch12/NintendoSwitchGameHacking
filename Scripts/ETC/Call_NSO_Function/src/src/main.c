#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "random.h"
#include "sdk.h"

#define kDefaultSDKPath "./sdk"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage: %s [jpeg file] (sdk file)\n", argv[0]);
        return -1;
    }
    
    char* sdk_name = kDefaultSDKPath;
    if(argc >= 3)
        sdk_name = argv[2];

    if(LoadSDK(sdk_name) != (void*)kSDKAddress)
    {
        puts("Failed to mmap at 0x7100000000");
        return -1;
    }

    const int fd = open(argv[1], O_RDWR);
    unsigned char* data = malloc(0x1BFD0);
    if(fd < 0)
    {
        printf("Cannot open %s\n", argv[1]);
        return -1;
    }
    else if(lseek(fd, 0, SEEK_END) != 0x1BFD0)
    {
        puts("The size of jpeg is not 0x1BFD0");
        return -1;
    }
    if(data < 0)
    {
        puts("Failed to allocate memory");
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    read(fd, data, 0x1BFD0);

    uint32_t sead[4];
    for(int i=0; i<4; ++i)
        sead[i] = *(uint32_t*)(data + 0x1BFC0 + 4*i);

    Random r;
    RandomInit(&r, sead[0], sead[1], sead[2], sead[3]);

    uint32_t key[4];
    GenerateHashKey(key, &r);

    uint64_t hash[4];
    GenerateHmacSha256Mac(hash, 0x20, data, 0x1BF9C, key, 0x10);

    lseek(fd, 0x1BFA0, 0);
    write(fd, hash, 0x20);

    close(fd);
    return 0;
}
