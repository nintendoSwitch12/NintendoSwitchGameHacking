# Overview


**This is an article about the Nintendo Switch save file structure.**

# Content


The format of the save file used by the Nintendo Switch is summarized in switchbrew[1].

# ImHex Pattern


Changing the structure of the save file to a pattern that can be used in ImHex is as follows. 

```
struct AESCMACHeader
{
	char aes_cmac[0x10];
	padding[0xf0];
};

struct DISFHeader
{
	char magic[4];
	u32 version;
	char hash_of_dpfs[32];
	u64 main_data_remap_mapping_table_offset;
	u64 main_data_remap_mapping_table_size;
	u64 meta_data_remap_mapping_table_offset;
	u64 meta_data_remap_mapping_table_size;
	u64 main_data_remap_offset;
	u64 main_data_remap_size;
	u64 duplex_level_1_virtual_offset_a;
	u64 duplex_level_1_virtual_offset_b;
	u64 duplex_level_1_size;
	u64 duplex_level_2_virtual_offset_a;
	u64 duplex_level_2_virtual_offset_b;
	u64 duplex_level_2_size;
	u64 journal_storage_virtual_offset;
	u64 journal_storage_data_size_a;
	u64 journal_storage_data_size_b;
	u64 journal_storage_journal_size;
	u64 duplex_master_bitmap_offset_a;
	u64 duplex_master_bitmap_offset_b;
	u64 duplex_master_bitmap_size;
	u64 ivfc_master_hash_offset_a;
	u64 ivfc_master_hash_offset_b;
	u64 ivfc_master_hash_size;
	u64 journal_block_table_virtual_offset;
	u64 journal_block_table_size;
	u64 virtual_offset_of_bitmap_of_modified_physical_journal_blocks;
	u64 size_of_bitmap_of_modified_physical_journal_blocks;
	u64 virtual_offset_of_bitmap_of_modified_virtual_journal_blocks;
	u64 size_of_bitmap_of_modified_virtual_journal_blocks;
	u64 virtual_offset_of_bitmap_of_free_journal_blocks;
	u64 size_of_bitmap_of_free_journal_blocks;
	u64 ivfc_level_1_virtual_offset;
	u64 ivfc_level_1_size;
	u64 ivfc_level_2_virtual_offset;
	u64 ivfc_level_2_size;
	u64 ivfc_level_3_virtual_offset;
	u64 ivfc_level_3_size;
	u64 file_allocation_table_virtual_offset;
	u64 file_allocation_table_size;
	char index_of_the_active_duplex_master_bitmap;
	u64 file_allocation_table_ivfc_master_hash_offset_a;
	u64 file_allocation_table_ivfc_master_hash_offset_b;
	u64 file_allocation_table_ivfc_level_1_virtual_offset;
	u64 file_allocation_table_ivfc_level_1_size;
	u64 file_allocation_table_ivfc_level_2_virtual_offset;
	u64 file_allocation_table_ivfc_level_2_size;
	padding[0x77];
};

struct DuplexHeader
{
	char magic[4];
	u32 version;
	u64 master_bitmap_offset;
	u64 master_bitmap_size;
	u32 master_bitmap_block_size_power;
	u64 level1_offset;
	u64 level1_size;
	u32 level1_block_size_power;
	u64 level2_offset;
	u64 level2_size;
	u32 level2_block_size_power;
};

struct LevelInformation
{
	u64 offset;
	u64 size;
	u32 block_size_power;
	u32 reserved;
};

struct IntegrityVerificationHeader
{
	char magic[4];
	u32 version;
	u32 master_hash_size;
	u32 number_of_levels;
	LevelInformation level_information[6];
	char salt[32];
	padding[4];
};

struct JournalMapHeader
{
	u32 version;
	u32 main_data_block_count;
	u32 journal_block_count;
	padding[4];
};

struct JournalHeader
{
	char magic[4];
	u32 version;
	u64 total_size;
	u64 journal_size;
	u64 block_size;
	JournalMapHeader journal_map_hdr;
	padding[0x1d0];
};

struct FATHeader
{
	u64 block_size;
	u64 fat_offset;
	u32 fat_entry_count;
	padding[4];
	u64 data_offset;
	u32 data_block_count;
	padding[4];
	u32 directory_table_block_index;
	u32 file_table_block_index;
};

struct SaveHeader
{
	char magic[4];
	u32 version;
	u64 number_of_blocks;
	u64 block_size;
	FATHeader fat_hdr;
};

struct RemapHeader
{
	char magic[4];
	u32 version;
	u32 number_of_remapping_entries;
	u32 number_of_remapping_segments;
	u32 number_of_bits_reserved_for_segment_index_in_virtual_offsets;
	padding[0x2c];
};

struct Unknown
{
	padding[8];
};

enum SaveDataType : char
{
	kSystem = 0,
	kAccount,
	kBcat,
	kDevice,
	kTemporary,
	kCache,
	kSystemBcat
};

enum SaveDataRank : char
{
	kPrimary = 0,
	kSecondary
};

struct SaveDataAttribute
{
	u64 application_id;
	u128 user_id;
	u64 system_save_data_id;
	SaveDataType save_data_type;
	SaveDataRank save_data_rank;
	u16 save_data_index;
	padding[4];
	padding[8];
	padding[8];
	padding[8];
};

struct ExtraData
{
	SaveDataAttribute save_data_attribute;
	u64 save_owner_id;
	u64 timestamp;
	u32 maybe_flags;
	u32 maybe_unused;
	u64 size_of_usable_save_data;
	u64 journal_size;
	u64 commit_id;
	padding[0x190];
};

struct AdditionalStorage
{
	padding[0x3528];
};

struct MainHeader
{
	AESCMACHeader aes_cma_hdr;
	DISFHeader disf_hdr;
	DuplexHeader duplex_hdr;
	IntegrityVerificationHeader ivfc_hdr;
	JournalHeader journal_hdr;
	SaveHeader save_hdr;
	RemapHeader main_data_remap_hdr;
	RemapHeader meta_data_remap_hdr;
	Unknown unknown;
	ExtraData extra_data_a;
	ExtraData extra_data_b;
	AdditionalStorage additional_storage8;
};

MainHeader main_hdr @ 0x00;
MainHeader same_main_hdr @ 0x4000;
```

# Reference



[1] https://switchbrew.org/wiki/Savegames#DISF
