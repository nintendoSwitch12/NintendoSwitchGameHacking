# Overview

This article talks about encryption and decryption of Course File in Super Mario Maker 2.

# Encryption Format

The encrypted Course file is divided into 3 parts: Save Header, Encrypted Data, and Encryption Config. The following shows the structure of each part. 

```cpp
struct EncrpytedCourse
{
	struct SaveHeader save_hdr;
	struct EncryptedData encrypted_data;
	struct EncryptionConfig encryption_config;
}

struct SaveHeader
{
	u32 format_version;
	u16 save_type;
	u16 unknown;
	u32 crc32_over_decrypted_file;
	char magic[4];
}

struct EncryptedData
{
	char data[0x5BFC0];
}

struct EncrpytionConfig
{
	u32 aes_iv[4];
	u32 random_seed[4];
	u32 aes_cmac_over_decrpyted_file[4];
}
```

# Installation

## Prerequisite

Out of many encryption/decryption tools, we used SMM2 python package made by JiXiaomai[1]. This package requires an another package called “nintendo” which is an old version of NintendoClients[2]. 

Therefore, you have to first install NintendoClients’ old version as below.

```
git clone https://github.com/kinnay/NintendoClients.git 
cd NintendoClients/
git checkout old 
pip install .
```

## Installation

To install JiXiaomai’s SMM2 python package[1], follow the commands below.

```
git clone https://github.com/JiXiaomai/SMM2.git 
cd SMM2/
pip install .
```

# Encryption/Decryption

To encrypt/decrypt the course file, use the code below

```
from SMM2 import encryption
from SMM2 import course

data = open("course_data_000.bcd", "rb").read()

Course = encryption.Course()
Course.load(data)
Course.decrypt()
# or
# Course.encrypt()

open("course_data_000.bcd", "wb").write(Course.data)
```

# Reference

[1] https://github.com/JiXiaomai/SMM2

[2] https://github.com/kinnay/NintendoClients/tree/old
