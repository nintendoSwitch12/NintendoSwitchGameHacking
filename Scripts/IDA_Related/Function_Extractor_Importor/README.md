# Table of Contents
- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Usage](#usage)


# Overview
When analyzing MK8D, `Switch Mapper` cannot resolve the symbols of the SDK.

Therefore, it was difficult to check the SDK functions where the symbols exist. Therefore, I wrote the following script to analyze them easily.

# Usage

Enter the following command in the IDA loading the binary you want to extract.

> [!IMPORTANT]
> The position of each function in the binary to be extracted must be the same as the corresponding function in the binary to be applied. 
> In simple terms, if you are extracting functions from an SDK, the base of the SDK in the binary to be extracted and the binary to be applied must be the same.

```python
ExtractFunctionList(file_path: str, do_ignore_sub: bool = True)
```

Enter the following command in the binary where you want to apply the extracted information.

```python
ImportFunctionList(file_path: str, do_ignore_sub: bool = True)
```
