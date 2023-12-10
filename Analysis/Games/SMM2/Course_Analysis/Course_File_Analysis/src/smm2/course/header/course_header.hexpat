#pragma once

namespace smm2::course::header
{
    enum CustomAutoscrollSpeed : u8
    {
        x1 = 0,
        x2,
        x3
    };

    enum ClearConditionCategory : u8
    {
        kNone = 0,
        kParts,
        kStatus,
        kAction
    };

    enum ClearConditionCRC32 : u32
    {
        kReachTheGoalWithoutTakingDamage = 0x1664515A,
        kReachTheGoalOnALakitusCloud = 0x4C8772A3
    };

    enum GameVersion : u32
    {
        k100 = 0,
        k101,
        k110,
        k200,
        k300,
        k301
    };

    enum ClearCheckGameVersion : u32
    {
        k100 = 0,
        k101,
        k110,
        k200,
        k300,
        k301
    };

    enum ManagementFlag : u32
    {
        kNeedsToBeSet = 0,
        kHasPassedClearCheck,
        kUnknown,
        kUsedOnlyForLessonAndQuestCourses,
        kCantUploadCourse,
        kHasClearConditionAmount,
        kHasChosenSubAreaOrientation,
        Unknown
    };

    struct CourseHeader
    {
        u8 start_y_position;
        u8 goal_y_position;
        u16 goal_x_position;
        u16 timer;
        u16 clear_condition_amount;
        u16 last_saved_year;
        u8 last_saved_month;
        u8 last_saved_day;
        u8 last_saved_hour;
        u8 last_saved_minute;
        CustomAutoscrollSpeed custom_autoscroll_speed;
        ClearConditionCategory clear_condition_category;
        ClearConditionCRC32 clear_condition_crc32;
        GameVersion game_version;
        ManagementFlag management_flags;
        u32 clear_check_attempts;
        u32 clear_check_time;
        u32 creation_id;
        u64 upload_id;
        ClearCheckGameVersion clear_check_game_version;
        padding[0xbc];
        u8 unknown;
        char game_style[3];
        char name[0x42];
        char description[0xca];
    };
}