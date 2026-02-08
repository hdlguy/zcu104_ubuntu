# QSPI to SSD boot instructions.
This document describes the operations necessary to boot Linux from the QSPI memory but use the sata SSD as the non-volatile root filesystem.

This setup is very nice since sata SSD are designed for OS support with built in wear leveling and fault management.  Using an SD card for filesystem storage can be slow and fail spontaneously. Also, sata SSD are blazingly fast and available in capacities up to 2TB. 

The idea is to use the soldered down QSPI flash device to hold the Petalinux FSBL, U-Boot and the Linux kernel. Then a Linaro Ubuntu filesystem is copied to the sata SSD. 

This setup should be ideal for a platform that needs a non-volatile filesystem but runs continuously or needs to log or process big data at a high rate.

## Petalinux

The petalinux steps for this are more or less the same as for any other embedded linux application. There are just a couple of changes to the configuration settings.




- Create a new petalinux project.

    petalinux-create --force --type project --template zynqMP --source ~/Downloads/xilinx/zcu104/xilinx-zcu104-v2018.3-final-v2.bsp --name proj1

- Adjust the petalinux configuration.

    cd proj1

    petalinux-config --get-hw-description=../../../fpga/implement/results/

    - At the configuration menu make the following changes.    
        - Select Subsystem AUTO Hardware Settings.
            - Select Advanced Bootable Images Storage Settings.
                - Select **boot** image settings.
                    - Select Image Storage Media.
                    - Select boot device as primary flash.
                - Select **kernel** image settings.
                    - Select Image Storage Media.
                    - Select the storage device as primary flash.
        - Under "Image Packaging Configuration" -> 
            - "Root filesystem type" -> 
            - Select "Other"
        - Under "DTG Settings" -> 
            - "Kernel Bootargs" -> 
            - Un-select "generate boot args automatically" -> 
            - Enter "user set kernel bootargs" -> Paste in the following line. (This tells u-boot to start the kernel with the root filesystem on the SSD.)

                earlycon clk_ignore_unused earlyprintk root=/dev/sda2 rw rootfstype=ext4 rootwait cma=1024M

    - Save and exit the configuration menu. Wait for configuration to complete.

- Now edit a file to patch a bug in the Petalinux BSP for the zcu104. (Perhaps this is no longer necessary.)

    vim project-spec/meta-user/conf/petalinuxbsp.conf

    * Add the followint line

        IMAGE_INSTALL_remove = "gstreamer-vcu-examples"

- Now build the bootloader

    petalinux-build -c bootloader -x distclean

- Now run another configu menu.

    petalinux-config -c kernel
    
    You don't need to change anything. Just exit.

- Now build the linux kernel

    petalinux-build

    It takes a while to run.

- Now create the boot BOOT.BIN file that u-boot expects. 

    petalinux-package --boot --force --fsbl --u-boot --kernel --fpga ../../../fpga/implement/results/top.bit

    Now BOOT.BIN contains the FSBL, U-Boot, device tree and Linux kernel.

- Now burn the BOOT.BIN into the QSPI flash. 
    - Set mode switch SW6 [4:1] = on, on, on, on. This puts the board into JTAG boot mode. (Note: the board won't boot from QSPI in this mode.)
    - Open Vivado Hardware Manager and connect to the target.
    - Add the QSPI flash memory. (mt25qu512-qspi-x4-single)
    - Add .elf file for the FSBL and the BOOT.BIN as the programming file. (.../proj1/images/linux/zynqmp_fsbl.elf, .../proj1/images/linux/BOOT.BIN)
    - Select program device and check erase, program and verify.
    - Burn QSPI.
    - Set the jumpers for QSPI boot mode, SW6 [4:1]= on, on, off, on.

## Root Filesystem


- Now down load the root filesystem. It is 360MB.

    wget https://releases.linaro.org/debian/images/developer-arm64/latest/linaro-stretch-developer-20180416-89.tar.gz

- Uncompress the root filesystem **preserving file attributes and ownership**.

    sudo tar --preserve -zxvf linaro-stretch-developer-20180416-89.tar.gz

- Copy the root filesystem onto the SSD **preserving file attributes and ownership**.  This assumes the M.2 sata SSD is connected to the workstation using an M.2 to sata adapter and mounted at /media/xxxx/rootfs,

    sudo cp --recursive --preserve binary/* /media/xxxx/rootfs/

- Install the SSD in the ZCU104.

- Connect to the USB Uart port on the zcu104 and start a terminal emulator. I use screen sometimes.

    sudo screen /dev/ttyUSB1 115200

- Power on the board and watch the boot sequence. U-boot will time out and start linux. You should end up at the command prompt as root.

    If you connect an ethernet cable to your network you should be able to update the OS with

    apt update
    apt upgrade

- You can start installing things.

    apt install man
    apt install subversion

- It is a good idea to create a user for yourself and give it sudoer privileges.

    adduser -m -d /home/myuser myuser
    usermod -aG sudo myuser

- There is also something wrong with the permissions on some directories. You may need these commands to ssh into a new user account.

    chmod 755 /
    chmod 755 /bin
    chmod 755 /lib

- The serial  terminal is limiting so I like to ssh into the board. First, find the ip address of the zcu104.

    ifconfig (or "ip address")

    Then go back to your workstation.

    ssh myuser@<ip address> 

- Configure the PL side of the Zynq with an FPGA design. This has changed with this newer Linux on Zynq+.

    Modify your FPGA build script to produce a .bin file in addition to the normal .bit file. The FPGA example in this project has that command in compile.tcl.
    
    Go to your terminal on the Zynq+ Linux command line.

    Do a "git pull" to get the latest .bin file from the FPGA side of the repo.

    Copy .../fpga/implement/results/top.bit.bin to /lib/firmware. I think you need to do this as sudo.

    Change to root with "sudo su".

    echo top.bit.bin > /sys/class/fpga_manager/fpga0/firmware

    This last command should make the "Done" LED go green indicating success.

- Good luck.

    

