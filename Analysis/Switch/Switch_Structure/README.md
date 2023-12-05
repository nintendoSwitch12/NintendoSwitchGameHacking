# Nintendo Switch 구조 분석

# Overview


Looking at Splatoon 2 and Super Mario Maker 2, I found that they had common characteristics.

I write it down because it seems like it can be used as a reference for the analysis.

# Structure Map


## Outline


![asdfasfasdfasd.asdf.png](./img/1.png)

## Process Memory Structure



![asdfsadff.png](./img/2.png)

# Setting

When Nintendo Switch games are loaded, 3 basic memory modules are loaded on to memory : 

1. Rtld - First module to be executed for initializing values.
2. Main - Contains the game logic.
3. SDK - Works as shared library. 

(The overall structure is similar to Linux)

# Memory modules


## Rtld


A function similar to dlsym exists.

The most common pattern is when the function operates in Sdk and calls the function of sdk after symbol resolve using a function such as dlsym in the Rtld area.

When Sdk calls a function, it corrects the get by restoring the symbol to rtld

The function pointer has the address of the symbol resolve function of rtld when it is actually called at the beginning.

The symbol-resolved address is written to the table and works.

```python
__int64 sub_83386660()
{
  return qword_83A787A0();
}
```

```python
.text_rtld0:0000000080001624 LDR             X17, [SP,#arg_0]
.text_rtld0:0000000080001628 STR             X29, [SP,#arg_0]
.text_rtld0:000000008000162C STP             X8, X19, [SP,#var_10]!
.text_rtld0:0000000080001630 STP             X6, X7, [SP,#0x10+var_20]!
.text_rtld0:0000000080001634 STP             X4, X5, [SP,#0x20+var_30]!
.text_rtld0:0000000080001638 STP             X2, X3, [SP,#0x30+var_40]!
.text_rtld0:000000008000163C STP             X0, X1, [SP,#0x40+var_50]!
.text_rtld0:0000000080001640 STP             Q6, Q7, [SP,#0x50+var_70]!
.text_rtld0:0000000080001644 STP             Q4, Q5, [SP,#0x70+var_90]!
.text_rtld0:0000000080001648 STP             Q2, Q3, [SP,#0x90+var_B0]!
.text_rtld0:000000008000164C STP             Q0, Q1, [SP,#0xB0+var_D0]!
.text_rtld0:0000000080001650 MOV             X29, SP
.text_rtld0:0000000080001654 MOV             X19, X17
.text_rtld0:0000000080001658 SUB             X1, X17, X16
.text_rtld0:000000008000165C SUB             X1, X1, #8
.text_rtld0:0000000080001660 LSR             X1, X1, #3
.text_rtld0:0000000080001664 LDUR            X0, [X16,#-8]
.text_rtld0:0000000080001668 BL              unk_80000800
.text_rtld0:000000008000166C STR             X0, [X19]
.text_rtld0:0000000080001670 MOV             X16, X0
.text_rtld0:0000000080001674 LDP             Q0, Q1, [SP+0xD0+var_D0],#0x20
.text_rtld0:0000000080001678 LDP             Q2, Q3, [SP+0xB0+var_B0],#0x20
.text_rtld0:000000008000167C LDP             Q4, Q5, [SP+0x90+var_90],#0x20
.text_rtld0:0000000080001680 LDP             Q6, Q7, [SP+0x70+var_70],#0x20
.text_rtld0:0000000080001684 LDP             X0, X1, [SP+0x50+var_50],#0x10
.text_rtld0:0000000080001688 LDP             X2, X3, [SP+0x40+var_40],#0x10
.text_rtld0:000000008000168C LDP             X4, X5, [SP+0x30+var_30],#0x10
.text_rtld0:0000000080001690 LDP             X6, X7, [SP+0x20+var_20],#0x10
.text_rtld0:0000000080001694 LDP             X8, X19, [SP+0x10+var_10],#0x10
.text_rtld0:0000000080001698 LDP             X29, X30, [SP+arg_0],#0x10
.text_rtld0:000000008000169C BR              X16
.text_rtld0:000000008000169C ; End of function sub_80001624
.text_rtld0:000000008000169C
.text_rtld0:000000008000169C ; -
```

**If you recognize it as a code and make it a function, you can check the following results.**

