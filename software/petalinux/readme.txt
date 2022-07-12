#petalinux-create --force --type project --template zynqMP --source ~/Downloads/xilinx/petalinux/xilinx-zcu104-v2022.1-04191534.bsp --name proj1
petalinux-create --force --type project --template zynqMP --name proj1

cd proj1

petalinux-config --get-hw-description ../../../fpga/implement/results/

        * Under "Image Packaging Configuration" -> "Root filesystem type" -> Select "SD Card"
        * Save and exit the configuration menu. Wait for configuration to complete.


petalinux-build -c bootloader -x distclean

petalinux-config --silentconfig -c kernel
    
petalinux-build

#petalinux-package --force --boot --u-boot --kernel --offset 0xF40000 --fsbl --pmufw --fpga ../../../fpga/implement/results/top.bit
petalinux-package --force --boot --u-boot --kernel --offset 0xF40000 --fpga ../../../fpga/implement/results/top.bit


    - Now copy the boot files to the SD card.

cp images/linux/BOOT.BIN /media/pedro/BOOT/
cp images/linux/image.ub /media/pedro/BOOT/
cp images/linux/boot.scr /media/pedro/BOOT/

    It is assumed that you already partitioned the SD card. 
    - sudo gparted  (make sure you have the correct drive selected!)
    - First partition called BOOT, FAT32, 512MB
    - Second partition called rootfs, ext4, use the rest of the card.

cd ..

get https://releases.linaro.org/debian/images/developer-arm64/latest/linaro-stretch-developer-20180416-89.tar.gz

sudo tar --preserve-permissions -zxvf linaro-stretch-developer-20180416-89.tar.gz

sudo cp --recursive --preserve binary/* /media/pedro/rootfs/ ; sync



    - Configure the PL side of the Zynq with an FPGA design. This has changed with this newer Linux on Zynq+.

    Modify your FPGA build script to produce a .bin file in addition to the normal .bit file. The FPGA example in this project has that command in compile.tcl.
    
    Go to your terminal on the Zynq+ Linux command line.

    Do a "git pull" to get the latest .bin file from the FPGA side of the repo.

    Copy .../fpga/implement/results/top.bit.bin to /lib/firmware. I think you need to do this as sudo.

    Change to root with "sudo su".

    echo top.bit.bin > /sys/class/fpga_manager/fpga0/firmware

    This last command should make the "Done" LED go green indicating success.

    - Good luck.

    
    Switch SW6 Configuration Option Settings
    Boot Mode   Mode Pins [3:0]     Mode SW6 [4:1]
    JTAG        0000/0x0            ON,ON,ON,ON
    QSPI32      0010/0x2            ON,ON,OFF,ON
    SD1         1110/0xE            OFF,OFF,OFF,ON
