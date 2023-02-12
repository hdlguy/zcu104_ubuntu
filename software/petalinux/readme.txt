# ZCU104 Ubuntu Boot instructions

## Linux Boot Image    

    * open https://petalinux.xilinx.com/ in a web browser. this makes sure we have a good connection to the yocto downloads.

petalinux-create --force --type project --template zynqMP --source ~/Downloads/xilinx/zcu104/xilinx-zcu104-v2022.2-10141622.bsp --name proj1

cd proj1

petalinux-config --get-hw-description ../../../fpga/implement/results/

    * Yocto Settings -> Add pre-mirror url -> change http: to https; change rel-v${PETALINUX_MAJOR_VER} to rel-v2022.2
    * Yocto Settings -> Network State Feeds url -> change http: to https; change rel-v${PETALINUX_MAJOR_VER} to rel-v2022.2
    * Image Packaging Configuration -> EXT4 (if you want the rootfs on the sd card)
    * save and exit

petalinux-build -c bootloader -x distclean

petalinux-config --silentconfig -c kernel
    
petalinux-build

petalinux-package --force --boot --u-boot --kernel --offset 0xF40000 --fpga ../../../fpga/implement/results/top.bit


    - Now copy the boot files to the SD card.

cp images/linux/BOOT.BIN /media/pedro/BOOT/
cp images/linux/image.ub /media/pedro/BOOT/
cp images/linux/boot.scr /media/pedro/BOOT/

    It is assumed that you already partitioned the SD card. 
    - sudo gparted  (make sure you have the correct drive selected!)
    - First partition called BOOT, FAT32, 512MB
    - Second partition called rootfs, ext4, use the rest of the card.

## Root Filesystem

cd ..

wget https://releases.linaro.org/debian/images/developer-arm64/latest/linaro-stretch-developer-20180416-89.tar.gz

sudo tar --preserve-permissions -zxvf linaro-stretch-developer-20180416-89.tar.gz

sudo cp --recursive --preserve binary/* /media/pedro/rootfs/ ; sync


## Run-time FPGA Configuration

- Configure the PL side of the Zynq with an FPGA design. This has changed with this newer Linux on Zynq+.

Modify your FPGA build script to produce a .bin file in addition to the normal .bit file. The FPGA example in this project has that command in compile.tcl.
    
Go to your terminal on the Zynq+ Linux command line.

Do a "git pull" to get the latest .bin file from the FPGA side of the repo.

cp .../fpga/implement/results/top.bit.bin to /lib/firmware

Change to root with "sudo su".

echo top.bit.bin > /sys/class/fpga_manager/fpga0/firmware

This last command should make the "Done" LED go green indicating success.


## Configuration Switch
    
    Switch SW6 Configuration Option Settings
    Boot Mode   Mode Pins [3:0]     Mode SW6 [4:1]
    JTAG        0000/0x0            ON,ON,ON,ON
    QSPI32      0010/0x2            ON,ON,OFF,ON
    SD1         1110/0xE            OFF,OFF,OFF,ON

## Booted Machine

apt install man
apt install subversion

adduser myuser
usermod -aG sudo myuser

