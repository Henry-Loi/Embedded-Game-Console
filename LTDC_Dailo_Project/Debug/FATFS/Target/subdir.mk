################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/Target/bsp_driver_sd.c \
../FATFS/Target/sd_diskio.c 

OBJS += \
./FATFS/Target/bsp_driver_sd.o \
./FATFS/Target/sd_diskio.o 

C_DEPS += \
./FATFS/Target/bsp_driver_sd.d \
./FATFS/Target/sd_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/Target/%.o FATFS/Target/%.su: ../FATFS/Target/%.c FATFS/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -IC:/Users/PC/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/PC/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/PC/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -IC:/Users/PC/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/PC/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FATFS-2f-Target

clean-FATFS-2f-Target:
	-$(RM) ./FATFS/Target/bsp_driver_sd.d ./FATFS/Target/bsp_driver_sd.o ./FATFS/Target/bsp_driver_sd.su ./FATFS/Target/sd_diskio.d ./FATFS/Target/sd_diskio.o ./FATFS/Target/sd_diskio.su

.PHONY: clean-FATFS-2f-Target

