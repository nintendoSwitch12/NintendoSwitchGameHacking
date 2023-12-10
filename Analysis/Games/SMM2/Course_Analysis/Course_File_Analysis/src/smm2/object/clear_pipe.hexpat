#pragma once

#include "smm2/object/direction.hexpat"

namespace smm2::object
{
    enum ClearPipeNodeType : u8
    {
        kClosedOnOppositeDirection = 0,
        kClosedOnDirection,
        kOpen,
        kUnknown
    };

    struct ClearPipeNode
    {
        ClearPipeNodeType type;
        u8 index;
        u8 x_position;
        u8 y_position;
        u8 width;
        u8 height;
        u8 unknown;
        Direction4On4 direction;
    };

    struct ClearPipe
    {
        u8 index;
        u8 node_count;
        u8 unknown;
        u8 reserved;
        ClearPipeNode clear_pipe_nodes[36];
    };
}