# execute this scipt with this command "xsdk -batch -source setup.tcl"

set hwspec ../../fpga/implement/results/top.hdf

# delete the old
file delete -force ./.metadata
file delete -force ./hw
file delete -force ./fsbl_bsp
file delete -force ./fsbl
file delete -force ./pmufw_bsp
file delete -force ./pmufw

# Create workspace and import the project into
setws .

createhw -name hw -hwspec $hwspec

# Create arm fsbl
createapp -name fsbl -app {Zynq MP FSBL} -proc psu_cortexa53_0 -hwproject hw -os standalone
configapp -app  fsbl define-compiler-symbols FSBL_DEBUG_INFO

# Create pmu firmware
createapp -name pmufw -app {ZynqMP PMU Firmware} -proc psu_pmu_0 -hwproject hw -os standalone

# Clean and build all projects
projects -clean
projects -build

puts "generating BOOT.bin from images"
exec bootgen -arch zynqmp -w on -log trace -image output.bif -w -o BOOT.bin

