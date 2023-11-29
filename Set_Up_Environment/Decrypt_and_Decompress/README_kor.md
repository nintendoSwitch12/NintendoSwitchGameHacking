# 닌텐도 파일 복호화 및 Decompress

---

# 개요

닌텐도에서 게임을 실행또는 업데이트를 하기 위해서는 NSP확장자를 갖고 있는 PFS0 파일에서 다른 파일들을 추출할 수 있다.

NCA(Nintendo Content Archive)는 닌텐도 스위치에서 사용하는 파일 형식이며 전체 파일은 암호화되어 있다[^1]. 

NCA 파일은 보통 RomFS와 ExeFS를 포함한다. RomFS는 닌텐도 스위치에서 사용하는 리소스들을 포함하며, ExeFS는 닌텐도 스위치의 실행 파일인 NSO 파일을 포함한다. NSO 파일은 LZ4를 통해 압축되어 있다[^2]. 

이 글에서는 Hactool[^3]과 nstool[^4]를 이용해 NCA를 복호화하고 NSO 파일을 Decompress하는 방법을 설명한다.

# PFS0 파일에서 다른 파일 추출

PFS0 파일을 복호화하기 위해서는 다음과 같이 명령어를 치면 된다.

```bash
hactool.exe -k [prod.keys] -t pfs0 [NSP file] --outdir=[out dir]
```

# title key 추출

title key를 추출하기 위해서는 다음과 같이 명령어를 치면 된다. Data 부분이 title key다.

```bash
nstool.exe -k prod.keys -v [tik file]
```

# NCA 파일 복호화

NCA 파일을 복호화하기 위해서는 다음과 같이 명령어를 치면 된다.

```bash
hactool.exe -k [prod.keys] -t nca [NCA file] --exefsdir=[out dir when file is ExeFS] --romfsdir=[out dir when file is RomFS] {--titlekey=[title key]}}
```

# NSO 파일 Decompress

NSO 파일을 Decompress하기 위해서는 다음과 같이 명령어를 치면 된다.

```bash
hactool.exe -k [prod.keys]-t nso0 "[target NSO file]" --uncompressed=[decompressed NSO file]
```

# 스크립트 파일

NCA 파일을 복호화 할 때, 각 NCA마다 명령어를 쳐주어야 한다. 또한 NSO 파일을 Decompress할 때도 마찬가지이다. 따라서 스크립트를 짜서 사용하는 것이 편하다.

```python
# Author: Ju Chanhyoung
# Date: 2023-09-04
# Decription: Decrypt & Extract atual files from nca files of firmware.

import os
import argparse

def make_out_dir(out_dir):
    os.makedirs(f'{out_dir}\\exefs', exist_ok=True)
    os.makedirs(f'{out_dir}\\romfs', exist_ok=True)

def decrypt_firmware(firmware_dir, prod_key, out_dir):
    for file in os.listdir(firmware_dir):
        os.system(f'.\hactool.exe -k {prod_key} -t nca "{firmware_dir}\{file}" --exefsdir={out_dir}\exefs\{file} --romfsdir={out_dir}\\romfs\{file}')

def decompress_nso(prod_key, out_dir):
    for exefs_dir in os.listdir(f'{out_dir}\exefs'): 
        target_dir = f'{out_dir}\exefs\{exefs_dir}'
        if 'main' in os.listdir(target_dir):
            os.system(f'.\hactool.exe -k {prod_key} -t nso0 "{target_dir}\main" --uncompressed={target_dir}\main_decompressed')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Decrypt & Extract actual files from nca files of firmware.')
    parser.add_argument('out_dir', metavar='output_folder', type=str, help='location to save out files.')
    parser.add_argument('firmware_dir', metavar='firmware_folder', type=str, help='location of nca files')
    parser.add_argument('prod_key', metavar='prod_key', type=str, help='location of prod.keys')
    args = parser.parse_args()

    make_out_dir(args.out_dir)
    decrypt_firmware(args.firmware_dir, args.prod_key, args.out_dir)
    decompress_nso(args.prod_key, args.out_dir)
```

[^1]: https://switchbrew.org/wiki/NCA
[^2]: https://switchbrew.org/wiki/NSO
[^3]: https://github.com/SciresM/hactool
[^4]: https://github.com/jakcron/nstool