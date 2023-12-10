#ifndef __SWITCH_SDK_H_
#define __SWITCH_SDK_H_

#include <stdlib.h>
#include <stdint.h>

#define kSDKAddress 0x80004000
#define kSDKSize 0x2ebe000

void* LoadSDK(const char* sdk_path, void* addr, size_t size);

#endif
