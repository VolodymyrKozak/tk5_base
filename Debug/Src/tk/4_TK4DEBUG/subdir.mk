################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tk/4_TK4DEBUG/d0_debug_main.c \
../Src/tk/4_TK4DEBUG/pr_OVEN_cmi.c \
../Src/tk/4_TK4DEBUG/tk_oven_smi2.c \
../Src/tk/4_TK4DEBUG/tk_wh1602b3.c \
../Src/tk/4_TK4DEBUG/tk_wo1602g.c \
../Src/tk/4_TK4DEBUG/zFF.c 

OBJS += \
./Src/tk/4_TK4DEBUG/d0_debug_main.o \
./Src/tk/4_TK4DEBUG/pr_OVEN_cmi.o \
./Src/tk/4_TK4DEBUG/tk_oven_smi2.o \
./Src/tk/4_TK4DEBUG/tk_wh1602b3.o \
./Src/tk/4_TK4DEBUG/tk_wo1602g.o \
./Src/tk/4_TK4DEBUG/zFF.o 

C_DEPS += \
./Src/tk/4_TK4DEBUG/d0_debug_main.d \
./Src/tk/4_TK4DEBUG/pr_OVEN_cmi.d \
./Src/tk/4_TK4DEBUG/tk_oven_smi2.d \
./Src/tk/4_TK4DEBUG/tk_wh1602b3.d \
./Src/tk/4_TK4DEBUG/tk_wo1602g.d \
./Src/tk/4_TK4DEBUG/zFF.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tk/4_TK4DEBUG/%.o: ../Src/tk/4_TK4DEBUG/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F105xC -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Inc" -I../Src/tk/3_Libraries/tkMenu -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/2_SubProjects" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/1_PROJECT" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/time64" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG/tkMenu" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkGrid" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkModbus" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