```python
// positive sp value has been detected, the output may be wrong!
__int64 __fastcall sub_80001624(
        __int64 a1,
        __int64 a2,
        __int64 a3,
        __int64 a4,
        __int64 a5,
        __int64 a6,
        __int64 a7,
        __int64 a8)
{
  __int64 v8; // x16
  __int64 (__fastcall *v9)(__int64, __int64, __int64, __int64, __int64, __int64, __int64, __int64); // x0
  __int64 (__fastcall **v19)(__int64, __int64, __int64, __int64, __int64, __int64, __int64, __int64); // [xsp+C0h] [xbp-10h]

  v9 = (__int64 (__fastcall *)(__int64, __int64, __int64, __int64, __int64, __int64, __int64, __int64))((__int64 (__fastcall *)(_QWORD, unsigned __int64))unk_80000800)(*(_QWORD *)(v8 - 8), ((unsigned __int64)v19 - v8 - 8) >> 3);
  *v19 = v9;
  return v9(a1, a2, a3, a4, a5, a6, a7, a8);
}
```

**In fact, as the above pattern is used, a specific function is found through symbol.**

```python
.text_rtld0:0000000080000C80 SUB             SP, SP, #0x80
.text_rtld0:0000000080000C84 STP             X22, X21, [SP,#0x70+var_20]
.text_rtld0:0000000080000C88 STP             X20, X19, [SP,#0x70+var_10]
.text_rtld0:0000000080000C8C STP             X29, X30, [SP,#0x70+var_s0]
.text_rtld0:0000000080000C90 ADD             X29, SP, #0x70
.text_rtld0:0000000080000C94 LDRB            W8, [X0,#0x30]
.text_rtld0:0000000080000C98 MOV             W20, W1
.text_rtld0:0000000080000C9C MOV             X19, X0
.text_rtld0:0000000080000CA0 CBZ             W8, loc_80000D28
.text_rtld0:0000000080000CA4 LDR             X21, [X19,#0x10]
.text_rtld0:0000000080000CA8 MOV             W9, #0x18
.text_rtld0:0000000080000CAC UMADDL          X10, W20, W9, X21
.text_rtld0:0000000080000CB0 LDR             X8, [X19,#0x68]
.text_rtld0:0000000080000CB4 SUB             X1, X29, #-var_28
.text_rtld0:0000000080000CB8 ADD             X2, SP, #0x70+var_58
.text_rtld0:0000000080000CBC LDR             W10, [X10,#0xC]
.text_rtld0:0000000080000CC0 MADD            X8, X10, X9, X8
.text_rtld0:0000000080000CC4 LDUR            X9, [X8,#4]
.text_rtld0:0000000080000CC8 LDUR            X10, [X8,#0xC]
.text_rtld0:0000000080000CCC LDR             W11, [X8,#0x14]
.text_rtld0:0000000080000CD0 LDR             W22, [X8]
.text_rtld0:0000000080000CD4 STP             X9, X10, [SP,#0x70+var_40]
.text_rtld0:0000000080000CD8 MOV             W8, W11
.text_rtld0:0000000080000CDC STR             W8, [SP,#0x70+var_44]
.text_rtld0:0000000080000CE0 LDR             X8, [SP,#0x70+var_40]
.text_rtld0:0000000080000CE4 MOV             X9, X10
.text_rtld0:0000000080000CE8 MOV             X0, X19
.text_rtld0:0000000080000CEC STR             W11, [SP,#0x70+var_30]
.text_rtld0:0000000080000CF0 STR             W22, [SP,#0x70+var_58]
.text_rtld0:0000000080000CF4 STUR            X9, [SP,#0x70+var_4C]
.text_rtld0:0000000080000CF8 STUR            X8, [SP,#0x70+var_54]
.text_rtld0:0000000080000CFC BL              sub_80000E30
.text_rtld0:0000000080000D00 TBZ             W0, #0, loc_80000D90
.text_rtld0:0000000080000D04 LDUR            X8, [X29,#var_28]
.text_rtld0:0000000080000D08 CBZ             X8, loc_80000E18
.text_rtld0:0000000080000D0C MOV             W9, W20
.text_rtld0:0000000080000D10 MOV             W10, #0x18
.text_rtld0:0000000080000D14 MADD            X9, X9, X10, X21
.text_rtld0:0000000080000D18 LDR             X9, [X9,#0x10]
.text_rtld0:0000000080000D1C ADD             X0, X9, X8
.text_rtld0:0000000080000D20 STUR            X0, [X29,#var_28]
.text_rtld0:0000000080000D24 B               loc_80000E1C
.text_rtld0:0000000080000D28 ; --
```

