################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/SDRAM/sdram.c 

OBJS += \
./Drivers/BSP/SDRAM/sdram.o 

C_DEPS += \
./Drivers/BSP/SDRAM/sdram.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/SDRAM/%.o Drivers/BSP/SDRAM/%.su: ../Drivers/BSP/SDRAM/%.c Drivers/BSP/SDRAM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-SDRAM

clean-Drivers-2f-BSP-2f-SDRAM:
	-$(RM) ./Drivers/BSP/SDRAM/sdram.d ./Drivers/BSP/SDRAM/sdram.o ./Drivers/BSP/SDRAM/sdram.su

.PHONY: clean-Drivers-2f-BSP-2f-SDRAM

