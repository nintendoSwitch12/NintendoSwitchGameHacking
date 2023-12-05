# Packet Analysis Tip

## 개요

닌텐도에서 주고 받는 패킷을 캡여하고 싶을 때 쓸 수 있다.

닌텐도에서는 와이파이 설정에서 proxy server 설정을 할 수 있다. 여기서 본인이 구축한 proxy server로 설정하면 패킷은 해당 서버로 패킷을 보내게 된다. 이 때 본인이 구축한 proxy server가 cli 환경만 제공한다면 GUI를 사용하는 wireshark를 쓰기 힘들다. 그렇기 때문에 CLI 환경에서 작동하는 tshark를 사용하면 된다.

## Tshark 사용법

```bash
apt install tshark
```

일단 위에 명령어를 통해 tshark를 설치하자

```xml
tshark -D
```

위에 명령어를 실행해서 내가 볼 NIC의 숫자를 기억한다

이제 다음과 같은 옵션을 넣어서 패킷을 캡쳐할 수 있다

```xml
tshark -i 3 -Y 'tcp.port == 80 && ip.addr == 192.168.1.10' -t a --color
```

-i : NIC의 숫자

-Y : 필터 옵션

-t : 시간 옵션

—color : 색 옵션

위에 명령어를 실행하면 /tmp에 .pcapng 파일이 생긴다.