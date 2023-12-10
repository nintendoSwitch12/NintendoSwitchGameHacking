# Setup Environment for Debugging

---

# Overview

---

The article describes the environment setup required for dynamic analysis.

# Content

---

## Prerequisite

---

### Atmosphere Installation

---

Nintendo Switch does not allow debugger connections. Therefore, Atmosphere has to been installed to enable debugging on Nintendo Switch.

### Atmosphere Setting

---

In order to enable gdbstub for debugging, add the following lines to "/atmosphere/config/system_settings.ini" on the SD card where Atmosphere is installed.

```
[atmosphere]
enable_htc = u8!0x0
enable_standalone_gdbstub = u8!0x1
```

### GDB Installation

---

Nintendo Switch uses arm architecture, so you need a GDB that supports arm architecture. Therefore, install a GDB that supports multi-architecture.

```bash
sudo apt-get install -y gdb-multiarch
```

## Debugging on Nintendo Switch

---

### Using GDB

---

Connect the Nintendo Switch and the computer on the same network. 

Find the IP address of the Switch (System Setting -> Internet -> Connection Status -> IP Address). Then, run the following commands on the computer.

```bash
gdb-multiarch
target extended-remote <switch ip>:22225
monitor wait application
```

Turn on the game on the Nintendo Switch and the following message will appear on GDB.

```
Send `attach 0xNN` to attach.
```

Enter ‘attach 0xNN’ as it says and you have successfully attached to the game. 

## Using IDA(version 7.7)

---

![IDA_Connetion_1](img/IDA_Connetion_1.png)

Debugger -> Attach -> Remote GDB debugger

![IDA_Connetion_2](img/IDA_Connetion_2.png)

Configure hostname (IP address of Nintendo Switch), port (default is 22225)

![IDA_Connetion_3](img/IDA_Connetion_3.png)

Debug options -> check the above options

![IDA_Connetion_4](img/IDA_Connetion_4.png)

set specific options -> Configuration -> ARM64 (AArch64)

Now, run the game on the console.

Then, come back to IDA and click ok. This will show you a windows with a list of process IDs.

Choose Application PID to attach to the console.

## Debugging on Yuzu

---

**Two things are needed to run Switch games on yuzu [^1].**

1. [XCI/NSP file](#merge-split-xcinsp)
2. [Keys](#yuzu-key-파일-설정)

### Merge Split XCI/NSP

---

By default, when XCI or NSP file is dumped, they will be split into files since the SD card uses FAT32 file format and this can hold only upto 4GB per file. Therefore, it is necessary to merge this files to create a single file to run on Yuzu. (Here we have chose XCI but you can just NSP as well.)

![Merge_NSP_XCI_1](img/Merge_NSP_XCI_1.png)

In the case above, the XCI file is divided into two parts.

In order to merge the files, follow the steps below.

![Merge_NSP_XCI_2](img/Merge_NSP_XCI_2.png)

**Download the above program[^2].**

![Merge_NSP_XCI_3](img/Merge_NSP_XCI_3.png)

After running the program, click on input and select the XCI file numbered 0

Then, click on output and select a folder to output. 

Click on fuse and the files should have merged to produce a single file like below. 

![Merge_NSP_XCI_4](img/Merge_NSP_XCI_4.png)

### yuzu key configuration

---

In order to play games on yuzu, you have to give prod.keys.

![Yuzu_Key_1](img/Yuzu_Key_1.png)

Go to ****yuzu folder and click on keys folder. (If you don't have one, make one.)

![Yuzu_Key_2](img/Yuzu_Key_2.png)

You can put the prod.keys file in this location.

### Run XCI/NSP on yuzu

---

![Run_Yuzu_1](img/Run_Yuzu_1.png)

After running yuzu, press **Add New Game Directory**, and select the game folder.

![Run_Yuzu_2](img/Run_Yuzu_2.png)

Now you should see a screen like above. Click on the game to play.

### Update Game

---

![Update_Yuzu_1](img/Update_Yuzu_1.png)

Click on **file -> install files to NAND** and choose the UPD file to update the game to the latest version.

## IDA debug

---

In order to debug on yuzu, you have to enable gdb stub.

![Debug_Setting_1](img/Debug_Setting_1.png)

From yuzu, go to Emulation → configure → general → debug and specify the port you want in the red circle.

Then, follow the same steps for IDA debugging above.

(ip addr = 127.0.0.1, port = 6543)

Now, if you run a game on yuzu, it will stop at "lauching…" screen.

This means that it is waiting for gdb to attach.

![Debug_Setting_2](img/Debug_Setting_2.png)

Attach to the game using IDA like above. (Here you won’t see application pid but only pid for yuzu)

## Rebase Program

---

### yuzu

---

Find the base address. (You can use the “get info” command in IDA .)

![Rebase_1](img/Rebase_1.png)

edit→Segments→Rebase program

![Rebase_2](img/Rebase_2.png)

Enter the base address and the program will be rebased to that address. 

[^1]: https://yuzu-emu.org/help/quickstart/
[^2]: https://github.com/oMaN-Rod/nxDumpFuse/releases/tag/v1.0.3
