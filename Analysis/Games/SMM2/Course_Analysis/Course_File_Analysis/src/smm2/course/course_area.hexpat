#pragma once

#include "smm2/course/header/course_area_header.hexpat"
#include "smm2/object/object.hexpat"
#include "smm2/sound_effect.hexpat"

namespace smm2::course
{
    struct CourseArea
    {
        header::CourseAreaHeader course_area_hdr;
        object::Object objects[2600];
        sound::SoundEffect sound_effects[300];
        object::SnakeBlock snake_blocks[5];
        object::ClearPipe clear_pipes[200];
        object::PiranhaCreeper piranha_creepers[10];
        object::ExclamationBlock exclamation_blocks[10];
        object::TrackBlock track_blocks[10];
        object::Ground grounds[4000];
        object::Track tracks[1500];
        object::Icicle icicles[300];
        padding[0xdbc];
    };
}