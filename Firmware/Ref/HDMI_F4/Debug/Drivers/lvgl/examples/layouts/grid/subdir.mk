################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.c \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.c \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.c \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.c \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.c \
../Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.c 

OBJS += \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.o \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.o \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.o \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.o \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.o \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.o 

C_DEPS += \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.d \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.d \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.d \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.d \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.d \
./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/layouts/grid/%.o Drivers/lvgl/examples/layouts/grid/%.su Drivers/lvgl/examples/layouts/grid/%.cyclo: ../Drivers/lvgl/examples/layouts/grid/%.c Drivers/lvgl/examples/layouts/grid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/lvgl" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UI" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/App" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SSD1963" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/GT911" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LVGL_PORTING" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UART" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SST26VF064" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FSP" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/Communicate" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/FATFS" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LOG" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FATFS_DRV" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/User_Message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-layouts-2f-grid

clean-Drivers-2f-lvgl-2f-examples-2f-layouts-2f-grid:
	-$(RM) ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_1.su ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_2.su ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_3.su ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_4.su ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_5.su ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.cyclo ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.d ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.o ./Drivers/lvgl/examples/layouts/grid/lv_example_grid_6.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-layouts-2f-grid

