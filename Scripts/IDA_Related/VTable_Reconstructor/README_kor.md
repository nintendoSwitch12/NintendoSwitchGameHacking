# 개요

Splatoon 2의 초기 버전은 각종 함수와 클래스의 심볼이 바이너리에 남아있다. 따라서 해당 버전들에서 사용된 클래스의 VTable을 Struct로 저장하여 다른 게임을 분석할 때 사용하고자 하였다. 이 글은 스플래툰 2의 VTable을 구조체 형식으로 추출하는 방법을 다룬다.

# HexRaysCodeXplorer

IDA 플러그인 중에 데이터 영역에 존재하는 VTable들과 클래스들을 Reconstruct 해주는 HexRaysCodeXplorer라는 플러그인이 존재한다[^1]. 이 플러그인을 사용하면, 심볼이 있는 클래스의 경우, 해당 클래스를 나타내는 구조체와 VTable을 나타내는 구조체를 만들어 준다. 

하지만, 스플래툰 2에서 직접 사용한 결과 VTable에 저장된 method의 이름이 Mangling되어 있다.

따라서 mangling된 이름을 demangle하는 루틴을 추가하여 사용하였다.

# Demangle 되게 수정

## Naming Routine

각 클래스의 VTable의 method 이름을 정하는 루틴은 GCCObjectFormatParser.cpp의 buildReconstructableTypesRecursive 함수에 존재한다. 이 함수에서는 IDA SDK를 사용하여 VTable이 저장된 위치의 이름을 가져와 그 이름을 structure의 멤버로 저장한다.

## 수정 방법

GCCObjectFormatParser.cpp의 buildReconstructableTypesRecursive 함수에 Demangle 하는 루틴을 추가해주면 된다. Demangle 하는 함수는 IDA SDK에 구현되어 있으므로 이 함수를 사용하면 된다[^2]. 

다만, 멤버의 이름은 똑같을 수 없지만, Demangle된 이름은 똑같을 수 있으므로 만약 같은 이름을 가질 경우, ‘_’를 붙이게 수정하였다. 또한 IDA의 구조체의 멤버의 이름에는 다음의 특수 문자들이 들어갈 수 없으므로, 이를 다른 문자로 바꾼다.

- `,` → `_C_` (Comma)
- `` → `_S_` (Space)
- `~` → `_T_` (Tilde)
- `*` → `_P_` (Pointer)
- `=` → `_E_` (Equal)
- `<` → `_L_` (Little)
- `>` → `_G_` (Greater)

## 수정된 소스 코드

### with args

[With args](./src/with_args/)

### without args

[Without args](./src/without_args/)

# 후처리

만들어진 VTable을 추출하고 다른 바이너리에서 로딩하려고 하면, 구조체 이름에 `<`나  ``, `,`, `*` 등이 포함되어 있어 불러올 수 없다. 따라서 이러한 문자가 포함된 구조체의 이름을 바꿔주는 스크립트를 작성하였다.
[Script](./src/post_process.py)

[^1]:https://github.com/zengfr/HexRaysCodeXplorer_plugin_for_ida_pro
[^2]: https://www.hex-rays.com/products/ida/support/idadoc/272.shtml