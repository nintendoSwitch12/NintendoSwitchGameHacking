# Course Format

Course Format에 관한 정보는 liamadvance에 의해 알려져 있다[1]. 

## 수정 사항

최신 버전에는 liamadvance에 의해 알려진 것과는 다른 점들이 존재한다. 그 사항들은 다음과 같다.

- Snake Block
    - Unknown 제거됨
    - Reserved 제거됨
    - Snake Block Node 이후에 padding 1 바이트 존재

## ImHex Pattern

알려진 Course Format을 ImHex에서 쓸 수 있는 Pattern으로 만들었고, 그 Pattern은 [여기서[2]](https://drive.google.com/drive/u/0/folders/1XyxVwIbPITqHdOUH-pq4ehZwSfhWvmOx) 다운로드 받을 수 있다. 

### 설치법

다운로드 받은 압축 파일은 다음 구조로 이루어져 있다.

```
smm2
 ├─course
 │  └─header
 ├─encryption
 │  └─header
 └─object
```

smm2라는 폴더가 ImHex 설치 폴더(C:\Program Files\ImHex)의 includes에 존재하게 놔두면 된다. 적용이 된 후 폴더 구조는 다음과 같다.

```
includes
 ├─hex
 │  ├─impl
 │  └─type
 ├─smm2
 │  ├─course
 │  │  └─header
 │  ├─encryption
 │  │  └─header
 │  └─object
 ├─std
 └─type
     └─types
```

### 사용법

분석하고 싶은 Course(BCD) 파일을 열고, 다음과 같은 Pattern을 사용하면 된다.

```
#include "smm2/main.hexpat"

smm2::course::Course course @ 0x00;
```

# 참고

[1] https://github.com/liamadvance/smm2-documentation

[2] https://drive.google.com/drive/u/0/folders/1XyxVwIbPITqHdOUH-pq4ehZwSfhWvmOx

# 테스트 파일

[map_for_clearpipe_and_piranhacreeper.bcd](./map/map_for_clearpipe_and_piranhacreeper.bcd)

[map_for_exclamation.bcd](./map/map_for_exclamation.bcd)

[map_for_ground.bcd](./map/map_for_ground.bcd)

[map_for_icicle.bcd](./map/map_for_icicle.bcd)

[map_for_snake_and_sound.bcd](./map/map_for_snake_and_sound.bcd)

[map_for_trackblock.bcd](./map/map_for_trackblock.bcd)

[map_for_tracks.bcd](./map/map_for_tracks.bcd)
