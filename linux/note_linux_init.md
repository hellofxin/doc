## update system
> ~# apt update
> ~# apt upgrade
> ~# apt install vim -y

## add user
> ~# useradd -m fxin -d /home/fxin -s /usr/bin/bash
> ~# passwd fxin

### longin with bash
> ~# usermod -s /usr/bin/bash fxin

### update sudoers
> ~# sudo vim /etc/sudoers
> 然后为当前用户增加权限
> ~# fxin ALL=(ALL:ALL) ALL
