# Script to compile the FPGA all the way to bit file.
close_project -quiet
file delete -force results
file mkdir ./results

open_project proj.xpr

update_compile_order -fileset sources_1
reset_run synth_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1
launch_runs impl_1  -jobs 8 -to_step route_design
wait_on_run impl_1

open_run impl_1

set_property BITSTREAM.GENERAL.COMPRESS TRUE [get_designs impl_1]
write_bitstream -force ./proj.runs/impl_1/top.bit
write_hw_platform -include_bit -fixed -force -file ./results/top.xsa

report_timing_summary   -file   ./results/timing.rpt
report_utilization      -file   ./results/utilization.rpt
write_debug_probes      -force  ./results/top.ltx

close_project

exec bootgen -image bitstream.bif -arch zynqmp -o ./results/top.bit.bin -w

