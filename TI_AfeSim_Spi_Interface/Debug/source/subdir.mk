################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TI_AfeSim_Spi_Interface.c \
../TI_AfeSim_Spi_Interface2.c 

OBJS += \
./source/TI_AfeSim_Spi_Interface.o \
./source/TI_AfeSim_Spi_Interface2.o 

C_DEPS += \
./source/TI_AfeSim_Spi_Interface.d \
./source/TI_AfeSim_Spi_Interface2.d 


# Each subdirectory must supply rules for building sources it contributes
source/TI_AfeSim_Spi_Interface.o: C:/Users/Administrator/Desktop/CATL_Xilinx/TI_AfeIP/TI_AfeSim_Spi_Interface/TI_AfeSim_Spi_Interface.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DAESL_TB -D__llvm__ -D__llvm__ -IC:/Xilinx/Vivado/2018.3/include -IC:/Xilinx/Vivado/2018.3/win64/tools/auto_cc/include -IC:/Xilinx/Vivado/2018.3/include/ap_sysc -IC:/Users/Administrator/Desktop/CATL_Xilinx/TI_AfeIP -IC:/Xilinx/Vivado/2018.3/win64/tools/systemc/include -IC:/Xilinx/Vivado/2018.3/include/etc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/TI_AfeSim_Spi_Interface2.o: C:/Users/Administrator/Desktop/CATL_Xilinx/TI_AfeIP/TI_AfeSim_Spi_Interface/TI_AfeSim_Spi_Interface2.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DAESL_TB -D__llvm__ -D__llvm__ -IC:/Xilinx/Vivado/2018.3/include -IC:/Xilinx/Vivado/2018.3/win64/tools/auto_cc/include -IC:/Xilinx/Vivado/2018.3/include/ap_sysc -IC:/Users/Administrator/Desktop/CATL_Xilinx/TI_AfeIP -IC:/Xilinx/Vivado/2018.3/win64/tools/systemc/include -IC:/Xilinx/Vivado/2018.3/include/etc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


