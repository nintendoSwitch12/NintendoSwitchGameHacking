#pragma once

#include "smm2/object/direction.hexpat"

namespace smm2::object
{
    struct ExclamationBlockNode
    {
        u8 unknown;
        Direction16On4 direction;
        u16 reserved;
    };

    struct ExclamationBlock
    {
        u8 unknown;
        u8 index;
        u8 node_count;
        u8 reserved;
        ExclamationBlockNode exclamation_block_nodes[10];
    };
}