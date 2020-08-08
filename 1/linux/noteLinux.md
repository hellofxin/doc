## 联网
---
两个电脑，一根网线
或者手机共享网络

## 添加源
sudo vi /etc/apt/sources.list
在插入模式下，添加源

## 安装无线网卡驱动
---
apt-get update
apt-get install linux-image-$(uname -r|sed 's,[^-]*-[^-]*-,,') linux-headers-$(uname -r|sed 's,[^-]*-[^-]*-,,') broadcom-sta-dkms
// modprobe -r b44 b43 b43legacy ssb brcmsmac bcma
modprobe wl

## 安装输入法
---
sudo dpkg-reconfigure locales
增加zh_cn, ok
apt-get install fcitx
apt-get install fcitx-sunpinyin
增加中文输入模式
fcitx-config-gtk3
