#include <sys/mman.h>
#include <stdint.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "sdk.h"

void* LoadSDK(const char* sdk_path)
{

    const int fd = open(sdk_path, O_RDONLY);
    if(fd < 0)
    {
        printf("Failed to open %s\n", sdk_path);
        return 0;
    }
    void* addr = mmap((void*)kSDKAddress, kSDKSize, PROT_EXEC | PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    return addr;
}

