#pragma once

namespace smm2
{
    namespace object
    {
        enum Direction4On4 : u8
        {
            kRight = 0,
            kLeft,
            kUp,
            kDown
        };

        enum Direction16On4 : u8
        {
            kLeft = 1,
            kRight,
            kDown,
            kUp,
            kLeftToDown,
            kDownToLeft,
            kLeftToUp,
            kUpToLeft,
            kRightToDown,
            kDownToRight,
            kRightToUp,
            kUpToRight,
            kRightToEnd,
            kLeftToEnd,
            kUpToEnd,
            kDownToEnd
        };

        enum Direction16On16 : u16
        {
            kLeft = 1,
            kRight,
            kDown,
            kUp,
            kLeftToDown,
            kDownToLeft,
            kLeftToUp,
            kUpToLeft,
            kRightToDown,
            kDownToRight,
            kRightToUp,
            kUpToRight,
            kRightToEnd,
            kLeftToEnd,
            kUpToEnd,
            kDownToEnd
        };
    }
}