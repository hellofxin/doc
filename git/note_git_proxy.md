## git 3种协议
git 远程拉取有3种协议，http、ssh、git，举例如下：
git clone https://github.com/user/reponame.git
git clone git@github.com:user/reponame.git
git clone git://git.openwrt.org/reponame.git

## 为http协议设置代理

### 使用http代理http
git config --global http.proxy http://127.0.0.1:1080

### 使用socks代理http
git config --global http.proxy socks5://127.0.0.1:1080


### 取消http代理
git config --global --unset http.proxy

> 相关设置会体现在~/.gitconfig文件中

## 为git协议设置代理
git协议代理需要安装使用connect

编写gitproxy.sh
```
#!/bin/bash
# gitproxy.sh
# -S:socks, -H:http
# 使用socks来代理
connect -S 127.0.0.1:1080
```

为gitproxy.sh添加权限，拷贝到用户目录下被git调用
```
chmod +x gitproxy.sh
sudo cp gitproxy.sh /usr/bin
```
将gitproxy.sh关联到git配置中
```
git config --global core.gitproxy gitproxy.sh
```

取消git协议代理
```
git config --global --unset core.gitproxy
```

### 为ssh协议设置代理
代理ssh协议需要安装设置connect，windows下git安装目录下已安装connect

编辑~/.ssh/config，新增内容如下：
```
Host github.com
    User git
    HostName github.com
    Port 22
    # 使用socks来代理
    Proxycommand connect -S 127.0.0.1:1080 -a none %h %p
    PreferredAuthentications publickey
    IdentityFile "C:\Users\username\.ssh\id_rsa"
    TCPKeepAlive yes
```
或
```
Host github.com
    User git
    HostName ssh.github.com
    Port 443
    # Linux/Mac:
    # Proxycommand    /usr/bin/ncat --proxy 127.0.0.1:7890 --proxy-type socks5 %h %p
    # Windows:
    Proxycommand connect -S 127.0.0.1:1080 -a none %h %p
    PreferredAuthentications publickey
    IdentityFile "C:\Users\fangx\.ssh\id_rsa"
    TCPKeepAlive yes
```
> -a none 无需认证


