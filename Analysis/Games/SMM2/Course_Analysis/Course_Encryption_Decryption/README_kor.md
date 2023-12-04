# 개요

이 글에서는 SMM2에서 Course 파일을 암호화하고 복호화하는 방법을 설명하는 글이다.

# 암호화 포맷

암호화된 Course 파일은 크게 세 부분, Save Header, Encrypted Data, Encryption Config로 나눌 수 있다. 각 부분을 구조체로 나타내면 다음과 같다.

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

# 설치 방법

## 사전 준비

암/복호화하는 툴은 많지만, 이 글에서는 JiXiaomai의 SMM2[1]라는 파이썬 패키지을 사용한다. 이 패키지에서는 nintendo라는 패키지를 요구하며 이 패키지는 NintendoClients의 옛날 버전[2]이다. 

따라서 해당 패키지를 설치해야 하며, 설치하는 스크립트는 다음과 같다.

```
git clone https://github.com/kinnay/NintendoClients.git 
cd NintendoClients/
git checkout old 
pip install .
```

## 설치 방법

JiXiaomai의 SMM2[1]를 설치하기 위해서는 다음과 같은 스크립트를 입력하면 된다.

```
git clone https://github.com/JiXiaomai/SMM2.git 
cd SMM2/
pip install .
```

# 암/복호화 방법

다음과 같은 코드를 작성하면 된다.

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

# 참고자료

[1] https://github.com/JiXiaomai/SMM2

[2] https://github.com/kinnay/NintendoClients/tree/old
