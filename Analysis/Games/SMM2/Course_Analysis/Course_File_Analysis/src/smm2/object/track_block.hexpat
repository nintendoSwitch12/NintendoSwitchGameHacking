#pragma once

#include "smm2/object/direction.hexpat"

namespace smm2::object
{
    struct TrackBlockNode
    {
        u8 unknown;
        Direction16On4 direction;
        u16 reserved;
    };

    struct TrackBlock
    {
        u8 unknown;
        u8 index;
        u8 node_count;
        u8 reserved;
        TrackBlockNode track_block_nodes[10];
    };
}