```python
_int64 v20; // [xsp+4h] [xbp-6Ch]
  __int64 v21; // [xsp+Ch] [xbp-64h]
  int v22; // [xsp+14h] [xbp-5Ch]
  int v23; // [xsp+18h] [xbp-58h] BYREF
  __int64 v24; // [xsp+1Ch] [xbp-54h]
  __int64 v25; // [xsp+24h] [xbp-4Ch]
  int v26; // [xsp+2Ch] [xbp-44h]
  __int64 v27; // [xsp+30h] [xbp-40h]
  __int64 v28; // [xsp+38h] [xbp-38h]
  int v29; // [xsp+40h] [xbp-30h]
  __int64 v30; // [xsp+48h] [xbp-28h] BYREF

  if ( !*(_BYTE *)(a1 + 48) )
  {
    v10 = (unsigned int *)(*(_QWORD *)(a1 + 104) + 24LL * *(unsigned int *)(*(_QWORD *)(a1 + 16) + 16LL * a2 + 12));
    v11 = *(_QWORD *)(v10 + 3);
    v12 = v10[5];
    v13 = *v10;
    v27 = *(_QWORD *)(v10 + 1);
    v28 = v11;
    v22 = v12;
    v29 = v12;
    v19 = v13;
    v21 = v11;
    v20 = v27;
    if ( (sub_80000E30(a1, &v30, &v19) & 1) != 0 )
      return v30;
    v16 = ((__int64 (__fastcall *)(const char *))unk_80001948)("[rtld] Unresolved symbol: '");
    ((void (__fastcall *)(const char *, __int64))unk_80001704)("[rtld] Unresolved symbol: '", v16);
    v15 = *(_QWORD *)(a1 + 96) + v13;
    goto LABEL_9;
  }
  v4 = *(_QWORD *)(a1 + 16);
  v5 = (unsigned int *)(*(_QWORD *)(a1 + 104) + 24LL * *(unsigned int *)(v4 + 24LL * a2 + 12));
  v6 = *(_QWORD *)(v5 + 3);
  v7 = v5[5];
  v8 = *v5;
  v27 = *(_QWORD *)(v5 + 1);
  v28 = v6;
  v26 = v7;
  v29 = v7;
  v23 = v8;
  v25 = v6;
  v24 = v27;
  if ( (sub_80000E30(a1, &v30, &v23) & 1) == 0 )
  {
    v14 = ((__int64 (__fastcall *)(const char *))unk_80001948)("[rtld] Unresolved symbol: '");
    ((void (__fastcall *)(const char *, __int64))unk_80001704)("[rtld] Unresolved symbol: '", v14);
    v15 = *(_QWORD *)(a1 + 96) + v8;
LABEL_9:
    v17 = ((__int64 (__fastcall *)(__int64))unk_80001948)(v15);
    ((void (__fastcall *)(__int64, __int64))unk_80001704)(v15, v17);
    v18 = ((__int64 (__fastcall *)(void *))unk_80001948)(&unk_800027FB);
    ((void (__fastcall *)(void *, __int64))unk_80001704)(&unk_800027FB, v18);
    result = 0LL;
    v30 = 0LL;
    return result;
  }
  if ( !v30 )
    return 0LL;
  result = *(_QWORD *)(v4 + 24LL * a2 + 16) + v30;
  v30 = result;
  return result;
}
```

**Perform symbol resolution.**

![Untitled](./img/3.png)

**You can easily distinguish it by using switch mapper.py .**

```python
.text_rtld0:0000000080001690 LDP             X6, X7, [SP+0x20+var_20],#0x10
.text_rtld0:0000000080001694 LDP             X8, X19, [SP+0x10+var_10],#0x10
.text_rtld0:0000000080001698 LDP             X29, X30, [SP+arg_0],#0x10
.text_rtld0:000000008000169C BR              X16
.text_rtld0:000000008000169C ; End of function sub_80001624
```

