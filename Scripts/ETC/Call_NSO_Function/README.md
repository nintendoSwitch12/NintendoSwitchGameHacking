# Table of Contents
- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [NSO file](#nso-file)
- [Usage](#usage)
- [Example](#example)


# Overview

An NSO file is an executable file that runs on the Nintendo Switch. The Nintendo Switch is based on AARCH64, so we thought it would be possible to run NSO files with QEMU. 

This article describes how to run NSO files, especially the functions in the SDK file, through QEMU.

# NSO file

The format of NSO files can be found in SwitchBrew[^1]. According to that article, an NSO file is an ELF file with omitted content. 

NSO files can be loaded into IDA and analyzed using the `SwitchIDAProLoader`[^2].

# Usage

To execute functions from an NSO file in QEMU, follow these steps

1. load the NSO file into the switch, then dump all loaded memory.
    1. You can also load it into IDA, to do this install `SwitchIDAProLoader`[^2] and load the binary.
    2. Dump all NSO file data loaded via IDA Python or IDC.
    3. I recommand you dump NSO file data after run a game for a while.
2. Write the desired code in main.c and compile it to AARCH64.
3. If you get an error while executing the SDK function, patch the code to fix the error.

> [!IMPORTANT]
> The defined variable [`kSDKAddress`](./src/src/sdk.h) and [`kSDKSize`](./src/src/sdk.h) may be differnet depending on what game use the SDK.
> So before you use this, you should set the right address.s

# Example

An example of calling a SDK function, `GenerateHmacSha256Mac` to get the hash of a thumbnail file can be found at the following address

- [Example: Call GenerateHmacSha256Mac](./src/example/GenerateHmacSha256Mac/)
- [Example: Encrypt and Decrypt PIA](./src/example/EncryptAndDecryptPIA/)

[^1]: https://switchbrew.org/wiki/NSO
[^2]: https://github.com/pgarba/SwitchIDAProLoader
