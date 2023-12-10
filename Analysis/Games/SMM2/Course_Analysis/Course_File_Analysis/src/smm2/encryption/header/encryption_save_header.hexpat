#pragma once

namespace smm2::encryption
{
    enum SaveType : u16
    {
        kQuest = 0,
        kNetwork = 8,
        kLater = 10,
        kSave = 11,
        kCourse = 16
    };

    struct EncriptionHeader
    {
        u32 format_version;
        SaveType save_type;
        u16 unknown;
        u32 crc32_over_decrypted_file;
        char magic[4];
    };
}