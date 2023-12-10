import socket
import time
from enum import Enum

Hex = lambda x : f'0x{x:X}'

# Enum for button
class Button(str, Enum):
    A = 'A'
    B = 'B'
    X = 'X'
    Y = 'Y'
    L = 'L'
    R = 'R'
    ZL = 'ZL'
    ZR = 'ZR'
    RightStick = 'RSTICK'
    LeftStick = 'LSTICK'
    Plus = 'PLUS'
    Minus = 'MINUS'
    Left = 'DLEFT'
    Right = 'DRIGHT'
    Up = 'DUP'
    Down = 'DDOWN'
    Home = 'HOME'
    Capture = 'CAPTURE'
    Palma = 'PALMA'

# Enum for key
class Key(str, Enum):
    A = 4
    B = 5
    C = 6
    D = 7
    E = 8
    F = 9
    G = 10
    H = 11
    I = 12
    J = 13
    K = 14
    L = 15
    M = 16
    N = 17
    O = 18
    P = 19
    Q = 20
    R = 21
    S = 22
    T = 23
    U = 24
    V = 25
    W = 26
    X = 27
    Y = 28
    Z = 29
    D1 = 30
    D2 = 31
    D3 = 32
    D4 = 33
    D5 = 34
    D6 = 35
    D7 = 36
    D8 = 37
    D9 = 38
    D0 = 39
    Return = 40
    Escape = 41
    Backspace = 42
    Tab = 43
    Space = 44
    Minus = 45
    Plus = 46
    OpenBracket = 47
    CloseBracket = 48
    Pipe = 49
    Tilde = 50
    Semicolon = 51
    Quote = 52
    Backquote = 53
    Comma = 54
    Period = 55
    Slash = 56
    CapsLock = 57
    F1 = 58
    F2 = 59
    F3 = 60
    F4 = 61
    F5 = 62
    F6 = 63
    F7 = 64
    F8 = 65
    F9 = 66
    F10 = 67
    F11 = 68
    F12 = 69
    PrintScreen = 70
    ScrollLock = 71
    Pause = 72
    Insert = 73
    Home = 74
    PageUp = 75
    Delete = 76
    End = 77
    PageDown = 78
    RightArrow = 79
    LeftArrow = 80
    DownArrow = 81
    UpArrow = 82
    NumLock = 83
    NumPadDivide = 84
    NumPadMultiply = 85
    NumPadSubtract = 86
    NumPadAdd = 87
    NumPadEnter = 88
    NumPad1 = 89
    NumPad2 = 90
    NumPad3 = 91
    NumPad4 = 92
    NumPad5 = 93
    NumPad6 = 94
    NumPad7 = 95
    NumPad8 = 96
    NumPad9 = 97
    NumPad0 = 98
    NumPadDot = 99
    Backslash = 100
    Application = 101
    Power = 102
    NumPadEquals = 103
    F13 = 104
    F14 = 105
    F15 = 106
    F16 = 107
    F17 = 108
    F18 = 109
    F19 = 110
    F20 = 111
    F21 = 112
    F22 = 113
    F23 = 114
    F24 = 115
    NumPadComma = 133
    Ro = 135
    KatakanaHiragana = 136
    Yen = 137
    Henkan = 138
    Muhenkan = 139
    NumPadCommaPc98 = 140
    HangulEnglish = 144
    Hanja = 145
    Katakana = 146
    Hiragana = 147
    ZenkakuHankaku = 148
    LeftControl = 224
    LeftShift = 225
    LeftAlt = 226
    LeftGui = 227
    RightControl = 228
    RightShift = 229
    RightAlt = 230
    HidKeyboardKey_RightGui = 231

# Enum for key modifier
class KeyModifier(str, Enum):
    Control = (1<<( 0 )) 
    Shift = (1<<( 1 )) 
    LeftAlt = (1<<( 2 )) 
    RightAlt = (1<<( 3 )) 
    Gui = (1<<( 4 )) 
    CapsLock = (1<<( 8 )) 
    ScrollLock = (1<<( 9 )) 
    NumLock = (1<<( 10 )) 
    Katakana = (1<<( 11 )) 
    Hiragana = (1<<( 12 ))

# Enum for stick
class Stick(str, Enum):
    Left = 'LEFT'
    Right = 'RIGHT'

# Enum for game meta data
class GameMetadata(str, Enum):
    Icon = 'icon'
    Version = 'version'
    Rating = 'rating'
    Author = 'author'
    Name = 'name'

