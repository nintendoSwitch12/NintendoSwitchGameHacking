# Decrypt and Decompress

---

# Overview

To run or update games on Nintendo, you have to dump files from the PFS0 file that has a NSP extension.

The Nintendo Content Archive (NCA) is a file format used by the Nintendo Switch, and it is encrypted [^1].

NCA files typically include RomFS and ExeFS. RomFS includes resources used by Nintendo Switch, and ExeFS includes the NSO file, which is the main executable file of Nintendo Switch. The NSO files are compressed using LZ4 [^2].

This article describes how to decrypt NCA and decompress NSO files using Hactool[^3] and nstool[^4].

# Dumping files from PFS0 files

**To decrypt PFS0 files, run the command below.**

```bash
hactool.exe -k [prod.keys] -t pfs0 [NSP file] --outdir=[out dir]
```

# Dumping title keys

**In order to extract the title keys, run command below. Data part is the title keys.**

```bash
nstool.exe -k prod.keys -v [tik file]
```

# Decrypting NCA files

**To decrypt NCA files, run the command below.**

```bash
hactool.exe -k [prod.keys] -t nca [NCA file] --exefsdir=[out dir when file is ExeFS] --romfsdir=[out dir when file is RomFS] {--titlekey=[title key]}
```

# Decompress NSO Files

**To decompress NSO files, run the command below.**

```bash
hactool.exe -k [prod.keys]-t nso0 "[target NSO file]" --uncompressed=[decompressed NSO file]
```

# Script File

When you decrypt NCA files, you must type the command for each NCA file. The same is true for decompressing NSO files. Therefore, it is convenient to create a script.

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