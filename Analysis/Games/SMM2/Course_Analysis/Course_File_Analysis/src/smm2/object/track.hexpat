#pragma once

namespace smm2::object
{
    enum TrackFlag : u8
    {
        kHasObject = 1
    };

    enum TrackType : u8
    {
        kHorizontal = 0,
        kVertical,
        kDescendingDiagonal,
        kAscendingDiagonal,
        kBottomLeftCurve,
        kTopRightCurve,
        kTopLeftCurve,
        kBottomRightCurve,
        kHorizontal3,
        kHorizontal1,
        kVertical3,
        kVertical1,
        kHorizontal4,
        kHorizontal2,
        kVertical4,
        kVertical5
    };

    struct Track
    {
        u16 unknown;
        TrackFlag flags;
        u8 x_position;
        u8 y_position;
        TrackType type;
        u16 link_id;
        u16 unknown1;
        u16 unknown2;
    };
}