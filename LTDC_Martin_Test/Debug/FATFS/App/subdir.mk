################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/App/fatfs.c 

OBJS += \
./FATFS/App/fatfs.o 

C_DEPS += \
./FATFS/App/fatfs.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/App/%.o FATFS/App/%.su: ../FATFS/App/%.c FATFS/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FATFS-2f-App

clean-FATFS-2f-App:
	-$(RM) ./FATFS/App/fatfs.d ./FATFS/App/fatfs.o ./FATFS/App/fatfs.su

.PHONY: clean-FATFS-2f-App
