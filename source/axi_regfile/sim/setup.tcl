# This script sets up a Vivado project with all ip references resolved.
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs
#
create_project -force proj 
set_property board_part em.avnet.com:ultrazed_eg_iocc_production:part0:1.0 [current_project]
set_property target_language Verilog [current_project]
set_property default_lib work [current_project]

#read_ip ../dgen_rom/dgen_rom.xci
#upgrade_ip -quiet  [get_ips *]
#generate_target {all} [get_ips *]

# Read in the hdl source.
read_verilog -sv ../axi_master_v1_0_M00_AXI.sv  
read_verilog -sv ../axi_regfile_tb.sv  
read_verilog -sv ../axi_regfile_v1_0_S00_AXI.sv

add_files -fileset sim_1 -norecurse ./axi_regfile_tb_behav.wcfg

close_project

#########################



