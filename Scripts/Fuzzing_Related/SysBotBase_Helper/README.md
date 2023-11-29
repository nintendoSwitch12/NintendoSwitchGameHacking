# Sys-BotBase

Sys-BotBase은 olliz0r가 만든 모듈로, CFW 환경에서 동작한다.[^1]. socket 통신 방식으로 기기로 명령을 내릴 수 있도록 지원한다. 이를 통해 스위치 게임 봇 등을 구현할 수 있다. 

해당 모듈이 지원하는 명령은 다음 주소에서 찾을 수 있다.

[sys-botbase commands](https://github.com/olliz0r/sys-botbase/blob/master/commands.md)

# Sys-BotBase Helper

sys-botbase의 기능을 보다 편리하게 활용할 수 있도록 파이썬 클래스를 제작하였다. 이 클래스는 PC에서 sys-botbase로 명령을 내릴 때 자주 구현하는 몇 가지 기능을 미리 구현해둔 것으로, sys-botbase와 연동시킬 PC에서 사용할 수 있다. 

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