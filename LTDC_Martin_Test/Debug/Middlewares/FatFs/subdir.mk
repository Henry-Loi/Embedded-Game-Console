################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/option/cc936.c \
C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/diskio.c \
C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/ff.c \
C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/option/syscall.c 

OBJS += \
./Middlewares/FatFs/cc936.o \
./Middlewares/FatFs/diskio.o \
./Middlewares/FatFs/ff.o \
./Middlewares/FatFs/ff_gen_drv.o \
./Middlewares/FatFs/syscall.o 

C_DEPS += \
./Middlewares/FatFs/cc936.d \
./Middlewares/FatFs/diskio.d \
./Middlewares/FatFs/ff.d \
./Middlewares/FatFs/ff_gen_drv.d \
./Middlewares/FatFs/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FatFs/cc936.o: C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/option/cc936.c Middlewares/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/diskio.o: C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/diskio.c Middlewares/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/ff.o: C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/ff.c Middlewares/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/ff_gen_drv.o: C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c Middlewares/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/syscall.o: C:/Users/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src/option/syscall.c Middlewares/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-FatFs

clean-Middlewares-2f-FatFs:
	-$(RM) ./Middlewares/FatFs/cc936.d ./Middlewares/FatFs/cc936.o ./Middlewares/FatFs/cc936.su ./Middlewares/FatFs/diskio.d ./Middlewares/FatFs/diskio.o ./Middlewares/FatFs/diskio.su ./Middlewares/FatFs/ff.d ./Middlewares/FatFs/ff.o ./Middlewares/FatFs/ff.su ./Middlewares/FatFs/ff_gen_drv.d ./Middlewares/FatFs/ff_gen_drv.o ./Middlewares/FatFs/ff_gen_drv.su ./Middlewares/FatFs/syscall.d ./Middlewares/FatFs/syscall.o ./Middlewares/FatFs/syscall.su

.PHONY: clean-Middlewares-2f-FatFs

