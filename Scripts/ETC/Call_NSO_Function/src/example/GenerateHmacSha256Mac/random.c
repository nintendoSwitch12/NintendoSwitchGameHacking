#include <stdint.h>

#include "random.h"

const uint32_t fixed_value[64] = {
    0x39B399D2, 0xFAE40B38, 0x851BC213, 0x8CB4E3D9,
    0x7ED1C46A, 0xE8050462, 0xD8D24F76, 0xB52886FC,
    0x67890BF0, 0xF5329CB0, 0xD597FB28, 0x2B8EE0EA,
    0x47574C51, 0x0F7569D9, 0xCF1163AE, 0xE4A153BF,
    0xD1FAE468, 0xD4C64738, 0x360106F5, 0xDD7EB113,
    0xC296F3E2, 0x2C58F258, 0x79B554E1, 0x85DF9D06,
    0xAA307330, 0x01410F69, 0xB2F2C573, 0x82B93EB1,
    0xF351A11C, 0x63098693, 0x885B5DA5, 0x8872A8ED,
    0xACD9CB13, 0xED7FBCAD, 0xE6A41EC2, 0x5F44E79F,
    0x8346F5B5, 0x389FE6ED, 0x507124B5, 0xE9B23EAA,
    0x577113F0, 0xA95ED917, 0x2F62D158, 0x47843F86,
    0xC65637D0, 0x2F272052, 0xBA4A4CC4, 0xB5F146F6,
    0x501B87A7, 0x51FC3A93, 0x6EDE3F02, 0x3D265728,
    0x9B809440, 0x75B89229, 0xF6A280CC, 0x8537FA68,
    0x5B5ED19A, 0x6FC05BB6, 0xF4EF5261, 0xAA1B7D4F,
    0xFCB26110, 0x00AD3D74, 0xC0E73A4B, 0xF132E7C7};

void RandomInit(Random* this, uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

uint32_t RandomGetU32(Random* this)
{
    uint32_t x = this->x ^ (this->x << 11u);
    this->x = this->y;
    this->y = this->z;
    this->z = this->w;
    this->w = this->w ^ (this->w >> 19u) ^ x ^ (x >> 8u);

    return this->w;
}

void GenerateHashKey(uint32_t key[4], Random *random)
{
    key[0] = 0; key[1] = 0; key[2] = 0x82A28C10; key[3] = 0;

    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            const uint32_t t = fixed_value[RandomGetU32(random) >> 26];
            key[i] = ((t >> ((RandomGetU32(random) >> 27) & 0x18)) & 0xff | key[i]) << 8;
        }
        const uint32_t t = fixed_value[RandomGetU32(random) >> 26];
        key[i] = ((t >> ((RandomGetU32(random) >> 27) & 0x18)) & 0xff | key[i]);
    }
}
