#ifndef __SWITCH_SDK_H_
#define __SWITCH_SDK_H_

#include "random.h"

#include <stdlib.h>
#include <stdint.h>

#define kSDKAddress 0x7100000000
#define kSDKSize 0xCD4060

void* LoadSDK(const char* sdk_name);

#endif
