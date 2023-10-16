################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/ATK_RGBLCD/atk_rgblcd.c \
../Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.c \
../Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.c \
../Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.c \
../Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.c 

OBJS += \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd.o \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.o \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.o \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.o \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.o 

C_DEPS += \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd.d \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.d \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.d \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.d \
./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/ATK_RGBLCD/%.o Drivers/BSP/ATK_RGBLCD/%.su: ../Drivers/BSP/ATK_RGBLCD/%.c Drivers/BSP/ATK_RGBLCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-ATK_RGBLCD

clean-Drivers-2f-BSP-2f-ATK_RGBLCD:
	-$(RM) ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd.d ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd.o ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd.su ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.d ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.o ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_ltdc.su ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.d ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.o ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_ftxx.su ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.d ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.o ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_gtxx.su ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.d ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.o ./Drivers/BSP/ATK_RGBLCD/atk_rgblcd_touch_iic.su

.PHONY: clean-Drivers-2f-BSP-2f-ATK_RGBLCD

