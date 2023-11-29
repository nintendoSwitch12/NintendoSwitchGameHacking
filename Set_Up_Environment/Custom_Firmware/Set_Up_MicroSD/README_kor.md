# Micro SD 카드 설정

---

# 개요

닌텐도 스위치에서 커스텀 펌웨어[^1]를 설치하거나 홈브루(Homebrew), 도구 등을 사용할 때는 해당 도구의 실행 파일을 SD 카드에 넣어줄 필요가 있다.

# SD 카드 포맷

닌텐도 스위치에서 SD 카드를 사용할 때는 SD 카드의 파일 시스템을 FAT32로 설정해야 하는 것이 좋다[^2]. 따라서 본인의 SD 카드의 파일 시스템이 FAT32가 아닐 경우, HP USB Disk Sotrage Format Tool 등을 사용하여 FAT32로 포맷을 할 수 있다[^3].

# SD 카드 구성

SD 카드를 통하여 툴을 사용하려고 할 때 SD 카드의 구조는 다음과 같아야 한다.

```
F:.
├─atmosphere
│  ├─config
│  ├─config_templates
│  ├─fatal_errors
│  ├─flags
│  ├─hbl_html
│  │  └─accessible-urls
│  └─kip_patches
├─bootloader
│  ├─ini
│  ├─payloads
│  ├─res
│  └─sys
│      └─l4t
└─switch
    └─appstore
        └─.get
```

위처럼 배치하는 방법에는 케피르[^4]와 같은 올인원 패키지를 설치하는  방법도 있지만 이 문서에서는 SD Setup을 이용한 방식을 설명할 것이다.

## SD Setup 이용

닌텐도 스위치에서 CFW를 사용하는 유저가 많고 누군가가 SD 카드 구성을 도와주는 사이트를 만들었다. SD Setup이라는 사이트에서 사용하고자 하는 툴을 선택하면 약간의 작업만으로 원하는 파일들로 SD 카드를 구성할 수 있다[^5]. SD Setup 사이트에 들어가면 다음과 같은 화면이 나타난다.

![Untitled](Micro%20SD%20%E1%84%8F%E1%85%A1%E1%84%83%E1%85%B3%20%E1%84%89%E1%85%A5%E1%86%AF%E1%84%8C%E1%85%A5%E1%86%BC%2060144359842e4abab2dd1571f231b8ed/Untitled.png)

여기서 원하는 툴들을 선택하고 화면 하단의 “Download your ZIP”을 클릭하여 파일을 다운로드 받은 뒤, 다운 받은 압축 파일에서 /payloads에 있는 파일들을 /sd/bootloader로 옮긴다. 그 뒤, /sd/*을 FAT32로 포맷한 SD 카드로 옮기면 SD 카드 구성이 끝난다. 

## 업데이트

만약, SD Setup에서 다운로드 받은 툴이 최신 버전이 아닐 경우 직접 최신 버전을 받아 교체해주면 된다. 각 툴마다 설치 방법이 다를 수 있으므로 각 툴의 다운로드 사이트를 참고하여 교체하여야 한다.

[^1]: https://wiki.gbatemp.net/wiki/List_of_Switch_custom_firmwares
[^2]: https://www.reddit.com/r/NintendoSwitch/comments/ekzbdo/exfat_vs_fat_32_whats_safest/
[^3]: https://blog.kakaocdn.net/dn/ub7ri/btqIwEuwnvs/PSQ1wHFZh9EtIiqqYTQMc1/HPUSBDisk.exe?attach=1&knm=tfile.exe
[^4]: https://codeberg.org/rashevskyv/kefir/releases
[^5]: https://www.sdsetup.com/console?switch

