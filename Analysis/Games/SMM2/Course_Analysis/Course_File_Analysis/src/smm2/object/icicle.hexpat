#pragma once

namespace smm2::object
{
    enum IcicleType : u8
    {
        kFalling = 0,
        kSolid
    };

    struct Icicle
    {
        u8 x_position;
        u8 y_position;
        IcicleType type;
        u8 reserved;
    };
}