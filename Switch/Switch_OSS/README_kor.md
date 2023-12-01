# 개요

Nintendo Open Source Software에 대해 찾아보았다.

# Nintendo Switch OSS

일부 닌텐도 제품군은 오픈소스 라이선스를 가진 OSS(오픈 소스 소프트웨어)를 사용하는데, 이는 인터넷에 공개되어 있다. 취약점 분석 중 공개된 소스와 관련된 코드가 있다면 참고할 수 있다.
OSS는 제품군 기준으로 하여, Nintendo Switch. Wii U, Nintendo 3DS, Other 등으로 나눠져 있다. 이 중 Nintendo Switch와 관련된 소스는 아래와 같다. 
Some Nintendo family uses open-source software (OSS) with an open-source license, which is published on the Internet. If there is a code related to the disclosed source during the vulnerability analysis, you can refer to it.
OSS is based on the product line, Nintendo Switch. It is divided into Wii U, Nintendo 3DS, and Other. Among them, sources related to Nintendo Switch are as follows.

- Nintendo Switch OSS
- Nintendo SDK OSS
- Nintendo ParentalContorls OSS

## **Nintendo Switch OSS**

- NSPR(Netscape Portable Runtime)
- NSS(Network Security Services)
- NX-NXFP2
- WebKit & WebKit Cario
- WKC(WebKit Core)

## Nindendo SDK OSS

닌텐도 SDK에서는 Movie(AAC) Library와 Codec Library를 위해 OSS를 사용한다.
Movie & Codec Library에서 사용하는 OSS는 다음과 같다.

- libAACdec & libAACdecenc
- libFDK
- libMpegTPDec & libMpegTPEnc
- libPCMutils
- libSBRdec & libSBRenc
- libSYS

# Ref

---

[1] https://www.nintendo.co.jp/support/oss/

[2] https://www.nintendo.co.jp/support/oss/data/NintendoSwitch_OpenSources16.1.0.zip

[3] https://www.nintendo.co.jp/support/oss/data/NintendoSDKLibrary_OpenSources.zip

[4] https://bbs.ruliweb.com/nin/board/300004/read/2118096

[5] https://www.nintendo.co.jp/support/oss/data/NintendoSwitchParentalControls_1.0.0_OSS.zip

[6] https://firefox-source-docs.mozilla.org/nspr/index.html

[7] https://firefox-source-docs.mozilla.org/security/nss/index.html#network-security-services-nss

[8] https://webkit.org/ 

[9] https://en.wikipedia.org/wiki/Advanced_Audio_Coding
