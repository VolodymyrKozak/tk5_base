################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tk/2_SubProjects/tk0_grid_sv.c \
../Src/tk/2_SubProjects/tk1_grid.c \
../Src/tk/2_SubProjects/vv300Flash.c \
../Src/tk/2_SubProjects/vv300set_factory.c \
../Src/tk/2_SubProjects/vv300set_user.c \
../Src/tk/2_SubProjects/wk_atime64.c \
../Src/tk/2_SubProjects/wk_distance_MBslave.c \
../Src/tk/2_SubProjects/wk_motor_commands.c \
../Src/tk/2_SubProjects/wk_motor_ctrl.c 

OBJS += \
./Src/tk/2_SubProjects/tk0_grid_sv.o \
./Src/tk/2_SubProjects/tk1_grid.o \
./Src/tk/2_SubProjects/vv300Flash.o \
./Src/tk/2_SubProjects/vv300set_factory.o \
./Src/tk/2_SubProjects/vv300set_user.o \
./Src/tk/2_SubProjects/wk_atime64.o \
./Src/tk/2_SubProjects/wk_distance_MBslave.o \
./Src/tk/2_SubProjects/wk_motor_commands.o \
./Src/tk/2_SubProjects/wk_motor_ctrl.o 

C_DEPS += \
./Src/tk/2_SubProjects/tk0_grid_sv.d \
./Src/tk/2_SubProjects/tk1_grid.d \
./Src/tk/2_SubProjects/vv300Flash.d \
./Src/tk/2_SubProjects/vv300set_factory.d \
./Src/tk/2_SubProjects/vv300set_user.d \
./Src/tk/2_SubProjects/wk_atime64.d \
./Src/tk/2_SubProjects/wk_distance_MBslave.d \
./Src/tk/2_SubProjects/wk_motor_commands.d \
./Src/tk/2_SubProjects/wk_motor_ctrl.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tk/2_SubProjects/%.o: ../Src/tk/2_SubProjects/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F105xC -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Inc" -I../Src/tk/3_Libraries/tkMenu -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/2_SubProjects" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/1_PROJECT" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/time64" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG/tkMenu" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkGrid" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkModbus" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


