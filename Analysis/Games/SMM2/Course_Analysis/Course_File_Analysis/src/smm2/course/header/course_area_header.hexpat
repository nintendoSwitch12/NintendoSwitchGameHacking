#pragma once

namespace smm2::course::header
{
    enum ThemeType : u8
    {
        kGround = 0,
        kUnderground,
        kCastle,
        kAirship,
        kUnderwater,
        kGhostHouse,
        kSnow,
        kDesert,
        kSky,
        kForest
    };

    enum AutoscrollType : u8
    {
        kNone = 0,
        kSlow,
        kNormal,
        kFast,
        kCustom
    };

    enum BoundaryFlag : u8
    {
        kBuiltAboceLine = 0,
        kBuiltUnderLine
    };

    enum Orientation : u8
    {
        kHorizontal = 0,
        kVertical
    };

    enum LiquidMode : u8
    {
        kStatic = 0,
        kRisingOrFalling,
        kRisingAndFalling
    };

    enum LiquidSpeed : u8
    {
        kNone = 0,
        x1,
        x2,
        x3
    };

    enum CourseAreaHeaderFlag : u32
    {
        kUnknown = 0,
        kNightTime,
        Unknown
    };

    struct CourseAreaHeader
    {
        ThemeType theme;
        AutoscrollType autoscroll_type;
        BoundaryFlag boundary_flags;
        Orientation orientation;
        u8 end_liquid_height;
        LiquidMode liquid_mode;
        LiquidSpeed liquid_speed;
        u8 start_liquid_height;
        u32 right_boundary;
        u32 top_boundary;
        u32 left_boundary;
        u32 bottom_boundary;
        CourseAreaHeaderFlag flags;
        u32 object_count;
        u32 sound_effect_count;
        u32 snake_block_count;
        u32 clear_pipe_count;
        u32 piranha_creeper_count;
        u32 exclamation_block_count;
        u32 track_block_count;
        u32 reserved;
        u32 ground_count;
        u32 track_count;
        u32 icicle_count;
    };
}