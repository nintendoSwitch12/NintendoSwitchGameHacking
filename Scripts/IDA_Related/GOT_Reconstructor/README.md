# Table of Contents
- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Usage](#usage)


# Overview

스위치에도 GOT 같은 영역이 존재한다. 따라서 SDK 함수를 호출할 때, GOT를 거친다. 하지만 이 부분에는 심볼이 없어 분석하기 어려웠고, 함수를 따라가서 GOT에 심볼을 적용하는 스크립트를 작성했다.
Switches also have areas like GOT. So when you call the SDK function, it goes through the GOT. However, since there are no symbols in this area, it was difficult to analyze, so I wrote a script to follow the function and apply symbols to the GOT.

# Usage

GOTReconstructor 객체를 생성할 때 탐색을 시작할 주소와 끝날 주소를 넣어준뒤, Reconstruct 메서드를 호출하면 된다.
When you create a GOTReconstructor object, you give it an address to start and an address to end the search, and then call the Reconstruct method.

```python
GOTReconstructor(0x0000000084536D70,0x000000008453A798).Reconstruct()
```