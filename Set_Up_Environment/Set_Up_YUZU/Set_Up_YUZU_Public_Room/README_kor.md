# Yuzu Public Room Server 설정

---

# Prerequisite

yuzu 사이트에 나와있는 내용이다[^1]

여긴 Linux Ubuntu 기준이다.

일단 원하는 포트를 udp로 열어준다. (tcp는 굳이 안 열어도 된다)

```bash
iptables -I INPUT -p udp --dport 3333 -j ACCEPT
```

방화벽이 enable되어 있다면 해당 포트를 allow 해준다

```bash
ufw allow 3333
```

이제 docker를 설치하자

```bash
sudo wget -qO- http://get.docker.com/ | sh
```

잘 설치되어 있는지 확인해보자

```bash
docker --version
```

이제 docker로 실행할 container를 만들어주자

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

위에서는 3333을 포트로 설정했다. 이건 다른 포트로 바꿔도 상관없다.

preferred game하고 preferred game id는 굳이 안 넣어도 된다. 이건 yuzu에서 어떤 게임을 하고 싶은지 보여주는 것 뿐이다.

room name은 최소 4글자 alphanumeric이어야한다.

또한 token을 발행해야한다. 발행 방법은 아래와 같다.

일단 이 [사이트](https://community.citra-emu.org/)로 가서 회원가입을 하고.[^2]

이 [링크](https://yuzu-emu.org/wiki/yuzu-web-service/)로 들어가면 토큰이 보일 거다.[^3]

마지막으로 위에 명령어를 실행하면 끝이다.

# Yuzu에서 접속하기

Yuzu에서 Multiplayer → Browse Public Game Lobby를 누른다

![YUZU_Public_Room](img/YUZU_Public_Room.png)

Filters에서 설정했던 room name을 입력하면 밑에 게임 방이 뜰 거다. 만약 안 뜨면 방화벽 문제일 확률이 높다. 그럼 아래 명령어로 방화벽을 비활성화하자

```c
ufw disable
```

[^1]: https://yuzu-emu.org/help/feature/multiplayer/#:~:text=yuzu
[^2]: https://community.citra-emu.org/
[^3]: https://yuzu-emu.org/wiki/yuzu-web-service/