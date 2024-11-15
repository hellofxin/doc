## TFCard linux dd cmd burning image to eMMC
### ls dev
```sh
pi@pi$ ls /dev/mmcblk*boot0 | cut-cl-12
/dev/mmcblk1
```

### clear eMMC
```sh
pi@pi$ sudo dd bs=1M if=/dev/zero of=/dev/mmcblk1 count=1000 status=progress
pi@pi$ sudo sync
```

### burning
```sh
pi@pi$ sudo dd bs=1M if=~/pi5max.img of=/dev/mmcblk1 count=1000 status=progress
pi@pi$ sudo sync
```


## TFCard linux dd cmd burning image to ssd
### install bootloader on SPI Flash
```sh
pi@pi$ sudo nand-sata-install
```
or
```sh
pi@pi$ sudo dd if=/boot/rkspi_loader.img of=/dev/mtdblock0
```

### ls dev
```sh
pi@pi$ lspci
0004:40:00.0 PCI bridge: Fuzhou Rockchip Electronics Co., Ltd Device 3588 (rev 01)
0004:41:00.0 Non-Volatile memory controller: MAXIO Technology (Hangzhou) Ltd. NVMe SSD Controller MAP1202 (rev 01)

pi@pi$ sudo fdisk -l | grep "nvme0n1"
Disk /dev/nvme0n1: 1.86 TiB, 2048408248320 bytes, 4000797360 sectors
```

### clear SSD
```sh
pi@pi$ sudo dd bs=1M if=/dev/zero of=/dev/nvme0n1 count=2000 status=progress
pi@pi$ sync
```

## TFCard and SSD plugged
当 TF 卡和 NVMe SSD 中烧录了一模一样的系统，如果TF 卡和NVMe SSD都插在了开发板中，此时上电启动开发板，u-boot 会优先启动TF 卡中的系统。
但由于TF卡和NVMe SSD中的系统一模一样，所以两个存储设备中的/boot分区和rootfs 分区的 UUID 也一样，这样会导致 TF 卡启动的时候有可能会加载了NVMe SSD中的分区。
运行下面的脚本可以解决这个问题。
一模一样的系统是指镜像名一字不差的意思。就算都是Debian11 系统，版本不同也是不同的。
OPi OS Arch 系统中没有 fix_mmc_ssd.sh 这个脚本。
```sh
pi@pi$ sudo fix_mmc_ssd.sh
```