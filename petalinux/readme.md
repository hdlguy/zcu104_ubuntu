# Petalinux (2024.2) on ZynqMP

## Petalinux Build instructions

    * open https://petalinux.xilinx.com/ in a web browser. this makes sure we have a good connection to the yocto downloads.

### Convert the vivado .xsa file to the system device tree files that Petalinux 2024.x wants.

/tools/Xilinx/Vitis/2024.2/bin/xsct ./gensdt.tcl

### Create Petalinux project

petalinux-create project --template zynqMP --name proj1
cd proj1

### fix bug in Petalinux 2024.2

vi project-spec/meta-user/conf/petalinuxbsp.conf

add these lines

MACHINE_FEATURES:append = " vcu"
SOC_VARIANT = "ev"

### configure project from hardware

petalinux-config --get-hw-description=../sdt/

    * Image Packaging Configuration -> Root Filesystem Type -> EXT4                         (if you want a persistent rootfs)
    * DTG Settings -> Kernel Bootargs -> manual bootargs -> earlycon console=ttyPS0,115200 root=/dev/mmcblk0p2 rw rootwait clk_ignore_unused
    * save and exit

### Build the bootloader

petalinux-build -c bootloader -x distclean

### Configure the kernel

petalinux-config -c kernel --silentconfig

### Build

petalinux-build

    * NOTE: frequently petalinux-build generates error messages. Just rerun the previous three commands to resolve that. (Who knows why?)

### Package 

petalinux-package --force --boot --fsbl --fpga --pmufw --u-boot

### Copy to SD Card

cp images/linux/BOOT.BIN /media/pedro/BOOT/; cp images/linux/image.ub /media/pedro/BOOT/; cp images/linux/boot.scr /media/pedro/BOOT/; sync

cd ..


## Installing a Debian root filesystem using debootstrap
Then follow instructions here to confgure the root file system: https://akhileshmoghe.github.io/_post/linux/debian_minimal_rootfs

Here are the most important commands listed for convenience. 

    sudo apt install qemu-user-static
    sudo apt install debootstrap

    sudo debootstrap --arch=arm64 --foreign buster debianMinimalRootFS
    sudo cp /usr/bin/qemu-aarch64-static ./debianMinimalRootFS/usr/bin/
    sudo cp /etc/resolv.conf ./debianMinimalRootFS/etc/resolv.conf
    sudo chroot ./debianMinimalRootFS
    export LANG=C

    /debootstrap/debootstrap --second-stage


Do some more file system configuration.

    apt update
    apt install locales dialog
    dpkg-reconfigure locales
    apt install vim openssh-server ntpdate sudo ifupdown net-tools udev iputils-ping wget dosfstools unzip binutils libatomic1
    passwd
    adduser myuser
    usermod -aG sudo myuser
    usermod --shell /bin/bash <user-name>

    Add to /etc/network/interfaces

    auto eth0
    iface eth0 inet dhcp

Exit chroot.

    exit

Write filesystem to SD card.

sudo cp --recursive --preserve ./debianMinimalRootFS/* /media/pedro/rootfs/; sync





# Commands to burn Petalinux into the flash prom (not used)

petalinux-package --boot --u-boot --fpga ../../implement/results/top.bit --format MCS

program_flash -f ./proj1/images/linux/boot.mcs -offset 0 -flash_type qspi-x4-single -fsbl ./proj1/images/linux/zynqmp_fsbl.elf -cable type xilinx_tcf url TCP:127.0.0.1:3121



# Commands to create and install a wic image (not used)

petalinux-package --wic

sudo dd if=images/linux/petalinux-sdimage.wic of=/dev/sdc conv=fsync


# FPGA boot mode dip switch

SW1 controls boot mode. Setting a switch to the "on" positions asserts a "0" on the mode line, ON = 0. OFF = 1;

Switch positions 1, 2, 3 and 4 correspond to mode lines 3, 2, 1, 0.

SD Card mode: switch 1 = on, switch 2 = off, switch 3 = on, switch 4 = off.

