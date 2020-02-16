################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tk/4_TK4DEBUG/tkMenu/tkDTset.c \
../Src/tk/4_TK4DEBUG/tkMenu/u16array_set.c \
../Src/tk/4_TK4DEBUG/tkMenu/u16set.c \
../Src/tk/4_TK4DEBUG/tkMenu/uList_set.c 

OBJS += \
./Src/tk/4_TK4DEBUG/tkMenu/tkDTset.o \
./Src/tk/4_TK4DEBUG/tkMenu/u16array_set.o \
./Src/tk/4_TK4DEBUG/tkMenu/u16set.o \
./Src/tk/4_TK4DEBUG/tkMenu/uList_set.o 

C_DEPS += \
./Src/tk/4_TK4DEBUG/tkMenu/tkDTset.d \
./Src/tk/4_TK4DEBUG/tkMenu/u16array_set.d \
./Src/tk/4_TK4DEBUG/tkMenu/u16set.d \
./Src/tk/4_TK4DEBUG/tkMenu/uList_set.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tk/4_TK4DEBUG/tkMenu/%.o: ../Src/tk/4_TK4DEBUG/tkMenu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F105xC -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Inc" -I../Src/tk/3_Libraries/tkMenu -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/2_SubProjects" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/1_PROJECT" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/time64" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/4_TK4DEBUG/tkMenu" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkGrid" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Src/tk/3_Libraries/tkModbus" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/KozakVF/workspase_TK5/tk5_base/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


