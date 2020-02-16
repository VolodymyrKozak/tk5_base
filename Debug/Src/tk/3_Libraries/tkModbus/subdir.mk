################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tk/3_Libraries/tkModbus/mb.c \
../Src/tk/3_Libraries/tkModbus/mb_forTK2_master.c \
../Src/tk/3_Libraries/tkModbus/mb_master.c \
../Src/tk/3_Libraries/tkModbus/mb_master5.c \
../Src/tk/3_Libraries/tkModbus/mb_slave.c \
../Src/tk/3_Libraries/tkModbus/mb_slave5.c \
../Src/tk/3_Libraries/tkModbus/mb_slave5a.c \
../Src/tk/3_Libraries/tkModbus/tkCRC.c 

OBJS += \
./Src/tk/3_Libraries/tkModbus/mb.o \
./Src/tk/3_Libraries/tkModbus/mb_forTK2_master.o \
./Src/tk/3_Libraries/tkModbus/mb_master.o \
./Src/tk/3_Libraries/tkModbus/mb_master5.o \
./Src/tk/3_Libraries/tkModbus/mb_slave.o \
./Src/tk/3_Libraries/tkModbus/mb_slave5.o \
./Src/tk/3_Libraries/tkModbus/mb_slave5a.o \
./Src/tk/3_Libraries/tkModbus/tkCRC.o 

C_DEPS += \
./Src/tk/3_Libraries/tkModbus/mb.d \
./Src/tk/3_Libraries/tkModbus/mb_forTK2_master.d \
./Src/tk/3_Libraries/tkModbus/mb_master.d \
./Src/tk/3_Libraries/tkModbus/mb_master5.d \
./Src/tk/3_Libraries/tkModbus/mb_slave.d \
./Src/tk/3_Libraries/tkModbus/mb_slave5.d \
./Src/tk/3_Libraries/tkModbus/mb_slave5a.d \
./Src/tk/3_Libraries/tkModbus/tkCRC.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tk/3_Libraries/tkModbus/%.o: ../Src/tk/3_Libraries/tkModbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F105xC -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Inc" -I../Src/tk/3_Libraries/tkMenu -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/2_SubProjects" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/1_PROJECT" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/time64" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG/tkMenu" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkGrid" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkModbus" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


