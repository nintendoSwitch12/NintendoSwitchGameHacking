# Table of Contents
- [Table of Contents](#table-of-contents)
- [Syscall Tracer](#syscall-tracer)
  - [Overview](#overview)
  - [Features](#features)
  - [Usage](#usage)
  - [How to find syscall wrapper](#how-to-find-syscall-wrapper)
  - [Trouble shooting](#trouble-shooting)


# Syscall Tracer

## Overview

```C++
__int64 __fastcall nn::sf::hipc::Receive(_BYTE *a1, int a2, __int64 a3, __int64 a4)
{
  __int64 result; // x0
  char v6[4]; // [xsp+Ch] [xbp-14h] BYREF
  int v7; // [xsp+18h] [xbp-8h] BYREF
  char v8[4]; // [xsp+1Ch] [xbp-4h] BYREF

  v7 = a2;
  if ( _ReadStatusReg(ARM64_SYSREG(3, 3, 13, 0, 3)) != a3 )
  {
    LODWORD(result) = sub_710037ECEC(v6, a3, a4, &v7, 1LL, 0LL, -1LL);
    if ( (_DWORD)result )
      goto LABEL_3;
LABEL_6:

_DWORD *__fastcall sub_710037ECEC(_DWORD *result, int a2)
{
  __asm { SVC             0x44 ; 'D' }
  *result = a2;
  return result;
}

```

```
.text:000000710037ECE8                 RET
.text:000000710037ECE8 ; End of function sub_710037ECD4
.text:000000710037ECE8
.text:000000710037ECEC
.text:000000710037ECEC ; =============== S U B R O U T I N E =======================================
.text:000000710037ECEC
.text:000000710037ECEC
.text:000000710037ECEC ; _DWORD *__fastcall sub_710037ECEC(_DWORD *result, int)
.text:000000710037ECEC sub_710037ECEC                          ; CODE XREF: nn::sf::hipc::Receive(bool *,nn::svc::Handle,void *,ulong)+44↑p
.text:000000710037ECEC                                         ; nn::sf::hipc::Receive(nn::sf::hipc::ReceiveResult *,nn::svc::Handle,void *,ulong)+44↑p ...
.text:000000710037ECEC
.text:000000710037ECEC var_10          = -0x10
.text:000000710037ECEC
.text:000000710037ECEC                 STR             X0, [SP,#var_10]!
.text:000000710037ECF0                 SVC             0x44 ; 'D'
.text:000000710037ECF4                 LDR             X2, [SP,#0x10+var_10]
.text:000000710037ECF8                 STR             W1, [X2]
.text:000000710037ECFC                 ADD             SP, SP, #0x10
.text:000000710037ED00                 RET
.text:000000710037ED00 ; End of function sub_710037ECEC
.text:000000710037ED00
.text:000000710037ED04
.text:000000710037ED04 ; =============== S U B R O U T I N E =======================================
.text:000000710037ED04
.text:000000710037ED04
.text:000000710037ED04 ; _DWORD *__fastcall sub_710037ED04(_DWORD *result, _DWORD *, int)
.text:000000710037ED04 sub_710037ED04                          ; CODE XREF: .text:000000710032EAC8↑p
.text:000000710037ED04
.text:000000710037ED04 var_10          = -0x10
.text:000000710037ED04 var_8           = -8
.text:000000710037ED04
.text:000000710037ED04                 STP             X0, X1, [SP,#var_10]!
.text:000000710037ED08                 SVC             0x45 ; 'E'
.text:000000710037ED0C                 LDR             X3, [SP,#0x10+var_10]
.text:000000710037ED10                 STR             W1, [X3]
.text:000000710037ED14                 LDR             X3, [SP,#0x10+var_8]
.text:000000710037ED18                 STR             W2, [X3]
.text:000000710037ED1C                 ADD             SP, SP, #0x10
.text:000000710037ED20                 RET
.text:000000710037ED20 ; End of function sub_710037ED04
.text:000000710037ED20
.text:000000710037ED24
.text:000000710037ED24 ; =============== S U B R O U T I N E =======================================
.text:000000710037ED24
.text:000000710037ED24
.text:000000710037ED24 ; void sub_710037ED24()
.text:000000710037ED24 sub_710037ED24                          ; CODE XREF: nn::os::detail::UnsafeMemoryManagerImplByHorizon::AllocateUnsafeMemoryImpl(ulong *,ulong)+60↑p
.text:000000710037ED24                 SVC             0x48 ; 'H'
.text:000000710037ED28                 RET
.text:000000710037ED28 ; End of function sub_710037ED24
.text:000000710037ED28
.text:000000710037ED2C
.text:000000710037ED2C ; =============== S U B R O U T I N E =======================================
.text:000000710037ED2C
.text:000000710037ED2C
.text:000000710037ED2C ; void sub_710037ED2C()
.text:000000710037ED2C sub_710037ED2C                          ; CODE XREF: nn::os::detail::UnsafeMemoryManagerImplByHorizon::AllocateUnsafeMemoryImpl(ulong *,ulong)+EC↑p
.text:000000710037ED2C                                         ; .text:0000007100328EEC↑p
.text:000000710037ED2C                 SVC             0x49 ; 'I'
.text:000000710037ED30                 RET
```

nintendo에서 게임을 실행할 때는 sdk도 메모리에 같이 로드된다. 이는 일종의 라이브러리같은 역할이다. SDK 영역에 연속적으로 syscall wrapper가 있다.

이 부분을 호출하는 함수를 확인하면, 어떤 함수에서 어떤 기능을 이용하는지 알 수 있을 것이라고 생각하여 이 스크립트를 개발했다.

When you run a game on Nintendo, the SDK is also loaded into memory. It acts as a kind of library. In the SDK area, there is a series of syscall wrappers.

I developed this script because I thought that if I could see which functions were calling this part, I could figure out which functions were using which features.

## Features

1. Nintendo SDK 영역에 syscall wrapper 존재한다. 이 영역을 자동으로 syscall 함수로 인식시키고 이름을 변경한다.
2. bp를 걸고, 그 bp에 콜백 함수를 지정해서 그 wrapper 함수를 호출하는 모든 함수들에 대해 콜스택을 수집하고 결과를 미리 저장한다.
3. 결과를 조회하거나, 추적이후 원래대로 상태를 되돌릴 수 있다.

1. syscall wrapper exists in the Nintendo SDK area. Automatically recognize this area as the syscall function and rename it.
2. Call a BP, assign a callback function to the BP to collect the callstack for all functions that call the wrapper function and save the result in advance.
3. retrieve the results, or revert to the original state after tracing.

## Usage

1. 게임을 실행
2. ida에서 gdb attach
3. Edit → Segments → Delete segment 
4. 소스코드 main 함수에서 원하는 게임의 syscall wrapper가 시작하는 위치하고 끝나는 위치는 startSys와 endSys에 적는다.
5. 같은 main 함수에 target 리스트에 원하는 syscall 번호를 넣는다 ( ex: 0x21(SendSyncRequest) )
    1. 혹은 getSysNum(syscall_name: str) 함수를 사용해도 된다.
6. File → script file → systracer 파일 실행  (bp가 걸려야함)
7. Debugger → Debugger Windows → Thread list
8. 원하는 thread가 생길 때까지 게임을 실행
9. 원하는 thread를 클릭하고 f9로 실행
10. SIGTRAP이 뜨면 edit exception눌러서 suspend program을 uncheck
11. 캡쳐된 call stack을 보고 싶으면 Output view에서 gdb를 python으로 바꾸고tracer.listAllCallStack() 입력
12. 특정 syscall의 call stack만 보고 싶으면 tracer.viewCallStack(33) 이렇게 원하는 syscall 숫자만 넣음

1. run the game
2. gdb attach from ida
3. Edit → Segments → Delete segment 
4. in the main function of the source code, write down where the syscall wrapper of the desired game starts and ends in startSys and endSys.
5. in the same main function, enter the desired syscall number in the target list (ex: 0x21(SendSyncRequest) )
    Alternatively, you can use the getSysNum(syscall_name: str) function. 6.
6. run File → script file → systracer file (it should take bp)
7. go to Debugger → Debugger Windows → Thread list
8. run the game until you find the desired thread
9. click on the desired thread and run it with F9
10. when SIGTRAP pops up, click edit exception and uncheck suspend program
11. if you want to see the captured call stack, replace gdb with python in the Output view and type tracer.listAllCallStack()
12. if you want to see only the call stack of a specific syscall, enter tracer.viewCallStack(33) with only the desired syscall number like this

![Syscall Tracer Result](./img/Syscall_Tracer_Result.png)

1. result에 call stack이 저정되어 있는데, 다시 초기화하고 싶으면 `tracer.result = {}`
2. 모든 작업이 다 끝났으면 `tracer.afterTrace()`

1. result has the call stack saved, if you want to reinitialize it, use `tracer.result = {}`
2. when all is said and done, call `tracer.afterTrace()`

## How to find syscall wrapper
You can start at the beginning of the code region and look around the SVCs where you find a lot of SVCs, or where you get stuck when you stop the game.

## Trouble shooting

There is an issue with tracing not working under certain conditions.

You can check two things

1. make sure your rebase is working properly.
2. check if the syscall wrapper area is properly undefined. If not, you can select it with the mouse and undefine it with u.