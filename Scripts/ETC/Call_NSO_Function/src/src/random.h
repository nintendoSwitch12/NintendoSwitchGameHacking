#ifndef SWITCH_RANDOM_H_
#define SWITCH_RANDOM_H_

#include <stdint.h>

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t w;
} Random;

void RandomInit(Random* this, uint32_t x, uint32_t y, uint32_t z, uint32_t w);

uint32_t RandomGetU32(Random* this);

void GenerateHashKey(uint32_t key[4], Random *random);

#endif
