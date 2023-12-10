# Proxy Server Setting

---

# Overview

When sending a packet to a game server, you can do MITM to modify the packet. This article talks about how to install forward proxy on Ubuntu 22.04.

# Installation

Install Apache2

```bash
sudo apt update
sudo apt install -y apache2 apache2-doc apache2-utils
```

Install the modules needed for proxy

```bash
sudo a2enmod ssl proxy proxy_connect proxy_ajp proxy_balancer proxy_html proxy_http headers proxy_fcgi rewrite
```

Now you have to go to /etc/apache2/sites-enabled and delete 000-default.conf (If you don't delete it, you will get an error saying “/etc/apache2/sites-enabled/000-default.conf is a real file, not touching it”)

Then, go to /etc/apache2/sites-available and modify 000-default.conf as follows.

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

VirtualHost*:31415 changes the proxy default port number to 31415 (The default is 80). You can switch to another port number if you want. We will use 8080 as the default port number.

Since the default port has been changed to 8080, /etc/apache2/ports.conf should be configured to listen to port number 8080.

Add the line 

```bash
Listen 8080
```

below “Listen 80”

Once you've finished setting up, run the command below

```bash
systemctl reload apache2
```

If everything went fine, you should see a symbolic link named '000-default.conf' in /etc/apache2/sites-enabled.

To check if it is working properly, check the log file.

Go to '/var/log/apache2/' and execute the command below.

```bash
tail -f access_forward_proxy.log
```

You'll see the log building up in real time when the proxy server has been set properly.

*** The configured port should be configured for port forwarding. (via iptables or ufw)

# Switch Setting

Press the Home button to enter the settings.

Under “Internet → Internet settings”, click “connected Wi-Fi” and click “Change Settings”.

In the proxy setting below, add the proxy-set server IP and the port number (8080) set above. Turn on “Auto-authentication” and sets the username password.

# Reference

- https://watilearnd2day.wordpress.com/2017/06/23/how-to-setup-an-apache-forward-proxy-server-on-ubuntu-14-04/
- https://bangu4.tistory.com/156