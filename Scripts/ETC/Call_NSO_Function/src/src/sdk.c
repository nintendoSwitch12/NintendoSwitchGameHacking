#include <sys/mman.h>
#include <stdint.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "sdk.h"

void (*GenerateHmacSha256Mac)(void* out_hash, const size_t hash_size, const void* data_to_get_hash, const size_t data_size, const void* key, const size_t key_size) = (void*)(kSDKAddress + 0x173510);

void* LoadSDK(const char* sdk_path)
{

    const int fd = open(sdk_path, O_RDONLY);
    if(fd < 0)
    {
        printf("Failed to open %s\n", sdk_path);
        return 0;
    }
    void* addr = mmap((void*)kSDKAddress, 0xCD4060, PROT_EXEC | PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    return addr;
}

