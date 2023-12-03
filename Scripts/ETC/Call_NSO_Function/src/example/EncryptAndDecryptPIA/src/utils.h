#ifndef __SWITCH_UTILS_H_
#define __SWITCH_UTILS_H_

void PrintHex(unsigned char* data, int len);

unsigned char GetByteFromHex(unsigned char* data);

int GetBytesFromHex(unsigned char* out, unsigned char* data, int len);

#endif