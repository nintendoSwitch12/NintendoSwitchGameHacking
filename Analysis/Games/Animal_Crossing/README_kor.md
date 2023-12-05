# 마이 디자인이란

마이 디자인이란 “모여봐요 동물의 숲”과 “튀어나와요 동물의 숲”에서 사용할 수 있는 옷, 벽지, 바닥지 등의 디자인 패턴이다. 사용자는 자신이 원하는 디자인을 제작하여 도안 ID, QR 코드 등을 통해 공유할 수 있으며 또한 다른 사용자가 만든 디자인을 불러와 사용할 수 있다.

마이 디자인에는 일반 버전과 프로 버전 두가지가 존재한다. 일반 마이 디자인은 벽지나 바닥지 등에 사용할 수 있으며 프로 마이디자인은 옷, 우산 등에 사용할 수 있다.

![](./img/1.png)

마이 디자인 PRO의 한 예

# 마이 디자인 구조체

마이 디자인은 “모여봐요 동물의 숲”과 “튀어나와요 동물의 숲”에서 모두 사용할 수 있으므로, 두 게임에서의 구조체는 동일하다. NHSE[https://github.com/kwsch/NHSE]에 의하면 마이 디자인의 구조체는 다음과 같이 생겼다.

```cpp
struct ColorPalette
{
    u8 B;
    u8 G;
    u8 R;
};

bitfield Color
{
    first_color_choice : 4;
    second_color_choice : 4;
};

struct DesignPattern
{
    u32 hash;
    u32 version;
    padding[8];
    char design_name[40];
    u32 town_id;
    char town_name[20];
    padding[4];
    u32 player_id;
    char player_name[20];
    padding[12];
    ColorPalette color_palette[15];
    Color color_choices[512];
    padding[3];
};

struct DesignPatternPROSheet
{
    Color color_choices[0x200];
};

struct DesignPatternPRO
{
    u32 hash;
    u32 version;
    padding[8];
    char design_name[40];
    u32 town_id;
    char town_name[20];
    padding[4];
    u32 player_id;
    char player_name[20];
    padding[12];
    ColorPalette color_palette[15];
    DesignPatternPROSheet sheets[4];
    padding[3];
};
```

# 마이 디자인 공유 방법

마이 디자인과 마이 디자인 PRO를 공유하는 방법은 디자인 ID를 이용하는 방법과 QR 코드를 이용하는 방법이 있다. 두 방법 모두 Nintendo Switch Online에 가입되어 있어야 한다, QR 코드 방식을 사용하기 위해선 “Nintendo Switch Online” 모바일 어플이 설치되어 있어야 한다.

## 디자인 ID

디자인 ID 등의 ID를 이용하는 방법에는 고순이의 옷 가게인 “에이블 시스터즈”가 열려야 한다. 에이블 시스터즈가 열린 생태에서 가게에 존재하는 “마이 디자인 단말기”를 사용하여 디자인을 업로드할 수 있다.

## QR 코드

스마트폰에 설치한 “Nintendo Switch Online” 어플과 “모여봐요 동물의 숲”을 연동한 뒤에, 스마트폰에서 QR 코드를 인식하면 해당 디자인을 다운로드 받을 수 있다.

QR 코드로 된 다자인은 ACPatterns[https://acpatterns.com/]와 같은 사이트를 이용하거나 3DS에 존재하는 “튀어나와요 동물의 숲”에서 만들 수 있다.

![](./img/2.png)

마이 디자인 QR 코드 예시

# QR 코드 분석

DamSenViet의 acpatterns-js[https://github.com/DamSenViet/acpatterns-js]를 참고하여 분석할 수 있다.

# 참고

http://dot-e-nanika.com/
