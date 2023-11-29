#ifndef SWITCH_SDK_H_
#define SWITCH_SDK_H_

#include "random.h"

#include <stdlib.h>
#include <stdint.h>

#define kSDKAddress 0x7100000000

extern void (*GenerateHmacSha256Mac)(void* out_hash, const size_t hash_size, const void* data_to_get_hash, const size_t data_size, const void* key, const size_t key_size);

void* LoadSDK(const char* sdk_name);

#endif
