## ssh 密钥登陆
- 有两台电脑，一台叫local，是ssh client，一台叫remote，是ssh server
- ***local的工作**
- local通过ssh-keygen生成公钥和私钥，通过ssh-copy-id将公钥上传remote
- 启动ssh-agent
- 通过ssh-add将私钥加入ssh agent
- 最后ssh username@host
- ***remote端工作**
- 配置sshd_config文件
- PublicKeysAuthentication yes
- Authorized_keysFile ~/.ssh/authorized_keys
- PasswordAuthentication no(这一项需要测试加或不加)
- 在etc/ssh/ 下生成rsa等服务器端的各种密钥
- 启动sshd服务

### 生成密钥
- ssh-keygen -t rsa -f ~/.ssh/id_rsa -C "username@host"
### 公钥上传服务器
- ssh-copy-id -i ~/.ssh/id_rsa username@host
- cat ~/.ssh/id_rsa.pub | (ssh username@host"cat >> ~/.ssh/authorized_keys")
- ssh username@host"cat >> ~/.ssh/authorized_keys" < ~/.ssh/id_rsa.pub
- ssh username@host tee- a ~/.ssh/authorized_keys < ~/.sshid_rsa.pub
### 启动ssh-agent
- ssh-agent bash (windows git-bash下的方式)
### 将私钥加入ssh agent
- ssh-add ~/.ssh/id_rsa
### 启动sshd
- /usr/bin/bash (windows git-bash下的方式)


## x11 forwarding
- 有两台电脑，一台叫local，一台叫remote
- 在local ssh 连接时 remote 时，local是ssh client， remote是ssh server
- 在x11 forwarding时，local是x11 server，remote是 x11 client
- 所以local是ssh client，也是x11 server；remote是ssh server，也是x11 client
- ***local端工作**
- 作为x11 server，local需要开启x11 server服务，如使用windows 下的mobaXterm，linux下x11 server已经启动了
- local ssh登陆remote，查看$DISPLAY变量，如果不对，需要export DISPLAY=local.ip:xx.xx
- 打开remote端gui程序，如xclock，local端会出现xclock的画面，则x11通信正常
- ***remote端工作**
- 配置sshd_config
- X11Forwarding yes
- X11DisplayOffset 10
