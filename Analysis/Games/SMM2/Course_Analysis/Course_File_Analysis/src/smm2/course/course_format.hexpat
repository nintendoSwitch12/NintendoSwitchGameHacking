#pragma once
#pragma pattern_limit 0x5BFC0

#include "smm2/course/header/course_header.hexpat"
#include "smm2/course/course_area.hexpat"

namespace smm2::course
{
    struct Course
    {
        header::CourseHeader course_hdr;
        CourseArea main_course_area;
        CourseArea sub_course_area;
    };
}