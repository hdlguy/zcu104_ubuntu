# Notes on FPGA Compilation
These instructions assume you are working from a supported Linux distribution.  We use Ubutu 16.04.
## Install Vivado
- Download the latest Vivado Xilinx_Vivado_SDK_2018.2_0614_1954.tar.gz (17GB)
- tar -zxvf Xilinx_Vivado_SDK_2018.2_0614_1954.tar.gz
- cd Xilinx_Vivado_SDK_2018.1_0405_1
- sudo ./xsetup

Set the environment variables. I put these in a shell script for convenience.
- source /opt/Xilinx/Vivado/2018.1/settings64.sh
- source /opt/Xilinx/SDK/2018.1/settings64.sh
## Setup the Project
- cd .../fpga/implement
- vivado -mode batch -source setup.tcl
- That last step creates a Vivado project ready to compile, "proj.xpr"
## Compile the FPGA
- vivado -mode batch -source compile.tcl
- That last step runs Vivado in "project flow" to write the needed output files and reports.
- ls -lh ./results
- The .hdf hardware definition file and .bit bitstream file are needed to build the software.
