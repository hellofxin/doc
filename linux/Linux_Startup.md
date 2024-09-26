## edit /etc/apt/sources.list
```
cat <<EOF >/etc/apt/sources.list
# https://mirror.tuna.tsinghua.edu.cn/help/debian/
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm main contrib non-free non-free-firmware
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm main contrib non-free non-free-firmware

deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-updates main contrib non-free non-free-firmware
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-updates main contrib non-free non-free-firmware

deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-backports main contrib non-free non-free-firmware
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-backports main contrib non-free non-free-firmware

deb https://mirrors.tuna.tsinghua.edu.cn/debian-security bookworm-security main contrib non-free non-free-firmware
# # deb-src https://mirrors.tuna.tsinghua.edu.cn/debian-security bookworm-security main contrib non-free non-free-firmware

# deb https://security.debian.org/debian-security bookworm-security main contrib non-free non-free-firmware
# deb-src https://security.debian.org/debian-security bookworm-security main contrib non-free non-free-firmware

EOF

apt-get update
```


## grant root privilage
```method 1
su -
sudo vim /etc/sudoers

# insert one row
fxin    ALL=(ALL:ALL)  ALL
```

```method 2
su -
usermod -aG sudo fxin
newgrp sudo
su - fxin
```

## install some tools
sudo apt install -y vim fcitx fcitx-sunpinyin


## install WIFI Driver
```
lspci -k

sudo apt-get install linux-headers-generic build-essential dkms broadcom-sta-modules

# download bcm.deb, then install
sudo dpkg -i wireless-bcm43142-dkms_6.20.55.19-1_amd64.deb

# probe wifi
sudo modprobe wl
```

## install some tools
sudo apt install -y git build-essential cmake


## optional config
```
sudo ln -s /usr/bin/python3 /usr/bin/python
```


## install vscode

Visual Studio Code (VSCode) is a powerful and lightweight source code editor developed by Microsoft, designed to support a wide range of programming languages and frameworks. It offers features such as debugging, embedded Git control, syntax highlighting, intelligent code completion, and customizable themes, making it a favorite among developers. VSCode is available in two main versions: the stable build, which is recommended for most users, and the Insiders build, which provides early access to the latest features and updates.

On Debian 12 or 11, you can install VSCode using Microsoft’s official APT repository. This method provides easy access to both the latest stable version and the Insiders build, ensuring that your development environment is always up-to-date with the newest features. This guide will walk you through the process of setting up the Microsoft APT repository and installing VSCode on your Debian system.

### Pre-installation Steps for VSCode

Update Debian Before Visual Studio Code Installation

Before diving into the installation process, ensuring your Debian system is updated with the latest packages is essential. This will help prevent any potential conflicts during the installation of Visual Studio Code.

To update your system, open a terminal and run the following command:
```
sudo apt update
```
After updating the package list, proceed to upgrade any outdated packages with the following command:
```
sudo apt upgrade
```
### Install Required Packages for VSCode

Next, you’ll need to install a few required packages to ensure a smooth installation of Visual Studio Code. These packages are standard across most Linux distributions and help manage repositories and securely download files.

Run the following command to install the necessary packages:
```
sudo apt install dirmngr ca-certificates software-properties-common apt-transport-https curl -y
```
Following these pre-install steps, you’ve prepared your Debian system to install Visual Studio Code. In the following steps, you’ll learn how to add the official Microsoft repository and install the latest version of Visual Studio Code.

### Import Visual Studio Code APT Repository

In this step, you’ll learn to import the official Microsoft repository for Visual Studio Code to ensure you get the latest version from a trusted source.

#### Import Microsoft GPG Key

First, you need to import the Microsoft GPG key, which allows your system to verify the authenticity of the Visual Studio Code installation package. To do this, run the following command in your terminal:
```
curl -fSsL https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor | sudo tee /usr/share/keyrings/vscode.gpg >/dev/null
```
This command downloads the GPG key from Microsoft’s server and adds it to your system’s keyring.

#### Add Microsoft VSCode APT Repository

Now that you’ve imported the GPG key, the next step is to add the Visual Studio Code repository to your system. This lets you install and update Visual Studio Code directly from Microsoft’s official repository.

Run the following command in your terminal to add the repository:
```
echo deb [arch=amd64 signed-by=/usr/share/keyrings/vscode.gpg] https://packages.microsoft.com/repos/vscode stable main | sudo tee /etc/apt/sources.list.d/vscode.list
```
This command adds the repository to your system’s list of package sources, making it accessible when searching for packages to install or update.

With the repository imported, you’re now ready to install Visual Studio Code on your Debian system.

### Finalize Installation of VSCode

Before you begin the installation process, it’s important to understand the difference between Visual Studio Code and Visual Studio Code Insiders. Each version offers distinct advantages, catering to various user needs.

Visual Studio Code is the stable release of the editor, designed to provide a dependable coding experience. This version undergoes extensive testing, making it ideal for developers who prioritize stability and reliability.

On the other hand, Visual Studio Code Insiders is a beta or preview version that provides early access to upcoming features. This version is continuously updated with new innovations, although it may not be as thoroughly tested as the stable release, which means there is a higher potential for encountering bugs. It is often preferred by developers who enjoy experimenting with new features or want to stay ahead of the curve.

Both versions can be installed on the same system, allowing developers to switch between the stable and beta environments as needed.

#### Update APT Cache Index After VSCode APT Repo Import

Before proceeding with the installation of Visual Studio Code, it’s a best practice to ensure your system’s package list is current, especially after adding a new repository:
```
sudo apt update
```
#### Select 1 or Both Versions of VSCode to Install:

Option 1: Install Visual Studio Code – stable build

For those who prioritize a stable and reliable coding environment, the following command will install the standard version of Visual Studio Code:
```
sudo apt install code
```
Option 2: Install Visual Studio Code – insiders build

If you want to explore the latest features, the Insiders version is for you, and don’t mind the occasional bug. To install, use the command:
```
sudo apt install code-insiders
```
Note: It’s worth reiterating that Visual Studio Code’s standard and insider versions are designed to operate concurrently on your Debian system. This dual-installation feature ensures developers can effortlessly transition between a stable environment and a beta testing ground, depending on their project requirements.

### Launch Visual Studio Code

CLI Commands to Launch VSCode

For those who enjoy using the terminal, you can launch Visual Studio Code quickly by entering the following command:

The stable version:
```
code
```
Those that want to install the insider (beta) version:
```
code-insiders
```


### Additional Commands for VSCode

#### Remove Visual Studio Code

If you remove Visual Studio Code from your Debian system, follow the steps below to uninstall the software, remove the repository, and delete the GPG key.

To uninstall Visual Studio Code or Visual Studio Code Insiders, run the corresponding command in your terminal:
```
sudo apt remove code

sudo apt remove code-insiders
```
After uninstalling the software, remove the Microsoft Visual Studio Code repository from your system with the following command:
```
sudo rm /etc/apt/sources.list.d/vscode.list
```
Removing the GPG key associated with the Visual Studio Code repository is a good practice for enhanced security and proper system maintenance. To do this, run the following command:
```
sudo rm /usr/share/keyrings/vscode.gpg
```
Following these steps, you’ll have successfully removed Visual Studio Code, its repository, and the GPG key from your Debian system.
