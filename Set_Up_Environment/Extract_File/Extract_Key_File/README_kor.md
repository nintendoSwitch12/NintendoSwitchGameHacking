# 키 파일 추출

---

# 개요

닌텐도 스위치에서 추출한 게임이나 펌웨어를 분석 / 에뮬레이팅하기 위해서는 prod.keys와 title.keys(이하 키)를 추출해야 할 필요가 있다. 이 글에서는 Hekate가 부팅된 닌텐도 스위치에서 키를 추출하는 방법을 설명한다.

# 키 파일 추출법

닌텐도 스위치에서 키를 추출하기 위해 Lockpick_RCM를 사용한다[^1].

## Lockpick_RCM 설치

Lockpick_RCM을 다운로드 받은 뒤, Lockpick_RCM.bin을 SD 카드의 bootloader/payloads에 넣으면 된다.

## Lockpick_RCM 사용법

hekate를 부팅한 뒤, payload ->Lockpick_RCM.bin을 실행한다. 그 뒤, “Dump from SysNAND”나 “Dump from EmuNAND” 중 하나를 택하여 키를 추출한다. 추출된 파일은 SD카드의 switch/*.keys이다.

![LockPick_Rcm](img/LockPick_Rcm.jpeg)

[^1]: https://github.com/Decscots/Lockpick_RCM