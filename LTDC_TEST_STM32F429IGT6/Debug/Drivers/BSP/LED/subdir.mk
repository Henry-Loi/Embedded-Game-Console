################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/LED/led.c 

OBJS += \
./Drivers/BSP/LED/led.o 

C_DEPS += \
./Drivers/BSP/LED/led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/LED/%.o Drivers/BSP/LED/%.su: ../Drivers/BSP/LED/%.c Drivers/BSP/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-LED

clean-Drivers-2f-BSP-2f-LED:
	-$(RM) ./Drivers/BSP/LED/led.d ./Drivers/BSP/LED/led.o ./Drivers/BSP/LED/led.su

.PHONY: clean-Drivers-2f-BSP-2f-LED

