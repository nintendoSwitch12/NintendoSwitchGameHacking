#include "smm2/object/direction.hexpat"
#include "smm2/object/clear_pipe.hexpat"
#include "smm2/object/exclamation_block.hexpat"
#include "smm2/object/ground.hexpat"
#include "smm2/object/icicle.hexpat"
#include "smm2/object/piranha_creeper.hexpat"
#include "smm2/object/snake_block.hexpat"
#include "smm2/object/track_block.hexpat"
#include "smm2/object/track.hexpat"
#include "smm2/sound_effect.hexpat"

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