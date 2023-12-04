# Set Up Yuzu Public Room Server 

---

# Overview

This article is a summary of the site on yuzu to set a public room server[^1].

# Prerequisite

This is based on Linux Ubuntu 22.0.4.

First, open the port you want for udp (You don't have to open for  tcp).

```bash
iptables -I INPUT -p udp --dport 3333 -j ACCEPT
```

If the firewall is enabled, allow the port.

```bash
ufw allow 3333
```

Now install the docker.

```bash
sudo wget -qO- http://get.docker.com/ | sh
```

Check if the docker has been installed successfully.

```bash
docker --version
```

Make a container to run with dockers

```bash
sudo docker run -d \
  --publish 3333:3333/udp \
  yuzuemu/yuzu-multiplayer-dedicated \
  --room-name "<INSERT ROOM NAME HERE>" \
  --room-description "<INSERT ROOM NAME HERE>" \
  --preferred-game "<INSERT GAME TITLE HERE>" \
  --preferred-game-id "<INSERT TITLE ID HERE>" \
  --port 3333 \
  --max_members 4 \
  --token "<INSERT YUZU ACCOUNT TOKEN HERE>" \
  --enable-yuzu-mods \
  --web-api-url https://api.yuzu-emu.org
```

The port has been set to 3333. However, you can choose any port you want.

The room name must be at least 4 alphanumeric characters.

For the token, you must follow the steps below:

- First, go to this [site](https://community.citra-emu.org/) and sign up for membership. [^2]
- After signing up, go into this [link](https://yuzu-emu.org/wiki/yuzu-web-service/) and you'll see the token issued.[^3]

Finally, run the command above and you're done.

# Connect to Yuzu

On yuzu, go to Multiplayer → Browse Public Game Lobby

![YUZU_Public_Room](img/YUZU_Public_Room.png)

If you type in the room name, you'll see the game room you created. If it doesn't show up, there's a high probability that it's a firewall problem. Disable the firewall with the command below

```bash
ufw disable
```

[^1]: https://yuzu-emu.org/help/feature/multiplayer/#:~:text=yuzu
[^2]: https://community.citra-emu.org/
[^3]: https://yuzu-emu.org/wiki/yuzu-web-service/
