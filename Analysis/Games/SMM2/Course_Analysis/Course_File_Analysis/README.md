# Overview

This article talks about the Course File Format that is used in Super Mario Maker 2.

# Course Format

Information on Course Format has been analyzed by liamadvance[^1]. 

## Updates

In the recent update, the following changes were made different from liamadvance’s results.

- Snake Block
    - Unknown removed
    - Reserved removed
    - 1 byte padding after Snake Block Node

## ImHex Pattern

We have created a pattern that can be used in ImHex[2] from a known Course Format. This pattern can be downloaded [here](./src/smm2/). 

### Installation

The downloaded compressed folder has the following structure. 

```
smm2
 ├─course
 │  └─header
 ├─encryption
 │  └─header
 └─object
```

Then, add the uncompressed folder inside the installation folder of ImHex\includes (C:\Program Files\ImHex\includes). The following is the result of it. 

```
includes
 ├─hex
 │  ├─impl
 │  └─type
 ├─smm2
 │  ├─course
 │  │  └─header
 │  ├─encryption
 │  │  └─header
 │  └─object
 ├─std
 └─type
     └─types
```

### Usage

Open the Course(BCD) file that you want to analyze, and include the following pattern.

```
#include "smm2/main.hexpat"

smm2::course::Course course @ 0x00;
```

# Test Files

[map_for_clearpipe_and_piranhacreeper.bcd](./map/map_for_clearpipe_and_piranhacreeper.bcd)

[map_for_exclamation.bcd](./map/map_for_exclamation.bcd)

[map_for_ground.bcd](./map/map_for_ground.bcd)

[map_for_icicle.bcd](./map/map_for_icicle.bcd)

[map_for_snake_and_sound.bcd](./map/map_for_snake_and_sound.bcd)

[map_for_trackblock.bcd](./map/map_for_trackblock.bcd)

[map_for_tracks.bcd](./map/map_for_tracks.bcd)


[^1] https://github.com/liamadvance/smm2-documentation

[^2] https://github.com/WerWolv/ImHex