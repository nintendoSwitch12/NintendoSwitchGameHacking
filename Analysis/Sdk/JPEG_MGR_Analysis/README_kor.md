# JPEG 분석
## JPEGMGR thread 분석
### 개요 
JPEG 파싱을 담당하는 쓰레드가 존재한다.
그 쓰레드에 대한 분석을 진행하였다.
### 구조

![JPEG_structure](./img/1.png)

### 분석
직접 JPEGMGR Thread를 분석해보면서 스위치 게임을 쓰레드들의 구조에 대해서 알 수 있다.
`nn::os::detail::ThreadManagerImplByHorizon::CreateThread(nn::os::ThreadType *,void (*)(nn::os::ThreadType *),int)` 을 통해 초반에 Jpeg Mgr 쓰레드가 생성된다.
초반에 tls같은 쓰레드의 로컬 스토리지를 초기화해준다.
분석을 좀 더 수월하게 진행하기 위해서 Bindiff를 이용해서 디핑을 먼저 진행한다.

![Diffing](./img/2.png)

File→BinDiff를 클릭해서 IDB를 이용해 디핑을 할 수 있다.
그리고 원하는 부분을 클릭해서 symbol을 적용시킬 수 있다.

#### ThreadMain
```
__int64 __fastcall dispatch(__int64 a1)
{
  __int64 v1; // x20
  __int64 result; // x0
  __int64 v4; // x1

  v1 = a1 + 48;
  result = get_msg(a1 + 48, *(_DWORD *)(a1 + 184));
  if ( result != *(_QWORD *)(a1 + 192) )
  {
    v4 = result;
    do
    {
      (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)a1 + 136LL))(a1, v4);
      result = get_msg(v1, *(_DWORD *)(a1 + 184));
      v4 = result;
    }
    while ( result != *(_QWORD *)(a1 + 0xC0) );
  }
  return result;
}
```
이런식의 구조를 취하고 있다.
get_msg는 nn::os::TryReceiveMessageQueue를 이용해 메세지 큐에서 메세지를 pop한다.
이후 여기서 세팅된 구조체에 따라 동작이 달라진다.
#### TryReceiveMessageQueue
```
__int64 __fastcall nn::os::TryReceiveMessageQueue(_QWORD *ptr, __int64 object_0x30)
{
  nn::os::detail::InternalCriticalSectionImplByHorizon *v2; // x19
  int v5; // w8
  __int64 v6; // x10
  int v7; // w8
  int v8; // w9
  __int64 msgs; // x11
  int v10; // w10
  __int64 i; // x21

  v2 = (nn::os::detail::InternalCriticalSectionImplByHorizon *)(object_0x30 + 0x38);
  nn::os::detail::InternalCriticalSectionImplByHorizon::Enter((nn::os::detail::InternalCriticalSectionImplByHorizon *)(object_0x30 + 0x38));
  v5 = *(_DWORD *)(object_0x30 + 0x2C);
  if ( v5 )
  {
    v6 = *(int *)(object_0x30 + 0x30);
    v7 = v5 - 1;
    v8 = *(_DWORD *)(object_0x30 + 0x28);
    msgs = *(_QWORD *)(*(_QWORD *)(object_0x30 + 0x20) + 8 * v6);// *(object + 0x40) <= msg array?
    v10 = v6 + 1;                               // initializing?
    if ( v10 < v8 )
      v8 = 0;
    *(_DWORD *)(object_0x30 + 0x2C) = v7;
    *(_DWORD *)(object_0x30 + 0x30) = v10 - v8;
    *ptr = msgs;                                // SignalProcessWideKey 0x1d syscall
    nn::os::detail::InternalConditionVariableImplByHorizon::Broadcast((nn::os::detail::InternalConditionVariableImplByHorizon *)(object_0x30 + 0x3C));
    for ( i = *(_QWORD *)(object_0x30 + 8); i != object_0x30; i = *(_QWORD *)(i + 8) )
      nn::os::detail::MultiWaitImpl::NotifyAndWakeupThread(
        *(nn::os::detail::MultiWaitImpl **)(i + 16),
        (nn::os::detail::MultiWaitHolderBase *)(i - 24));
    nn::os::detail::InternalCriticalSectionImplByHorizon::Leave(v2);
    return 1LL;
  }
  else
  {
    nn::os::detail::InternalCriticalSectionImplByHorizon::Leave(v2);
    return 0LL;
  }
}
```
이런식으로 큐에서 메세지를 pop해서 처리하는 것은 스위치 게임에서 가장 흔한 형태이다.
대부분의 쓰레드가 이런식으로 구현이 되어있다.
#### JPEG Marker Enum 제작
```
enum markers
{
	M_SOF0  = 0xFFC0, // Start Of Frame N
	M_SOF1,// N indicates which compression process
	M_SOF2,// Only SOF0-SOF2 are now in common use
	M_SOF3,
	M_DHT, //Huffman Table
	M_SOF5,
	M_SOF6,
	M_SOF7,
	M_JPG,
	M_SOF9,
	M_SOF10,
	M_SOF11,
	M_DAC = 0xffcc,
	M_SOF13,
	M_SOF14,
	M_SOF15,
	M_RST0 = 0xffd0,
	M_RST1, M_RST2, M_RST3, M_RST4, M_RST5, M_RST6, M_RST7,
	M_SOI = 0xFFD8, // Start Of Image
	M_EOI,// End Of Image
	M_SOS,// Start Of Scan (begins compressed data)
	M_DQT,
	M_DNL,
	M_DRI,
	M_DHP,
	M_EXP,
	M_APP0 = 0xFFE0, // Jfif marker
	M_APP1,// Exif marker
	M_APP2,
	M_APP3,
	M_APP4,
	M_APP5,
	M_APP6,
	M_APP7,
	M_APP8,
	M_APP9,
	M_APP10,
	M_APP11,
	M_APP12,
	M_APP13,
	M_APP14,
	M_APP15,
	M_JPG0 = 0xfff0,
	M_JPG1, M_JPG2, M_JPG3, M_JPG4, M_JPG5, M_JPG6, M_JPGLS,
	M_JPG8, M_JPG9, M_JPG10, M_JPG11, M_JPG12, M_JPG13, M_JPG14, M_JPG15,
	M_COMM = 0xFFFE  // Comment
};
```
IDA 분석을 보다 더 쉽게 하기 위해서 enum을 미리 만들어준다.
IDA에서 이제 parse c headerfile해서 enumeration을 만들어주면 enum이 생긴다.
이제 상수에 마우스 오른쪽 클릭을 통해 symbolic constant 기능을 이용해 분석을 수월하게 할 수 있다.
#### nn::image::detail::ExtractJpegHeader
당연하지만 처음 호출땐 lazy binding으로 symbol resolve 작업을 거치고 got 수정이 일어난다.
JPEG 헤더를 실질적으로 파싱하고, 구조체 초기화를 진행해주는 함수이다.
```
...
if ( (unsigned __int64)size < 2 )
    return 0xFFFFFFE0LL;
  is_valid = nn::image::detail::CheckSoi(Content, size);// Check start of Image
  if ( !(_DWORD)is_valid )                      // must be 0xFFD8
  {
    v114 = 0LL;
    v115 = 0LL;
...
```
```
...
memset(v102, 0, sizeof(v102));
    v100 = 0LL;
    v101 = 0LL;
    is_valid = nn::image::detail::ReadJpegHeader(hdrStruct, (__int64)Content, (unsigned __int64)size, a4);// Contains Validation Logic
    if ( !(_DWORD)is_valid )
    {
      if ( WORD1(v106) || HIWORD(v106) )
        return 0xFFFFFFE0LL;
...
```
#### CheckSoi
```
__int64 __fastcall nn::image::detail::CheckSoi(nn::image::detail *this, const unsigned __int8 *a2)
{
  if ( bswap32(*(unsigned __int16 *)this) >> 16 == StartOfImage )
    return 0LL;
  else
    return 0xFFFFFFE0LL;
}
```
SOI 마커를 체크한다.
#### get_marker 
여기서부터는 직접 리버싱한 함수이다.
```
...
unsigned __int64 iterator; // x8
  char *base; // x24
  char *marker_start; // x0 End of Image 0xffd9, Start of Image 0xffd8
  signed __int64 offset_from_start; // x9
  unsigned int marker; // w10
  unsigned __int64 hdr_pay_2; // x11
  int flag; // w8
  unsigned int v17; // w8
  __int64 v18; // x8

  *ret = 0LL;                                   // start is always 0
  if ( size )
  {
    iterator = 0LL;
    base = &data[baseOff];
    while ( 1 )                                 // if 0xff00 continue
    {
      while ( 1 )
      {
        if ( iterator + baseOff >= size )       // loop until end
          return 0;
        marker_start = (char *)j_memchr(&base[iterator], 0xFFLL, size - (iterator + baseOff));// search 0xff
        if ( !marker_start )                    // SOI must be 0xffd8 so it always return start of image
          return 0;
        offset_from_start = marker_start - data;
        if ( marker_start - data + 2 > size )
          return 0;
        marker = bswap32(*(unsigned __int16 *)marker_start) >> 16;
        if ( marker != 0xFF00 )
          break;
        iterator = marker_start + 2 - base;
        if ( iterator >= size )
          return 0;
      }
      if ( marker < ArithTemp )
        goto LABEL_14;
      if ( marker == ArithTemp )
        break;
      if ( marker < 0xFF4F )
      {
        flag = 3;
...
```
루프를 돌면서 여러 마커와 벼교한다.
```
...
else if ( marker >> 6 < 0x3FF )           // 0xffc0 HuffBaselineDCT
      {
        flag = 4;
      }
      else
      {
        if ( marker < ArthBaselineDCT )
          break;
        if ( marker == ArthBaselineDCT )
        {
          flag = 3;
        }
        else
        {
          if ( marker >> 4 < 0xFFD )            // 0xffd0 RestartIntervalStart
            break;
          if ( marker < StartOfScan )
          {
            flag = 0;
          }
          else
          {
            if ( marker >> 4 < 0xFFF )          // 0xfff0 JpegExt0
              break;
            if ( marker < Comment )
            {
              flag = 3;
            }
            else
            {
              v18 = 9LL;
              if ( marker == 0xFFFF )           // ? not jpeg marker
                v18 = 10LL;
              flag = *((_DWORD *)&unk_838AD570 + 2 * v18 + 1);
              if ( flag == 1 )
                break;
            }
          }
        }
      }
      if ( flag != 2 )
      {
        if ( flag == 4 )
          return 0;
LABEL_14:
        hdr_pay_2 = 2LL;
        if ( marker == target_marker )          // if 0xffd9? return
          goto LABEL_47;
        goto LABEL_15;
...
 ```
