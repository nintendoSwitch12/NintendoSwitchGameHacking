# Proxy Server Setting

---

# Why Proxy

게임 서버로 패킷을 전송할 때, 중간에 패킷을 수정하기 위해 씀. 우리는 우분투 22.04에 forward proxy를 설치할 거다

# How to set?

Apache2를 설치하자

```bash
sudo apt update
sudo apt install -y apache2 apache2-doc apache2-utils
```

이제 proxy에 필요한 모듈을 설치하자

```bash
sudo a2enmod ssl proxy proxy_connect proxy_ajp proxy_balancer proxy_html proxy_http headers proxy_fcgi rewrite
```

이제 `/etc/apache2/sites-enabled`에 가서 `000-default.conf` 를 지워야한다 (안 지우면 /etc/apache2/sites-enabled/000-default.conf is a real file, not touching it 이 에러가 뜬다)

그리고 `/etc/apache2/sites-available`에 가서 `000-default.conf` 를 다음과 같이 수정한다.

```xml
<VirtualHost *:31415>
    ProxyRequests On
    ProxyVia On

    AllowCONNECT 80 443 563

    <Proxy "*">
     	AllowOverride All
        Options All -Indexes
        Require all granted
    </Proxy>

    ErrorLog ${APACHE_LOG_DIR}/error_forward_proxy.log
    CustomLog ${APACHE_LOG_DIR}/access_forward_proxy.log combined
</VirtualHost>
```

VirtualHost *:31415은 proxy 기본 포트를 31415으로 바꾼다는 거다. (기본은 80이다) 원하면 다른 포트로 바꿔도 된다. 우리는 8080을 기본 포트로 할 거다.

기본 포트를 8080으로 바꿨으니 이걸 `/etc/apache2/ports.conf`에서 8080을 listen한다고 알려줘야한다.

기본으로 Listen 80이 위에 있을텐데 그냥 밑에 Listen 8080을 추가해주면 된다.

모든 설정을 마쳤으면 밑에 명령어를 실행한다

```bash
systemctl reload apache2
```

그럼 `/etc/apache2/sites-enabled` 에  `000-default.conf` 이 심볼릭 링크로 만들어져있을 것이다.

잘 실행되는지 확인하기 위해 로그 파일을 보면 된다

`/var/log/apache2/` 로 가서 아래 명령어를 실행하면 된다.

```bash
tail -f access_forward_proxy.log
```

그럼 proxy 서버를 사용할 때 실시간으로 log가 쌓이는 게 보일 거다.

*** 설정한 포트는 port forwarding을 해줘야한다. (iptables이나 ufw를 통해)

# Switch Setting

Home 버튼을 누른 후 설정에 들어간다.

인터넷에 들어가서 인터넷 설정에 들어간다.

연결된 와이파이를 누른 후 설정 변경을 누른다.

아래 Proxy 설정에서 위에 proxy 설정한 서버 ip하고 설정한 port (8080)를 넣어준다. Auto-authentication도 켜줘서 username password도 설정해준다.

# 참고자료 (참고한 사이트)

[1] [https://watilearnd2day.wordpress.com/2017/06/23/how-to-setup-an-apache-forward-proxy-server-on-ubuntu-14-04/](https://watilearnd2day.wordpress.com/2017/06/23/how-to-setup-an-apache-forward-proxy-server-on-ubuntu-14-04/)

[2] [https://bangu4.tistory.com/156](https://bangu4.tistory.com/156)