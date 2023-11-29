# Background

MK8D를 분석할 때, [SwitchMapper.py, sysTracer.py 활용한 분석](https://www.notion.so/SwitchMapper-py-sysTracer-py-e312b893e50a4352ae70c1176ae82408?pvs=21)를 사용할 경우 SDK의 심볼을 살릴 수 없다.

따라서 심볼이 존재하는 SDK 함수들을 확인하기 힘들었다. 따라서 이를 편하게 분석하고자 이와 같은 스크립트를 작성했다.

# Usage

추출을 원하는 바이너리에서 다음 커맨드를 입력한다.

> [!IMPORTATNT]
추출할 바이너리에서 각 함수의 위치는 적용할 바이너리에서 대응되는 함수의 위치와 같아야 한다.
쉽게 생각하면 SDK에서 함수를 추출한다면 추출할 바이너리와 적용할 바이너리에서의 SDK의 베이스가 같아야한다.

```python
ExtractFunctionList(file_path: str, do_ignore_sub: bool = True)
```

추출한 정보를 적용하고 싶은 바이너리에서 다음 커맨드를 입력한다.

```python
ImportFunctionList(file_path: str, do_ignore_sub: bool = True)
```