```
...
}
      hdr_pay_2 = 1LL;
      if ( marker == target_marker )
        goto LABEL_47;
LABEL_15:
      if ( marker != EndOfImage )
      {
        iterator = &marker_start[hdr_pay_2] - base;// to get next one
        if ( iterator < size )
          continue;
      }
      return 0;
    }
    if ( offset_from_start + 4 > size )
      return 0;
    hdr_pay_2 = ((unsigned __int8)marker_start[3] | ((unsigned __int64)(unsigned __int8)marker_start[2] << 8)) + 2;// get payload bytes becuz entry's(0xff..) sz is 0x2. just right after hdrs
    v17 = 0;
    if ( hdr_pay_2 < 4 || hdr_pay_2 + offset_from_start > size )
      return v17;
    if ( marker == target_marker )
    {
LABEL_47:
      *ret = offset_from_start;
      return 1;
    }
    goto LABEL_15;
  }
...
```
JPEG Data 시작에서부터의 마커 오프셋을 리턴한다.
#### ReadJpegHeader
```
...
++v8;
      v10 += v18;
      v13 -= v18;
    }
    while ( v13 );
    tmp = v10;
  }
  while ( (get_marker(&tmp, data, size, v10, 0xFFDBu) & 1) != 0 );
  if ( !v8 )
    return 0xFFFFFFE0LL;
  *((_DWORD *)hdr_struct + 28) = v8;
  *((_DWORD *)hdr_struct + 29) = v7;
  tmp = 0LL;
  v19 = -64;
  if ( (get_marker(&tmp, data, size, 0LL, M_SOF0) & 1) == 0 )
  {
    v19 = M_SOF2;
    if ( (get_marker(&tmp, data, size, 0LL, M_SOF2) & 1) == 0 )
    {
      v19 = M_SOF6;
      if ( (get_marker(&tmp, data, size, 0LL, M_SOF6) & 1) == 0 )
      {
        v19 = M_SOF10;
        if ( (get_marker(&tmp, data, size, 0LL, M_SOF10) & 1) == 0 )
        {
          v19 = M_SOF14;
          if ( (get_marker(&tmp, data, size, 0LL, M_SOF14) & 1) == 0 )
            return 0xFFFFFFDFLL;
        }
      }
    }
  }
  tmp = 0LL;
  if ( (get_marker(&tmp, data, size, 0LL, v19) & 1) == 0 )// most of times 0xffc0
    return 0xFFFFFFE0LL;
  v20 = tmp;
  v21 = tmp + 4;
  if ( tmp + 4 > size )
...
```
marker를 검증, 체크한다.
기본적으로 생성되는 썸네일 이미지의 방식으로 보인다.
SOF 마커를 체크하는 것을 볼 수 있다.
```
}
  tmp = 0LL;
  if ( (get_marker(&tmp, data, size, 0LL, v19) & 1) == 0 )
    return 0xFFFFFFE0LL;
  v20 = tmp;
  v21 = tmp + 4;
  if ( tmp + 4 > size )
    return 0xFFFFFFE0LL;
  *a1 = bswap32(*(unsigned __int16 *)&data[tmp]) >> 16;
  tmp = v20 + 2;
  v22 = (unsigned __int8)data[v20 + 3] | ((unsigned __int64)(unsigned __int8)data[v20 + 2] << 8);
  a1[1] = _byteswap_ushort(*(_WORD *)&data[v20 + 2]);
  if ( (unsigned int)v22 < 8 )
    return 0xFFFFFFE0LL;
  if ( v20 + 2 + v22 > size )
    return 0xFFFFFFE0LL;
  *((_BYTE *)a1 + 4) = data[v21];
  a1[3] = bswap32(*(unsigned __int16 *)&data[v20 + 5]) >> 16;
  a1[4] = bswap32(*(unsigned __int16 *)&data[v20 + 7]) >> 16;
  v23 = (unsigned __int8)data[v20 + 9];
  *((_BYTE *)a1 + 10) = v23;
  tmp = v20 + 10;
  if ( (unsigned int)(v23 - 1) > 3 || 3 * (_DWORD)v23 + 8 != (_DWORD)v22 )
    return 0xFFFFFFE0LL;
  v24 = 0;
  v25 = 0;
  v26 = 0LL;
  *((_DWORD *)a1 + 3) = 0;
```
그리고 header struct의 필드를 파싱해서 집어넣는다.
이때 저장되는 값들은 SOF 마커의 ImageX, ImageY 필드의 값들이 저장된다.
```
...
return 0xFFFFFFE0LL;
  tmp = 0LL;
  *((_BYTE *)a1 + 160) = 0;
  ret = get_marker(&tmp, data, size, 0LL, App0);// 0xffe0 -> signature Marker ? 
  sig_idx = tmp;
  if ( (ret & 1) == 0 )
    goto LABEL_50;
  tmp += 2LL;
  v35 = (unsigned __int8)data[sig_idx + 3] | ((unsigned __int64)(unsigned __int8)data[sig_idx + 2] << 8);
  if ( (unsigned int)v35 < 2 || sig_idx + 2 + v35 > size )
    return 0xFFFFFFE0LL;
  signature_idx = sig_idx + 4;
  v37 = (unsigned __int16)(_byteswap_ushort(*(_WORD *)&data[sig_idx + 2]) - 2);
  tmp = sig_idx + 4;
  if ( (unsigned int)(v35 - 2) >= 5 && !(unsigned int)NvOsMemcmp_0(&data[signature_idx], "JFIF", 5LL) )// Check Signature
    *((_BYTE *)a1 + 0xA0) = 1;
  sig_idx = signature_idx + v37;
  tmp = signature_idx + v37;
LABEL_50:
  tmp = 0LL;
  *((_BYTE *)a1 + 161) = 0;
  marker = get_marker(&tmp, data, size, sig_idx, App14);
  v39 = *((unsigned __int8 *)a1 + 161);
  if ( (marker & 1) != 0 )
...
```
#### DecoderMain
앞서 설명한 함수들이 jpeg을 미리 분석하고 구조체를 초기화시키는 함수들이다.
```
...
ZN2nn5image11JpegDecoderC2Ev(&JPEG);
  ZN2nn5image11JpegDecoder12SetImageDataEPKvm(&JPEG, content, sz);
  if ( !(unsigned int)ZN2nn5image11JpegDecoder7AnalyzeEv(&JPEG, v12, v13, v14, v15) )// 0x08310B990
  {
    AnalyzedDimensionEv = ZNK2nn5image11JpegDecoder20GetAnalyzedDimensionEv((nn::image::JpegDecoder *)&JPEG);// High/Low -> X Y 
    AnalyzedWorkBufferSizeEv = ZNK2nn5image11JpegDecoder25GetAnalyzedWorkBufferSizeEv((nn::image::JpegDecoder *)&JPEG);// x0 + 0x30
    if ( AnalyzedWorkBufferSizeEv < (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)CurrentHeap + 0x78LL))(CurrentHeap) )
    {
...
```
실제로 JPEG 디코딩을 하는 함수에선 Jpeg::Decoder::Analyze 함수에서 앞서 설명한 함수들이 호출이 되면서 초기화를 진행하며 validation도 같이 진행하였다.
```
...
v18 = (unsigned int)calc(1, a6, AnalyzedDimensionEv, SHIDWORD(AnalyzedDimensionEv), 1, 1, 2);
      if ( (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v8 + 120LL))(v8) > v18 )
      {
        GPUMEM = (GPUMEM *)malloc_0(0x38LL, v8, 8u);
        agl::GPUMemBlockBase::GPUMemBlockBase(GPUMEM);
        GPUMEM->vtable = qword_82A074C0 + 16;
        agl::GPUMemBlockBase::allocBuffer_(GPUMEM, v18, (GPUMEM *)v8, 0x2000, 8);
        sub_802EB510((aassddff *)&main.field_18, GPUMEM, 0);
        v20 = (void *)alloc_mem(AnalyzedWorkBufferSizeEv, CurrentHeap, 0x10u);
        v21 = v28;
        if ( v28 )
          v21 = (*(__int64 (**)(void))off_82A05248[0])();
        v22 = ZN2nn5image11JpegDecoder6DecodeEPvmiS2_m(
                (nn::image::JpegDecoder *)&JPEG,
                (void *)(v21 + LODWORD(main.field_18)),
                v18,
                1LL,
                v20);
        if ( v20 )
          delete((__int64)v20);
        if ( !v22 )
        {
          obj = (asdf *)malloc_0(0xA0LL, v8, 8u);
          prepare_textureData(obj);
          *(_QWORD *)&main.offset = main.field_18;
          main.field_8 = v28;
          main.field_10 = v29;
          render_related(obj, &main, v18, a6, AnalyzedDimensionEv, SWORD2(AnalyzedDimensionEv), a5);
          v25 = (__int64 *)malloc_0(0x28LL, v8, 8u);
          sub_8027C480(v25);
          v26 = qword_82A166B0;
          v25[4] = (__int64)obj;
          *v25 = v26 + 16;
          goto LABEL_13;
...
```
이후 파싱한 데이터를 토대로 size를 계산하거나, 렌더링 관련 작업을 수행하는 등 여러가지 작업을 수행한다.
앞서 디핑을 통해 미리 심볼들이 복구했으니, 이를 이용해 GPU 관련 렌더링 함수들을 미리 식별할 수 있다.
