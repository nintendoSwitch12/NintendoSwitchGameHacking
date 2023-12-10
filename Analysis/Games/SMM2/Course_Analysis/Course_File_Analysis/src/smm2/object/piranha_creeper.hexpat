#pragma once

#include "smm2/object/direction.hexpat"

namespace smm2::object
{
    struct PiranhaCreeperNode
    {
        u8 unknown;
        Direction16On4 direction;
        u16 reserved;
    };

    struct PiranhaCreeper
    {
        u8 unknown;
        u8 index;
        u8 node_count;
        u8 reserved;
        PiranhaCreeperNode piranha_creeper_nodes[20];
    };
}