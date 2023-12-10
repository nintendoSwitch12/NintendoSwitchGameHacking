#pragma once
#pragma pattern_limit 0x5BFC0

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

namespace smm2
{
    namespace object
    {
        enum Direction4On4 : u8
        {
            kRight = 0,
            kLeft,
            kUp,
            kDown
        };

        enum Direction16On4 : u8
        {
            kLeft = 1,
            kRight,
            kDown,
            kUp,
            kLeftToDown,
            kDownToLeft,
            kLeftToUp,
            kUpToLeft,
            kRightToDown,
            kDownToRight,
            kRightToUp,
            kUpToRight,
            kRightToEnd,
            kLeftToEnd,
            kUpToEnd,
            kDownToEnd
        };

        enum Direction16On16 : u16
        {
            kLeft = 1,
            kRight,
            kDown,
            kUp,
            kLeftToDown,
            kDownToLeft,
            kLeftToUp,
            kUpToLeft,
            kRightToDown,
            kDownToRight,
            kRightToUp,
            kUpToRight,
            kRightToEnd,
            kLeftToEnd,
            kUpToEnd,
            kDownToEnd
        };
    }
}

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

namespace smm2::object
{
    struct Ground 
    {
        u8 x_position;
        u8 y_position;
        u8 id;
        u8 background_id;
    };
}

namespace smm2::object
{
    enum IcicleType : u8
    {
        kFalling = 0,
        kSolid
    };

    struct Icicle
    {
        u8 x_position;
        u8 y_position;
        IcicleType type;
        u8 reserved;
    };
}

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

namespace smm2::sound
{
    enum SoundEffectIDOn : u8
    {
        kShock = 0,
        kClatter,
        kKick,
        kApplause,
        kGlory,
        kPunch,
        kLaughter,
        kBaby,
        kDingDong,
        kBossMusic,
        kHeartbeat,
        kScream,
        kDrama,
        kJump,
        kCheer,
        kDoom,
        kFireworks,
        kHonkHonk,
        kBzzt,
        kBonusMusic,
        kSilence,
        kUnknown,
        kUnknown,
        kPartyPopper,
        kBooo,
        kGuffaw,
        kNearMiss,
        kUnknown,
        kUnknown,	
        kOink,
        kKuhThunk,
        kBeep,
        kNinjaAttack,
        kUnknown,
        kUnknown,
        kZap,
        kFlash,
        kYeah,
        kAww,
        kUnknown,
        kUnknown,
        kAudience,
        kScatting,
        kSpark,
        kTraditional,
        kElectricGuitar,
        kTwistyTurny,
        kWoozy,
        kFinalBoss,
        kPeaceful,
        kHorror,
        kSuperMarioGalaxy,
        kSuperMario64,
        kSuperMarioSunshine,
        kSuperMarioKart,
        kUnknown
    };

    enum SoundEffectIDOn16 : u16
    {
        kShock = 0,
        kClatter,
        kKick,
        kApplause,
        kGlory,
        kPunch,
        kLaughter,
        kBaby,
        kDingDong,
        kBossMusic,
        kHeartbeat,
        kScream,
        kDrama,
        kJump,
        kCheer,
        kDoom,
        kFireworks,
        kHonkHonk,
        kBzzt,
        kBonusMusic,
        kSilence,
        kUnknown,
        kUnknown,
        kPartyPopper,
        kBooo,
        kGuffaw,
        kNearMiss,
        kUnknown,
        kUnknown,	
        kOink,
        kKuhThunk,
        kBeep,
        kNinjaAttack,
        kUnknown,
        kUnknown,
        kZap,
        kFlash,
        kYeah,
        kAww,
        kUnknown,
        kUnknown,
        kAudience,
        kScatting,
        kSpark,
        kTraditional,
        kElectricGuitar,
        kTwistyTurny,
        kWoozy,
        kFinalBoss,
        kPeaceful,
        kHorror,
        kSuperMarioGalaxy,
        kSuperMario64,
        kSuperMarioSunshine,
        kSuperMarioKart,
        kUnknown
    };

    struct SoundEffect
    {
        SoundEffectIDOn id;
        u8 x_position;
        u8 y_position;
        u8 reserved;
    };
}

namespace smm2
{
    namespace object
    {
        enum ObjectInformation : u16
        {
            Goomba = 0,
            KoopaTroopa,
            PiranhaPlant,
            HammerBro,
            Block,
            QustionBlock,
            HardBlock,
            Ground,
            Coin,
            Pipe,
            Trampoline,
            Lift,
            Thwomp,
            BillBlaster,
            MushroomPlatform,
            Bobomb,
            SemisolidPlatform,
            Bridge,
            PSwitch,
            POWBlock,
            SuperMushroom,
            DonutBlock,
            CloudBlock,
            NoteBlock,
            FireBar,
            Spiny,
            GoalGround,
            GoalPole,
            BuzzyBeetle,
            HiddenBlock,
            Lakitu,
            LakituCloud,
            BanzaiBill,
            LifeUpMushroom,
            FireFlower,
            SuperStar,
            LavaLift,
            StartGround,
            StartSignBoard,
            Magikoopa,
            SpikeTop,
            Boo,
            KoopaClownCar,
            SpikeTrap,
            BigMushroom,
            ShoeGoomba,
            DryBones,
            Cannon,
            Blooper,
            CastleBridge,
            HopChops,
            Skipsqueak,
            Wiggler,
            ConveyorBelt,
            Burner,
            WarpDoor,
            CheepCheep,
            Muncher,
            RockyWrench,
            Track,
            LavaBubble,
            ChainChomp,
            Bowser,
            IceBlock,
            Vine,
            Stingby,
            ArrowSign,
            OneWayWall,
            Grinder,
            Player,
            LargeCoin,
            SemisolidPlatform,
            KoopaTroopaCar,
            Toad,
            Spike,
            Stone,
            Twister,
            BoomBoom,
            Pokey,
            PBlock,
            DashBlock,
            SMB2Mushroom,
            Bumper,
            Skewer,
            SnakeBlock,
            TrackBlock,
            Charvaargh,
            GentleSlope,
            SteepSlope,
            AutoscrollCamera,
            CheckpointFlag,
            Seesaw,
            PinkCoin,
            ClearPipe,
            ConveyorBeltSlope,
            Key,
            AntTrooper,
            WarpBox,
            BowserJr,
            ONOFFSwitch,
            DottedLineBlock,
            LavaEditor,
            MontyMole,
            FishBone,
            AngrySun,
            SwingingClaw,
            Tree,
            PiranhaCreeper,
            BlinkingBlock,
            SoundEffectIcon,
            SpikeBlock,
            Mechakoopa,
            Crate,
            MushroomTrampoline,
            Porcupuffer,
            GoalToadette,
            SuperHammer,
            Bully,
            Icicle,
            ExcalmationBlock,
            Lemmy,
            Morton,
            Larry,
            Wendy,
            Iggy,
            Roy,
            Ludwig,
            CannonBox,
            PropellerBox,
            GoombaMask,
            BulletBillMask,
            RedPOWBox,
            ONOFFTrampoline
        };

        struct Object
        {
            u32 x_position;
            u32 y_position;
            u16 reserved;
            u8 width;
            u8 height;
            u32 flags;
            u32 child_flags;
            u32 extended_data;
            ObjectInformation id;
            u16 child_id;
            u16 link_id;
            sound::SoundEffectIDOn16 sound_effect_id;
        };
    }
}

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

namespace smm2::course
{
    struct Course
    {
        header::CourseHeader course_hdr;
        CourseArea main_course_area;
        CourseArea sub_course_area;
    };
}