# Notes on Software compilation.
These instructions attempt to compile everythng from primary sources. Xilinx calls this the OSL flow from here: https://github.com/hdlguy/zcu104_ubuntu.git

Unfortunately, I have been unable to come up with a set of instructiions which result in booting linux with a root filesystem on non-volatile storage supporting full Ubuntu desktop linux.  Instead, I was able to follow the petalinux flow and boot into Ubuntu on the SD card.  Please see the instructions at ./petalinux/readme.md.
## Setup Compiler Toolchain
- install Xilinx SDK 2018.2
- source /opt/Xilinx/SDK/2018.2/settings64.sh
- export CROSS_COMPILE=aarch64-linux-gnu-
## Build bl31.elf - Arm Trusted Firmware
ATF is something new with the 64 bit Arm processors. It has something to do with secure access to hardware features of the chip but for most users it is a black box.  ATF must be compiled and included in the ZynqMP boot image.  We found that the DEBUG option must be set when running make in order to get a working elf. 
- cd \<git clone location\>/zcu104_ubuntu/software
- git clone https://github.com/Xilinx/arm-trusted-firmware.git
- cd arm-trusted-firmware
- git checkout xilinx-v2018.2
- make ARCH=aarch64 DEBUG=1 RESET_TO_BL31=1 PLAT=zynqmp bl31
- Output image is ../arm-trusted-firmware/build/zynqmp/debug/bl31/bl31.elf
## Build Das U-Boot
- cd \<git clone location\>/zcu104_ubuntu/software
- git clone https://github.com/Xilinx/u-boot-xlnx.git
- cd u-boot-xlnx/
- git checkout xilinx-v2018.2
- make  xilinx_zynqmp_zcu104_revA_defconfig
- make
- "u-boot.elf" is the resulting elf image.
## Create BOOT.bin
BOOT.bin is a collection of compiled software moudules and an FPGA .bit file. It is produced automatically here by a TCL script that runs in XSDK.
- cd \<git clone location\>/zcu104_ubuntu/software/sdk
- xsdk -batch -source setup.tcl
- BOOT.bin is the output file.
## Format MicroSD card
- I use a SanDisk Ultra PLUS 32GB MicrSD card.
- Use USB adapter to connect it to your Linux workstation.
- sudo gparted
- Carefully select the SD card as the device.  It should be obvious by size.
- Delete any existing partitions. (!Make sure you are operating on the SD card!)
- Create FAT32 partion called BOOT, 64MB, starting at the default low address.
- Create ext4 partition called rootfs, using the rest of the SD card.
## Build boot.bin
- cd sdk
- xsdk -batch -source setup.tcl
- The above step automates everything you need to do in XSDK, starting from the .hdf file.
- BOOT.bin is the first stage boot loader (FSBL) combined with u-boot into a single boot image.
- cp BOOT.bin /\<path to boot partition on microSD card\>
## Build the Linux Kernel
- cd \<git clone location\>/zcu104_ubuntu/software
- git clone https://github.com/Xilinx/linux-xlnx.git
- cd linux-xlnx/
- git checkout xilinx-v2018.2
- make ARCH=arm64 xilinx_zynqmp_defconfig
- make ARCH=arm64 menuconfig
- make ARCH=arm64 -j 8
- ls -lh arch/arm64/boot/Image
- cp arch/arm64/boot/Image /media/pedro/BOOT/
## Build the Device Tree Blob (Optional)
You can skip this step because the u-boot compilation produces a generic dtb file for your target when it compiles. That one seems to work fine.

The dtb is another piece that is generated from the .hdf file.  Because I reconfigure the PL frequenly I don't like to put any of the PL logic into the device tree.  I create a ZynqMP project without anything else and export the hdf from there.
Xilinx provides yet another github repository of stuff to build your device tree source (.dts) file. Here we go.
- cd \<git clone location\>/zcu104_ubuntu/software
- git clone https://github.com/Xilinx/device-tree-xlnx
- cd device-tree-xlnx
- git checkout xilinx-v2018.2

Now we have to run yet another Xilinx command line tool called HSI and type a few commands to get the dts from our hdf. Make sure you still have the compiler tool chain setup from above.
- hsi

Now we are in HSI so run these commands.
- open_hw_design ../../fpga/implement/results/top.hdf
- set_repo_path .
- create_sw_design device-tree -os device_tree -proc psu_cortexa53_0
- generate_target -dir ../dts_files
- exit

There should be dts files in the folder specified above.
- cd ../dts_files
- dtc -I dts -O dtb -o system-top.dtb system-top.dts
- cp system-top.dtb /media/pedro/BOOT/system.dtb

If the dtc command above is not found you need to install the device-tree-compiler package, "sudo apt install device-tree-compiler" on Ubuntu.
## Get Ubuntu root filesystem
- We'll try to run Ubuntu 16.04 LTS
- wget https://releases.linaro.org/debian/images/developer-arm64/16.04/linaro-jessie-developer-20160428-75.tar.gz
- sudo tar -zxvf linaro-jessie-developer-20160428-75.tar.gz
- sudo cp -r binary/* /\<path to rootfs partition on microSD card\>

## How to load bitfile into PL from Linux on ZynqMP
The fpga build script automatically generates top.bit.bin that is commited to the repository.
1. sudo -i
1. cd ../fpga/implement/results
1. cp top.bit.bin  /lib/firmware
1. echo top.bit.bin > /sys/class/fpga_manager/fpga0/firmware