```python

.data:0000000083A80208 off_83A80208 DCQ sub_83386660           ; DATA XREF: nn::image::detail::ReadJpegHeader(nn::image::detail::JpegInfo *,uchar const*,ulong,uint)↑o
.data:0000000083A80208                                         ; nn::image::detail::ReadJpegHeader(nn::image::detail::JpegInfo *,uchar const*,ulong,uint)+4↑r ...
.data:0000000083A80210 off_83A80210 DCQ sub_83386660           ; DATA XREF: nn::image::detail::SetJpegHeader(nn::image::detail::JpegInfo *,ushort,ushort,nn::image::JpegSamplingRatio)↑o
.data:0000000083A80210                                         ; nn::image::detail::SetJpegHeader(nn::image::detail::JpegInfo *,ushort,ushort,nn::image::JpegSamplingRatio)+4↑r ...
.data:0000000083A80218 ; __int64 (*off_83A80218)(void)
.data:0000000083A80218 off_83A80218 DCQ _ZN2nn5image6detail17ExtractJpegHeaderEPNS1_10JpegHeaderEPKvmi
.data:0000000083A80218                                         ; DATA XREF: nn::image::detail::ExtractJpegHeader(nn::image::detail::JpegHeader *,void const*,ulong,int)↑o
.data:0000000083A80218                                         ; nn::image::detail::ExtractJpegHeader(nn::image::detail::JpegHeader *,void const*,ulong,int)+4↑r ...
.data:0000000083A80220 off_83A80220 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_std_error(nn::image::detail::jpeg::jpeg_error_mgr *)↑o
.data:0000000083A80220                                         ; nn::image::detail::jpeg::jpeg_std_error(nn::image::detail::jpeg::jpeg_error_mgr *)+4↑r ...
.data:0000000083A80228 ; __int64 (*off_83A80228)(void)
.data:0000000083A80228 off_83A80228 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_CreateDecompress(nn::image::detail::jpeg::jpeg_decompress_struct *,int,ulong)↑o
.data:0000000083A80228                                         ; nn::image::detail::jpeg::jpeg_CreateDecompress(nn::image::detail::jpeg::jpeg_decompress_struct *,int,ulong)+4↑r ...
.data:0000000083A80230 off_83A80230 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_mem_src(nn::image::detail::jpeg::jpeg_decompress_struct *,uchar *,ulong)↑o
.data:0000000083A80230                                         ; nn::image::detail::jpeg::jpeg_mem_src(nn::image::detail::jpeg::jpeg_decompress_struct *,uchar *,ulong)+4↑r ...
.data:0000000083A80238 ; __int64 (*off_83A80238)(void)
.data:0000000083A80238 off_83A80238 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_read_header(nn::image::detail::jpeg::jpeg_decompress_struct *,bool)↑o
.data:0000000083A80238                                         ; nn::image::detail::jpeg::jpeg_read_header(nn::image::detail::jpeg::jpeg_decompress_struct *,bool)+4↑r ...
.data:0000000083A80240 ; __int64 (*off_83A80240)(void)
.data:0000000083A80240 off_83A80240 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_start_decompress(nn::image::detail::jpeg::jpeg_decompress_struct *)↑o
.data:0000000083A80240                                         ; nn::image::detail::jpeg::jpeg_start_decompress(nn::image::detail::jpeg::jpeg_decompress_struct *)+4↑r ...
.data:0000000083A80248 off_83A80248 DCQ sub_83386660           ; DATA XREF: nn::image::detail::jpeg::jpeg_read_scanlines(nn::image::detail::jpeg::jpeg_decompress_struct *,uchar **,int)↑o
.data:0000000083A80248                                         ; nn::image::detail::jpeg::jpeg_read_scanlines(nn::image::detail::jpeg::jpeg_decompress_struct *,uchar **,int)+4↑r ...
.data:0000000083A80250 off_83A80250 DCQ sub_83386660
```

**After the resolve, the got-like part of the sdk is modified in this way.**

## SDK



Use Lazy binding.

The functions that generate threads are also here.

The peculiar thing is that some implementations of linux libc also exist.

There are memcpy, strcmp, putts, strlen, etc.

## Main



# Services



## System services



Several services exist.

It is not a method in which a user process directly accesses resources through a direct svc call, but processes resources by proxy through sysmoudules.

API exists in SDK.

This API gets the handle of the system service through a specific ciscoll, and the memory of the service is mapped to shared. Since tasks are handled through IPC, it is impossible to directly access resources and connect sockets in the user-level process.

### Example

Send an IPC request to svc 0x21 SendSyncRequest.

The part that sends the actual packet also sends the request to IPC at 0x21.

r—Shared memory is mapped with permissions.

The result comes into the readonly shared memory.

### syscalls

System service uses system calls with regular users.

It uses multiple system calls such as IPC mapping or accept session.

In general, such system calls are not allowed in the game, and those system call calls are blocked in the game.

# Sandbox



The npdm files contains a catalog of permissions

![Untitled](./img/4.jpeg)

Therefore, if one tries to run a syscall that is blocked by Sandbox, kernel panic would occur. 

There are many syscall defined in SDK wrapper. However, some of them are blocked by the OS and not used.