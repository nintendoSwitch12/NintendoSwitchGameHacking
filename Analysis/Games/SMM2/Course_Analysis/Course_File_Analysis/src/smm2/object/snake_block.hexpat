#pragma once

#include "smm2/object/direction.hexpat"

namespace smm2::object
{
    struct SnakeBlockNode
    {
        be u16 index;
        be Direction16On16 direction;
        u16 unknown;
        u16 reserved;
    };

    struct SnakeBlock
    {
        u8 index;
        u16 node_count;
        SnakeBlockNode snake_block_nodes[120];
        padding[1];
    };
}