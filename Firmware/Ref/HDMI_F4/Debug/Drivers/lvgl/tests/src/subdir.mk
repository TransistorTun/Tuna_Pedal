################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/tests/src/lv_test_indev.c \
../Drivers/lvgl/tests/src/lv_test_init.c 

OBJS += \
./Drivers/lvgl/tests/src/lv_test_indev.o \
./Drivers/lvgl/tests/src/lv_test_init.o 

C_DEPS += \
./Drivers/lvgl/tests/src/lv_test_indev.d \
./Drivers/lvgl/tests/src/lv_test_init.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/tests/src/%.o Drivers/lvgl/tests/src/%.su Drivers/lvgl/tests/src/%.cyclo: ../Drivers/lvgl/tests/src/%.c Drivers/lvgl/tests/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/lvgl" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UI" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/App" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SSD1963" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/GT911" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LVGL_PORTING" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UART" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SST26VF064" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FSP" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/Communicate" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/FATFS" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LOG" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FATFS_DRV" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/User_Message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-tests-2f-src

clean-Drivers-2f-lvgl-2f-tests-2f-src:
	-$(RM) ./Drivers/lvgl/tests/src/lv_test_indev.cyclo ./Drivers/lvgl/tests/src/lv_test_indev.d ./Drivers/lvgl/tests/src/lv_test_indev.o ./Drivers/lvgl/tests/src/lv_test_indev.su ./Drivers/lvgl/tests/src/lv_test_init.cyclo ./Drivers/lvgl/tests/src/lv_test_init.d ./Drivers/lvgl/tests/src/lv_test_init.o ./Drivers/lvgl/tests/src/lv_test_init.su

.PHONY: clean-Drivers-2f-lvgl-2f-tests-2f-src

