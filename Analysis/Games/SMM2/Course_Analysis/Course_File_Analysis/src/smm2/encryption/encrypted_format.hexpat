#pragma once

#include "smm2/encryption/header/encryption_save_header.hexpat"

#include <std/mem.pat>

namespace smm2::encryption
{
    struct EncryptionInformation
    {
        u32 aes_iv[4];
        u32 random_seed[4];
        u32 aes_cmac_over_decrypted_file[4];
    };

    struct EncryptedFile
    {
        EncriptionHeader encryption_hdr @ 0x00;
        EncryptionInformation encryption_information @ (std::mem::size() - 0x30);
    };
}