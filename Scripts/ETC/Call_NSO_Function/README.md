# 개요

NSO 파일은 닌텐도 스위치에서 실행되는 실행 파일이다. 닌텐도 스위치는 AARCH64 기반이다. 따라서 NSO 파일을 QEMU 등을 통해 실행할 수 있을 것이라고 생각하였다. 

이 글은 NSO 파일, 그 중에서도 SDK 파일에 존재하는 함수를 QEMU를 통해 실행하는 방법을 서술한다.

# NSO 파일

NSO 파일의 포맷은 SwitchBrew에서 찾을 수 있다[^1]. 해당 글에 따르면 NSO 파일은 생략된 내용이 있는 ELF 파일이다. 

NSO 파일은 SwitchIDAProLoader[^2]를 통해 IDA에 로드할 수 있고 분석할 수 있다.

# 실행 방법

NSO 파일의 함수를 QEMU에서 실행하기 위한 과정은 다음과 같다.

1. NSO 파일을 스위치에 로드한 뒤, 로드된 메모리를 전부 덤프한다.
    1. IDA에 로드해도 되며, 그러기 위해 SwitchIDAProLoader를 설치하고 바이너리를 로드한다.
    2. IDA Python 혹은 IDC를 통해 로드된 NSO 파일을 전부 덤프한다.
2. main.c에 원하는 코드를 작성하고 이를 AARCH64로 컴파일한다.
3. 만약, SDK 함수 실행 중 에러가 난다면 코드 패치를 통해 에러를 수정한다.

# 예시

SDK 함수의 GenerateHmacSha256Mac을 호출하여, 썸네일 파일의 해시를 구하는 예제는 다음 주소에서 찾을 수 있다.

[Example: Call GenerateHmacSha256Mac](./src/example/)

[^1]: https://switchbrew.org/wiki/NSO
[^2]: https://github.com/pgarba/SwitchIDAProLoader
