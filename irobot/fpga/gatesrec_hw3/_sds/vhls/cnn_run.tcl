open_project cnn
set_top cnn
add_files /home/student/sbm99/hldda_project/code/gatesrec_hw3/cnn.cpp -cflags "-Wall -O3 -MMD -MP -D __SDSCC__ -I /work/zhang/common/tools/xilinx/SDSoC/2015.2.1/arm-xilinx-linux-gnueabi/include -I/home/student/sbm99/hldda_project/code/gatesrec_hw3 -D __SDSVHLS__ -I /home/student/sbm99/hldda_project/code/gatesrec_hw3 -w"
open_solution "solution" -reset
set_part { xc7z020clg484-1 }
# synthesis directives
create_clock -period 8.5
config_rtl -reset_level low
source /home/student/sbm99/hldda_project/code/gatesrec_hw3/_sds/vhls/cnn.tcl
# end synthesis directives
csynth_design
export_design -acc
