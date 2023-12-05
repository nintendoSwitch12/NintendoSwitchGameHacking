# Packet Analysis Tip


## Overview

In order to analyze the packets from the Nintendo Switch, one can set up a proxy server. The configuration can be done in proxy server option under Wifi in the Nintendo Switch. However, if the proxy server only supports CLI environment, Wireshark would not run. Therefore, an alternative to Wireshark in CLI envrionment called Tshark can be used. This article talks about how one can analyze packets from the Nintendo Switch using Tshark.

## How to use?

```bash
apt install tshark
```

Run the above command to install tshark

```xml
tshark -D
```

The above command will display various NICs installed on the computer. Find the NIC number that can capture the packet. 

```xml
tshark -i 3 -Y 'tcp.port == 80 && ip.addr == 192.168.1.10' -t a --color
```

Using the above command, Tshark will be run.

Below are few options that have been applied:

-i : NIC number

-Y : Filter 

-t : Show time

—color : Show color

This will create a .pcapng file in /tmp directory which can later be moved to a GUI environment to open with Wireshark.