# Helper class for sequential input
class Sequence:
    __Wait = 'W'
    __Press = '+'
    __Release = '-'

    @staticmethod
    def Click(button: Button) -> str:
        return f'{button.value}'

    @staticmethod
    def WaitTime(time_in_ms: int) -> str:
        assert(time_in_ms >= 0)
        return f'{Sequence.__Wait}{str(time_in_ms)}'
    
    @staticmethod
    def Press(button: Button) -> str:
        return f'{Sequence.__Press}{button.value}'
    
    @staticmethod
    def Release(button: Button) -> str:
        return f'{Sequence.__Release}{button.value}'

    def __init__(self):
        super().__init__()
        self.__sequence: list[str] = []
        self.__wait_in_ms: int = 0
    
    def Clear(self):
        self.__sequence: list[str] = []
        self.__wait_in_ms: int = 0

    def GetCommand(self) -> str:
        return ','.join(self.__sequence)

    def GetSequence(self) -> str:
        return self.__sequence

    def SetSequence(self, sequence: list[str]):
        self.__sequence = sequence
        return self

    def Sync(self):
        time.sleep(self.__wait_in_ms/1000)
        self.__wait_in_ms = 0

    def AddClick(self, button: Button):
        self.__sequence.append(Sequence.Click(button))
        return self
    
    def AddWait(self, time_in_ms: int):
        self.__sequence.append(Sequence.WaitTime(time_in_ms))
        self.__wait_in_ms += time_in_ms
        return self
    
    def AddPress(self, button: Button):
        self.__sequence.append(Sequence.Press(button))
        return self

    def AddRelease(self, button: Button):
        self.__sequence.append(Sequence.Release(button))
        return self
    
    def AddMoveStick(self, stick: Stick, x_position: int, y_position: int):
        pl: str = ''
        if(stick == Stick.Left):
            pl += '%'
        else:
            pl += '&'
        assert((-0x8000 <= x_position <= 0x7FFF) and (-0x8000 <= y_position <= 0x7FFF))
        pl += f'{str(x_position)},{str(y_position)}'
        self.__sequence.append(pl)
        return self

    
    def AddClickSequence(self, sequence: list[Button]):
        for click in sequence:
            self.AddClick(click)
        return self
    
    def AddClickSequenceWithWait(self, sequence: list[Button], time_in_ms: int = 100):
        for click in sequence:
            self.AddClick(click).AddWait(time_in_ms)
            self.__wait_in_ms += time_in_ms
        return self
    
    def AddPressSequence(self, sequence: list[Button]):
        for click in sequence:
            self.AddPress(click)
        return self
    
    def AddReleaseSequence(self, sequence: list[Button]):
        for click in sequence:
            self.AddRelease(click)
        return self
    

