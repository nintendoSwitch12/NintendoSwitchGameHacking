# JPEGMGR Thread 분석



# Overview



Any JPEG image is parsed by a separate thread called JPEGMGR.

This article examines the working logic of this thread and how it validates JPEG format.

# Structure



![JPEG_Structure](./img/1.png)

# Analysis



Initially, JPEG MGR thread is created by calling `nn::os::detail::ThreadManagerImplByHorizon::CreateThread(nn::os::ThreadType *,void (*)(nn::os::ThreadType *),int)` 

After the thread is created, local storage for JPEG MGR thread is initialized like initializing TLS(Thread Local Storage) at the beginning of the game.

For easier analysis, we used dipping using Bindiff.

## Diffing



![Diffing](./img/2.png)

You can start dipping by clicking File→BinDiff and choose the IDB you wish to dip.(You have to install BinDiff if you haven’t).

After dipping, you apply symbol by clicking on the part you want.

## ThreadMain



```python
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

In the above code, get_msg calls nn::os::TryReceiveMessageQueue to pop a message from the message queue.

After that, the operation varies depending on the structure set here.

### TryReceiveMessageQueue

```python
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

Popping and processing messages from queues in this way is very common in switch games.

Most threads are implemented this way.

## JPEG Marker enum



```python
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

Enum is created to make analysis easier on IDA.

Now, if you create an enumeration by parsing the header file, enum will be created.

You can now use enum using the symbolic constant function by right clicking on the constant.

## nn::image::detail::ExtractJpegHeader



When a function is called for the first time, symbols are resolved through lazy binding and GOT table gets modified.

This is a function that parses the JPEG header and initializes its structure.

```python
if ( (unsigned __int64)size < 2 )
    return 0xFFFFFFE0LL;
  is_valid = nn::image::detail::CheckSoi(Content, size);// Check start of Image
  if ( !(_DWORD)is_valid )                      // must be 0xFFD8
  {
    v114 = 0LL;
    v115 = 0LL;
```

```python
memset(v102, 0, sizeof(v102));
    v100 = 0LL;
    v101 = 0LL;
    is_valid = nn::image::detail::ReadJpegHeader(hdrStruct, (__int64)Content, (unsigned __int64)size, a4);// Contains Validation Logic
    if ( !(_DWORD)is_valid )
    {
      if ( WORD1(v106) || HIWORD(v106) )
        return 0xFFFFFFE0LL;
```

### CheckSoi

```python
__int64 __fastcall nn::image::detail::CheckSoi(nn::image::detail *this, const unsigned __int8 *a2)
{
  if ( bswap32(*(unsigned __int16 *)this) >> 16 == StartOfImage )
    return 0LL;
  else
    return 0xFFFFFFE0LL;
```

Checks the SOI marker.


### get_marker

```python
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
```

Parses several markers while looping.

```python
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
```

undergo some comparisons.

```python
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
```

Returns the marker offset from the beginning of JPEG Data.

### ReadJpegHeader

```python
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
```

This verifies and checks the marker.

It appears to follow the typical approach of generating thumbnail images.

You can observe the checking of the SOF marker.

```python
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

It also parses the fields of header struct.

The values stored here becomes the values for the ImageX and ImageY fields of the SOF marker.

```python
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
```

Checks the JFIF and sets a value for the field of the header struct.

```python
v52 = *((unsigned __int8 *)a1 + 0x38);
  if ( v50 == 1 && v51 == 2 && v52 == 3 )
  {
LABEL_70:
    v48 = 3;
  }
  else
  {                                             // RGB color representation
    v53 = v52 == 0x42 && v51 == 0x47;           // 'B' 'G'
    v49 = v53 && v50 == 0x52;                   // 'R'
LABEL_86:
    v48 = v49 ? 2 : 3;
  }
LABEL_89:
  *((_DWORD *)a1 + 38) = 2;
  *((_DWORD *)a1 + 39) = v48;
  *((_DWORD *)a1 + 36) = 3;
  *((_BYTE *)a1 + 162) = 0;
  tmp = 0LL;
  if ( (get_marker(&tmp, data, size, 0LL, HuffmanTableDef) & 1) == 0 )
    return 0xFFFFFFE0LL;
  v54 = 0;
  v55 = 0;
  do
  {
```

RGB may also be used.

Below, it appears that there are functions performing scan-related tasks based on finding the scan marker.

```python
if ( (get_marker(&tmp, data, size, 0LL, v19) & 1) == 0 )// most of times 0xffc0
    return 0xFFFFFFE0LL;                        // HuffBaselineDCT
  v20 = tmp;
  v21 = tmp + 4;
  if ( tmp + 4 > size )
    return 0xFFFFFFE0LL;
  *(_WORD *)a1 = bswap32(*(unsigned __int16 *)&data[tmp]) >> 16;// most times ffc0
  tmp = v20 + 2;
  v22 = (unsigned __int8)data[v20 + 3] | ((unsigned __int64)(unsigned __int8)data[v20 + 2] << 8);// get word
  *((_WORD *)a1 + 1) = _byteswap_ushort(*(_WORD *)&data[v20 + 2]);// 0x11
  if ( (unsigned int)v22 < 8 )                  // 0x11 < 8
    return 0xFFFFFFE0LL;
  if ( v20 + 2 + v22 > size )                   // offset? OoB validation
    return 0xFFFFFFE0LL;
  a1[4] = data[v21];
  *((_WORD *)a1 + 3) = bswap32(*(unsigned __int16 *)&data[v20 + 5]) >> 16;
  *((_WORD *)a1 + 4) = bswap32(*(unsigned __int16 *)&data[v20 + 7]) >> 16;
  v23 = (unsigned __int8)data[v20 + 9];
  a1[10] = v23;
  tmp = v20 + 10;
```

compression 타입 관련해서 헤더의 멤버변수를 세팅해주는 것을 발견했다.

이때 특정 헤더의 오프셋? 같은 역할을 해주는 부분을 보면 offset 체크가 있다.

It was found that the member variable of the header was set in relation to the compression type.

Offset of a particular header at this time? If you look at the part that plays the same role, there is an offset check.

## DecoderMain



The previously described functions are responsible for pre-analyzing JPEG, and initializing data structures

```jsx
ZN2nn5image11JpegDecoderC2Ev(&JPEG);
  ZN2nn5image11JpegDecoder12SetImageDataEPKvm(&JPEG, content, sz);
  if ( !(unsigned int)ZN2nn5image11JpegDecoder7AnalyzeEv(&JPEG, v12, v13, v14, v15) )// 0x08310B990
  {
    AnalyzedDimensionEv = ZNK2nn5image11JpegDecoder20GetAnalyzedDimensionEv((nn::image::JpegDecoder *)&JPEG);// High/Low -> X Y 
    AnalyzedWorkBufferSizeEv = ZNK2nn5image11JpegDecoder25GetAnalyzedWorkBufferSizeEv((nn::image::JpegDecoder *)&JPEG);// x0 + 0x30
    if ( AnalyzedWorkBufferSizeEv < (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)CurrentHeap + 0x78LL))(CurrentHeap) )
    {                                           // 0xff~~
```

In the actual JPEG decoding function, the functions described earlier are invoked within the **`Jpeg::Decoder::Analyze`** function, initiating initialization and conducting validation.

```jsx
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
```

Subsequently, various operations are performed based on the parsed data, such as calculating size or executing rendering-related tasks.

Having previously recovered symbols through diffing, one can leverage this information to pre-identify GPU-related rendering functions.