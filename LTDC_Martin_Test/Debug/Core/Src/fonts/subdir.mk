################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/fonts/font12.c \
../Core/Src/fonts/font12CN.c \
../Core/Src/fonts/font16.c \
../Core/Src/fonts/font20.c \
../Core/Src/fonts/font24.c \
../Core/Src/fonts/font24CN.c \
../Core/Src/fonts/font8.c 

OBJS += \
./Core/Src/fonts/font12.o \
./Core/Src/fonts/font12CN.o \
./Core/Src/fonts/font16.o \
./Core/Src/fonts/font20.o \
./Core/Src/fonts/font24.o \
./Core/Src/fonts/font24CN.o \
./Core/Src/fonts/font8.o 

C_DEPS += \
./Core/Src/fonts/font12.d \
./Core/Src/fonts/font12CN.d \
./Core/Src/fonts/font16.d \
./Core/Src/fonts/font20.d \
./Core/Src/fonts/font24.d \
./Core/Src/fonts/font24CN.d \
./Core/Src/fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/fonts/%.o Core/Src/fonts/%.su: ../Core/Src/fonts/%.c Core/Src/fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Middlewares/Third_Party/FatFs/src -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -IC:/Users/marti/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/marti/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/marti/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/marti/STM32Cube/Repository/STM32Cube_FW_F4_V1.24.2/Drivers/CMSIS/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/areion/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-fonts

clean-Core-2f-Src-2f-fonts:
	-$(RM) ./Core/Src/fonts/font12.d ./Core/Src/fonts/font12.o ./Core/Src/fonts/font12.su ./Core/Src/fonts/font12CN.d ./Core/Src/fonts/font12CN.o ./Core/Src/fonts/font12CN.su ./Core/Src/fonts/font16.d ./Core/Src/fonts/font16.o ./Core/Src/fonts/font16.su ./Core/Src/fonts/font20.d ./Core/Src/fonts/font20.o ./Core/Src/fonts/font20.su ./Core/Src/fonts/font24.d ./Core/Src/fonts/font24.o ./Core/Src/fonts/font24.su ./Core/Src/fonts/font24CN.d ./Core/Src/fonts/font24CN.o ./Core/Src/fonts/font24CN.su ./Core/Src/fonts/font8.d ./Core/Src/fonts/font8.o ./Core/Src/fonts/font8.su

.PHONY: clean-Core-2f-Src-2f-fonts

