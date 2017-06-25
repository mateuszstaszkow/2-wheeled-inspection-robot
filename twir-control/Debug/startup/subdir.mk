################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f10x_md.S 

OBJS += \
./startup/startup_stm32f10x_md.o 

S_UPPER_DEPS += \
./startup/startup_stm32f10x_md.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -IC:/stm32balancingrobot/Balancing/inc -IC:/stm32balancingrobot/Balancing/CMSIS/core -IC:/stm32balancingrobot/Balancing/CMSIS/device -IC:/stm32balancingrobot/Balancing/StdPeriph_Driver/inc -IC:/stm32balancingrobot/Balancing/Utilities/STM32F1xx-Nucleo -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


