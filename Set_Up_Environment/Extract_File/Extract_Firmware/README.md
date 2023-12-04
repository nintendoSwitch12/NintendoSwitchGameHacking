# 펌웨어 추출

---

# Overview

This article explains how to extract firmware from a Nintendo Switch booted into Hekate.

# Dumping Firmware

In order to dump Nintento Switch firmware, we will be using TegraExplorer [^1].

## TegraExplorer Installation

Put TegraExplorer.bin file under bootloader/payloads folder on the SD card.

## TegraExplorer Usage

After booting into Hekate, choose payload ->TegraExplorer.bin. Then, the following screen should appear.

![Tegra_Explorer](img/Tegra_Explorer.jpeg)

Then, select Scripts -> FirmwareDump.te -> Dump (SysMMC/EmuMMC). The extracted firmware will be located at tegraexplorer/Firmware/(version)/ on the SD card.

# Reference

[^1]: https://github.com/suchmememanyskill/TegraExplorer