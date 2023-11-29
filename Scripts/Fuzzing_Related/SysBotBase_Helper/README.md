# Table of contents
- [Table of contents](#table-of-contents)
- [Sys-BotBase](#sys-botbase)
- [Sys-BotBase Helper](#sys-botbase-helper)
  - [Example](#example)
    - [Click single button](#click-single-button)
    - [Press several buttons and release them](#press-several-buttons-and-release-them)
    - [Sequential click several buttons](#sequential-click-several-buttons)
    - [Get switch screen](#get-switch-screen)


# Sys-BotBase

Sys-BotBase은 olliz0r가 만든 모듈로, CFW 환경에서 동작한다.[^1]. socket 통신 방식으로 기기로 명령을 내릴 수 있도록 지원한다. 이를 통해 스위치 게임 봇 등을 구현할 수 있다. 

해당 모듈이 지원하는 명령은 다음 주소에서 찾을 수 있다.

Sys-BotBase is a module created by olliz0r that works in the CFW environment[^1]. It supports socket communication to send commands to devices. This allows you to implement Switch game bots, etc. 

The commands supported by this module can be found at the following address.

[sys-botbase commands](https://github.com/olliz0r/sys-botbase/blob/master/commands.md)

# Sys-BotBase Helper

sys-botbase의 기능을 보다 편리하게 활용할 수 있도록 파이썬 클래스를 제작하였다. 이 클래스는 PC에서 sys-botbase로 명령을 내릴 때 자주 구현하는 몇 가지 기능을 미리 구현해둔 것으로, sys-botbase와 연동시킬 PC에서 사용할 수 있다. 

To make it easier to utilize the features of sys-botbase, we have created a Python class. This class pre-implements some of the features that are often implemented when commanding sys-botbase from a PC, and can be used on a PC that will be interfacing with sys-botbase.

## Example

### Click single button

```python
sb = SysBot(ip=ip, click_global_delay=2.0)
seq = Sequence()

sb.Click(Button.A)
```

### Press several buttons and release them

```python
sb = SysBot(ip=ip, click_global_delay=2.0)
seq = Sequence()

seq.AddPressSequence([Button.L, Button.R]).AddWait(100).AddReleaseSequence([Button.L, Button.R]).AddWait(1000)
sb.ClickSequenceAndSync(seq)
```

### Sequential click several buttons

```python
sb = SysBot(ip=ip, click_global_delay=2.0)
seq = Sequence()

seq.AddClickSequenceWithWait([Button.Right, Button.A]).AddWait(900)
seq.AddClickSequenceWithWait([Button.Down, Button.Down, Button.A])
sb.ClickSequenceAndSync(seq)
```

### Get switch screen

```python
sb = SysBot(ip=ip, click_global_delay=2.0)
seq = Sequence()

screen_shot = sb.ScreenShot()
open('screen.jpg', 'wb+').write(screen_shot)
```

[^1]: https://github.com/olliz0r/sys-botbase