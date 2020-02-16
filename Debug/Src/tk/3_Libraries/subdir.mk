################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tk/3_Libraries/tk0flash_base.c \
../Src/tk/3_Libraries/tk1flash2page_read.c \
../Src/tk/3_Libraries/tk1flash2page_write.c \
../Src/tk/3_Libraries/tk2_ModbusControl.c \
../Src/tk/3_Libraries/tkDebugThings.c \
../Src/tk/3_Libraries/tkErrLib.c \
../Src/tk/3_Libraries/tkSGL.c \
../Src/tk/3_Libraries/vkButton5ms.c \
../Src/tk/3_Libraries/vkButtonSmooth.c 

OBJS += \
./Src/tk/3_Libraries/tk0flash_base.o \
./Src/tk/3_Libraries/tk1flash2page_read.o \
./Src/tk/3_Libraries/tk1flash2page_write.o \
./Src/tk/3_Libraries/tk2_ModbusControl.o \
./Src/tk/3_Libraries/tkDebugThings.o \
./Src/tk/3_Libraries/tkErrLib.o \
./Src/tk/3_Libraries/tkSGL.o \
./Src/tk/3_Libraries/vkButton5ms.o \
./Src/tk/3_Libraries/vkButtonSmooth.o 

C_DEPS += \
./Src/tk/3_Libraries/tk0flash_base.d \
./Src/tk/3_Libraries/tk1flash2page_read.d \
./Src/tk/3_Libraries/tk1flash2page_write.d \
./Src/tk/3_Libraries/tk2_ModbusControl.d \
./Src/tk/3_Libraries/tkDebugThings.d \
./Src/tk/3_Libraries/tkErrLib.d \
./Src/tk/3_Libraries/tkSGL.d \
./Src/tk/3_Libraries/vkButton5ms.d \
./Src/tk/3_Libraries/vkButtonSmooth.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tk/3_Libraries/%.o: ../Src/tk/3_Libraries/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F105xC -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Inc" -I../Src/tk/3_Libraries/tkMenu -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/2_SubProjects" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/1_PROJECT" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/time64" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG/tkMenu" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkGrid" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkModbus" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


