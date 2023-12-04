# 개요

이 글에서는 슈퍼 마리오 메이커 2에서 사용하는 썸네일 파일의 구조를 분석한다.

# 구조

## 암호화 구조

썸네일 파일은 Course 생성 시 동시에 생성되며, Course와 마찬가지로 암호화되어 있다. 

```cpp
struct EncrpytedThumbnail
{
	struct EncrpytedData encrpyted_data;
	struct EncryptionConfig encryption_config;
}

struct EncryptedData
{
	char data[0x1BFD0];
}

struct EncrpytionConfig
{
	u32 aes_iv[4];
	u32 random_seed[4];
	u32 aes_cmac_over_decrpyted_file[4];
}
```

## 실제 구조

위 구조를 이해하기 쉽게 정리하면 아래와 같다. 

썸네일 파일은 Course 파일과 마찬가지로 크기가 정해져 있으며, 그 크기는 0x1BFD0 Byte이다. JPEG 포맷에 무결성 검사를 위한 Hash 값과 Seed 값을 덧붙인 형태이다. 

![](./img/1.png)

```cpp
struct Thumbnail // size is always 0x1BFD0
{
	char jpeg_data[0x0001BF9C];
	int size_of_jpeg_data; // this is always 0x0001BF9C
	char hmac_sha256_of_jpeg_data[0x20];
	char seed_for_hmac_sha256[0x10];
}
```

# 파일 포맷 적합화

임의의 썸네일 파일을 슈퍼 마리오 메이커2에 사용하기 위해서는 해당 파일 포맷을 맞춰주어야 한다.  아래는 그 과정을 서술한다. 

## key 계산

썸네일 파일 포맷의 seed를 통해 HMAC 검증값에 사용할 키 값을 계산할 수 있다. 계산 코드는 아래와 같다. 

```cpp
def generate_hash_key(rand: sead.Random):
		fixed_value = [0x39B399D2, 0xFAE40B38, 0x851BC213, 0x8CB4E3D9, 
		            0x7ED1C46A, 0xE8050462, 0xD8D24F76, 0xB52886FC, 
		            0x67890BF0, 0xF5329CB0, 0xD597FB28, 0x2B8EE0EA, 
		            0x47574C51, 0xF7569D9, 0xCF1163AE, 0xE4A153BF, 
		            0xD1FAE468, 0xD4C64738, 0x360106F5, 0xDD7EB113, 
		            0xC296F3E2, 0x2C58F258, 0x79B554E1, 0x85DF9D06, 
		            0xAA307330, 0x1410F69, 0xB2F2C573, 0x82B93EB1, 
		            0xF351A11C, 0x63098693, 0x885B5DA5, 0x8872A8ED, 
		            0xACD9CB13, 0xED7FBCAD, 0xE6A41EC2, 0x5F44E79F, 
		            0x8346F5B5, 0x389FE6ED, 0x507124B5, 0xE9B23EAA, 
		            0x577113F0, 0xA95ED917, 0x2F62D158, 0x47843F86, 
		            0xC65637D0, 0x2F272052, 0xBA4A4CC4, 0xB5F146F6, 
		            0x501B87A7, 0x51FC3A93, 0x6EDE3F02, 0x3D265728, 
		            0x9B809440, 0x75B89229, 0xF6A280CC, 0x8537FA68, 
		            0x5B5ED19A, 0x6FC05BB6, 0xF4EF5261, 0xAA1B7D4F, 
		            0xFCB26110, 0xAD3D74, 0xC0E73A4B, 0xF132E7C7]
		hmac_key = [0, 0, 0x82A28C10, 0, 1]
		
		for i in range(4):
		    for _ in range(3):
		        t = fixed_value[(rand.u32() >> 26)&0xffffffff]
		        hmac_key[i] = ((t >> ((rand.u32() >> 27) & 0x18)) & 0xff | hmac_key[i]) << 8
		    t = fixed_value[(rand.u32() >> 26)&0xffffffff]
		    hmac_key[i] = ((t >> ((rand.u32() >> 27) & 0x18)) & 0xff | hmac_key[i]) & 0xffffffff
		
		return struct.pack('<IIII', hmac_key[0], hmac_key[1], hmac_key[2], hmac_key[3])
```

## SHA-256 with HMAC

SHA-256 with HMAC은 닌텐도 자체 규격을 쓰기 때문에 계산 방법을 분석하지 않고는 계산할 수 없다. 따라서, 아래와 같은 방식으로 재현할 필요가 있다. 

1. 게임과 함께 첨부된 SDK를 리버싱하여 똑같은 동작을 구현
2. SDK에 포함된 함수를 실행할 수 있게 처리 
3. switchbrew의 [libnx](https://github.com/switchbrew/libnx) 등을 사용

![](./img/2.png)

# 암호화

실제 세이브 파일에 썸네일을 덮어쓰기 위해서는 암호화 역시 똑같이 거칠 필요가 있다. 해당 암호화 과정은 JiXiaomai의 github에 공개되어 있다.[https://github.com/JiXiaomai/SMM2]

[SMM2 Course 암호화 및 복호화 방법](../Course_Analysis/Course_Encryption_Decryption/README_kor.md) 에 자세히 서술되어 있다.