# Main class for using sys-botbase 
class SysBot:
    '''
    SysBot class using https://github.com/olliz0r/sys-botbase \n
    This works well only when sys-botbase is installed on the target switch
    '''
    def __init__(self, ip: str, port: int = 6000, click_global_delay:float = 0.0)  -> None:
        self.SetIpAndPort(ip, port)
        self.SetDelay(click_global_delay)
        self.Connect()
        self.SetTimeout(1)

    def SetIpAndPort(self, ip: str, port: int = 6000)  -> None:
        self.__ip: str = ip
        self.__port: int = port
    
    def SetDelay(self, delay: float = 0.0)  -> None:
        if(delay < 0.0):
            delay = 0.0
        self.__click_delay = delay

    def Connect(self)  -> None:
        self.soc: socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.soc.connect((self.__ip, self.__port))

    def Disconnect(self)  -> None:
        self.soc.close()

    def SendCommand(self, payload: str)  -> None:
        payload += '\r\n'
        self.soc.sendall(payload.encode())
    
    def SetTimeout(self, second: float):
        self.soc.settimeout(second)

    def ReceiveData(self, size_to_receive: int) -> bytes:
        try:
            return self.soc.recv(size_to_receive)
        except:
            return None
    
    def ReceiveDataAndWait(self, size_to_receive: int) -> bytes:
        data: bytes = b''
        while(len(data) < size_to_receive):
            now: bytes = self.ReceiveData(size_to_receive)
            if(now == None):
                break
            if(len(now) == 0):
                break
            data += now
        return data


    def Click(self, button: Button) -> None:
        self.SendCommand(f'click {button.value}')
        time.sleep(self.__click_delay)
    
    def ClickSequence(self, sequence: Sequence) -> None:
        self.SendCommand(f'clickSeq {sequence.GetCommand()}')
    
    def ClickSequenceAndSync(self, sequence: Sequence) -> None:
        self.ClickSequence(sequence)
        sequence.Sync()

    def ClickCancel(self) -> None:
        self.SendCommand('clickCancel')
    
    def SetStick(self, stick: Stick, x_value: int, y_value: int) -> None:
        assert(-0x8000 <= x_value <= 0x7FFF and -0x8000 <= y_value <= 0x7FFF)
        self.SendCommand(f'setStick {stick.value} {Hex(x_value)} {Hex(y_value)}')

    def Press(self, button: Button) -> None:
        self.SendCommand(f'press {button.value}')
    
    def Release(self, button: Button) -> None:
        self.SendCommand(f'release {button.value}')

    def DetachController(self) -> None:
        self.SendCommand(f'detachController')

    def Touch(self, *x_and_y_location_pairs: list[int]) -> None:
        param: str = ''
        for x_loc, y_loc in x_and_y_location_pairs:
            assert(0 <= x_loc <= 1280 and 0 <= y_loc <= 720)
            param += f'{str(x_loc)} {str(y_loc)} '
        self.SendCommand(f'touch {param}')
    
    def TouchHold(self, x_location: int, y_location: int, milliseconds_to_hold: int) -> None:
        assert(0 <= x_location <= 1280 and 0 <= y_location <= 720)
        self.SendCommand(f'touchHold {str(x_location)} {str(y_location)} {str(milliseconds_to_hold)}')

    def TouchDraw(self, *x_and_y_location_pairs: list[int]) -> None:
        param: str = ''
        for x_loc, y_loc in x_and_y_location_pairs:
            assert(0 <= x_loc <= 1280 and 0 <= y_loc <= 720)
            param += f'{str(x_loc)} {str(y_loc)} '
        self.SendCommand(f'touchDraw {param}')

    def TouchCancel(self) -> None:
        self.SendCommand(f'touchCancel')

    def TypeKey(self, *keys: Key) -> None:
        param: str = ''
        for key in keys:
            param += f'{str(key.value)} '
        self.SendCommand(f'key {param}')
    
    def TypeKeyWithModifier(self, *key_and_modifier_pairs: list[Key, KeyModifier]) -> None:
        param: str = ''
        for key, modifier in key_and_modifier_pairs:
            param += f'{str(key.value)} {str(modifier.value)} '
        self.SendCommand(f'keyMod {param}')
    
    def TypeKeyMultiple(self, *keys: Key) -> None:
        param: str = ''
        for key in keys:
            param += f'{str(key.value)} '
        self.SendCommand(f'keyMulti {param}')


    def Peek(self, relative_address_of_heap: int, amount_of_bytes_to_read: int) -> str:
        self.SendCommand(f'peek {Hex(relative_address_of_heap)} {Hex(amount_of_bytes_to_read)}')
        return self.ReceiveData(2*amount_of_bytes_to_read + 1)[:-1].decode()
    
    def PeekAbsolute(self, absolute_address: int, amount_of_bytes_to_read: int) -> str:
        self.SendCommand(f'peekAbsolute {Hex(absolute_address)} {Hex(amount_of_bytes_to_read)}')
        return self.ReceiveData(2*amount_of_bytes_to_read + 1)[:-1].decode()
    
    def PeekMain(self, relative_address_of_nso_main: int, amount_of_bytes_to_read: int) -> str:
        self.SendCommand(f'peekAbsolute {Hex(relative_address_of_nso_main)} {Hex(amount_of_bytes_to_read)}')
        return self.ReceiveData(2*amount_of_bytes_to_read + 1)[:-1].decode()

    def PeekMultiple(self, *relative_address_of_heap_and_amount_pairs: list[int]) -> list[str]:
        param: str = ''
        sizes: list[int] = []
        for address, amount in relative_address_of_heap_and_amount_pairs:
            sizes.append(address)
            param += f'{Hex(address)} {Hex(amount)} '
        self.SendCommand(f'peekMulti {param}')
        data = self.ReceiveData(2*sum(sizes) + 1)
        result: list[str] = []
        for size in sizes:
            result.append(data[:size].decode())
            data = data[size:]
        return result
    
    def PeekAbsoluteMultiple(self, *absolute_address_of_heap_and_amount_pairs: list[int]) -> list[str]:
        param: str = ''
        sizes: list[int] = 0
        for address, amount in absolute_address_of_heap_and_amount_pairs:
            sizes.append(amount)
            param += f'{Hex(address)} {Hex(amount)} '
        self.SendCommand(f'peekAbsoluteMulti {param}')
        data = self.ReceiveData(2*sum(sizes) + 1)
        result: list[str] = []
        for size in sizes:
            result.append(data[:size].decode())
            data = data[size:]
        return result

    def PeekMainMultiple(self, *relative_address_of_nso_main_and_amount_pairs: list[int]) -> list[str]:
        param: str = ''
        sizes: list[int] = 0
        for address, amount in relative_address_of_nso_main_and_amount_pairs:
            sizes.append(amount)
            param += f'{Hex(address)} {Hex(amount)} '
        self.SendCommand(f'peekMainMulti {param}')
        data = self.ReceiveData(2*sum(sizes) + 1)
        result: list[str] = []
        for size in sizes:
            result.append(data[:size].decode())
            data = data[size:]
        return result
    
    def PointerPeek(self, amount_of_bytes_to_read: int, relative_address_to_nso_main: int, *offsets: int) -> str:
        param: str = ''
        for offset in offsets:
            param += f'{Hex(offset)} '
        self.SendCommand(f'pointerPeek {Hex(amount_of_bytes_to_read)} {Hex(relative_address_to_nso_main)} {param}')
        return self.ReceiveData(2*amount_of_bytes_to_read + 1)[:-1].decode()
    
    def Pointer(self, relative_address_to_nso_main: int, *offsets: int) -> int:
        param: str = ''
        for offset in offsets:
            param += f'{Hex(offset)} '
        self.SendCommand(f'pointer {Hex(relative_address_to_nso_main)} {param}')
        return int(self.ReceiveData(16), 16)
    
    def Poke(self, relative_address_to_heap: int, data_to_write: int) -> None:
        self.SendCommand(f'poke {Hex(relative_address_to_heap)} {Hex(data_to_write)}')
    
    def PokeAbsolute(self, absolute_address: int, data_to_write: int) -> None:
        self.SendCommand(f'pokeAbsolute {Hex(absolute_address)} {Hex(data_to_write)}')

    def PokeMain(self, relative_address_to_nso_main: int, data_to_write: int) -> None:
        self.SendCommand(f'pokeMain {Hex(relative_address_to_nso_main)} {Hex(data_to_write)}')

    def PointerPoke(self, data_to_write: int, relative_address_to_nso_main: int, *offsets: int) -> None:
        param: str = ''
        for offset in offsets:
            param += f'{Hex(offset)} '
        self.SendCommand(f'pointerPoke {Hex(data_to_write)} {Hex(relative_address_to_nso_main)} {param}')
    
    def Freeze(self, absolute_address: int, value_to_freeze: int) -> None:
        self.SendCommand(f'freeze {Hex(absolute_address)} {Hex(value_to_freeze)[2:]}')

    def Unfreeze(self, absolute_address: int) -> None:
        self.SendCommand(f'unFreeze {Hex(absolute_address)}')

    def FreezeCount(self) -> None:
        self.SendCommand(f'freezeCount')

    def FreezeClear(self) -> None:
        self.SendCommand(f'freezeClear')
    
    def FreezePause(self) -> None:
        self.SendCommand(f'freezePause')

    def FreezeUnpause(self) -> None:
        self.SendCommand(f'freezeUnpause')


    def ScreenShot(self) -> bytes:
        self.SendCommand(f'pixelPeek')
        return bytes.fromHex(self.ReceiveDataAndWait(0x7D000).decode())
    
    def ScreenOff(self) -> None:
        self.SendCommand(f'screenOff')

    def ScreenOn(self) -> None:
        self.SendCommand(f'screenOn')

    def GetTitleID(self) -> str:
        self.SendCommand(f'getTitleID')
        return self.ReceiveData(10000)[:-1].decode()
    
    def GetTitleVersion(self) -> str:
        self.SendCommand(f'getTitleVersion')
        return self.ReceiveData(10000)[:-1].decode()

    def GetSystemLanguage(self) -> bytes:
        self.SendCommand(f'getSystemLanguage')
        return self.ReceiveData(10000)[:-1]
    
    def GetBuildID(self) -> str:
        self.SendCommand(f'getBuildID')
        return self.ReceiveData(10000)[:-1].decode()
    
    def GetHeapBase(self) -> int:
        self.SendCommand(f'getHeapBase')
        return int(self.ReceiveData(16)[:-1], 16)
    
    def GetMainNsoBase(self) -> int:
        self.SendCommand(f'getMainNsoBase')
        return int(self.ReceiveData(16)[:-1], 16)
    
    def IsProgramRunning (self, program_id: int) -> bool:
        self.SendCommand(f'isProgramRunning {Hex(program_id)}')
        return (self.ReceiveData(10000)[:-1] == '1')
    
    def GetGameMetadata(self, type: GameMetadata):
        self.SendCommand(f'game {type.value}')
        if(type == GameMetadata.Icon):
            return bytes.fromHex(self.ReceiveDataAndWait(0x1000).decode())
        else:
            return self.ReceiveData(10000)[:-1].decode()

    def GetVersion(self) -> str:
        self.SendCommand(f'getVersion')
        return self.ReceiveData(10000)[:-1].decode()
    
    def GetCharge(self) -> int:
        self.SendCommand(f'charge')
        return int(self.ReceiveData(4)[:-1])