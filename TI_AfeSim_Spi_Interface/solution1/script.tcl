############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project TI_AfeSim_Spi_Interface
set_top ti_spi_if
add_files TI_AfeSim_Spi_Interface/TI_AfeSim_Spi_Interface.c
add_files TI_AfeSim_Spi_Interface/TI_AfeSim_Spi_Interface2.c
open_solution "solution1"
set_part {xc7z020clg400-2} -tool vivado
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
#source "./TI_AfeSim_Spi_Interface/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -rtl verilog -format ip_catalog
