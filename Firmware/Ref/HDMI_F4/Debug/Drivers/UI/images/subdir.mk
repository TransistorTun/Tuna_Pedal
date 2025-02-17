################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UI/images/ui_img_back1_png.c \
../Drivers/UI/images/ui_img_back_png.c \
../Drivers/UI/images/ui_img_back_run_png.c \
../Drivers/UI/images/ui_img_backed_png.c \
../Drivers/UI/images/ui_img_infor_png.c \
../Drivers/UI/images/ui_img_infored_png.c \
../Drivers/UI/images/ui_img_left_png.c \
../Drivers/UI/images/ui_img_pause_png.c \
../Drivers/UI/images/ui_img_play_png.c \
../Drivers/UI/images/ui_img_power_png.c \
../Drivers/UI/images/ui_img_powered_png.c \
../Drivers/UI/images/ui_img_seting_png.c \
../Drivers/UI/images/ui_img_setinged_png.c 

OBJS += \
./Drivers/UI/images/ui_img_back1_png.o \
./Drivers/UI/images/ui_img_back_png.o \
./Drivers/UI/images/ui_img_back_run_png.o \
./Drivers/UI/images/ui_img_backed_png.o \
./Drivers/UI/images/ui_img_infor_png.o \
./Drivers/UI/images/ui_img_infored_png.o \
./Drivers/UI/images/ui_img_left_png.o \
./Drivers/UI/images/ui_img_pause_png.o \
./Drivers/UI/images/ui_img_play_png.o \
./Drivers/UI/images/ui_img_power_png.o \
./Drivers/UI/images/ui_img_powered_png.o \
./Drivers/UI/images/ui_img_seting_png.o \
./Drivers/UI/images/ui_img_setinged_png.o 

C_DEPS += \
./Drivers/UI/images/ui_img_back1_png.d \
./Drivers/UI/images/ui_img_back_png.d \
./Drivers/UI/images/ui_img_back_run_png.d \
./Drivers/UI/images/ui_img_backed_png.d \
./Drivers/UI/images/ui_img_infor_png.d \
./Drivers/UI/images/ui_img_infored_png.d \
./Drivers/UI/images/ui_img_left_png.d \
./Drivers/UI/images/ui_img_pause_png.d \
./Drivers/UI/images/ui_img_play_png.d \
./Drivers/UI/images/ui_img_power_png.d \
./Drivers/UI/images/ui_img_powered_png.d \
./Drivers/UI/images/ui_img_seting_png.d \
./Drivers/UI/images/ui_img_setinged_png.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UI/images/%.o Drivers/UI/images/%.su Drivers/UI/images/%.cyclo: ../Drivers/UI/images/%.c Drivers/UI/images/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/lvgl" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UI" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/App" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SSD1963" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/GT911" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LVGL_PORTING" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UART" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SST26VF064" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FSP" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/Communicate" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/FATFS" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LOG" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FATFS_DRV" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/User_Message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-UI-2f-images

clean-Drivers-2f-UI-2f-images:
	-$(RM) ./Drivers/UI/images/ui_img_back1_png.cyclo ./Drivers/UI/images/ui_img_back1_png.d ./Drivers/UI/images/ui_img_back1_png.o ./Drivers/UI/images/ui_img_back1_png.su ./Drivers/UI/images/ui_img_back_png.cyclo ./Drivers/UI/images/ui_img_back_png.d ./Drivers/UI/images/ui_img_back_png.o ./Drivers/UI/images/ui_img_back_png.su ./Drivers/UI/images/ui_img_back_run_png.cyclo ./Drivers/UI/images/ui_img_back_run_png.d ./Drivers/UI/images/ui_img_back_run_png.o ./Drivers/UI/images/ui_img_back_run_png.su ./Drivers/UI/images/ui_img_backed_png.cyclo ./Drivers/UI/images/ui_img_backed_png.d ./Drivers/UI/images/ui_img_backed_png.o ./Drivers/UI/images/ui_img_backed_png.su ./Drivers/UI/images/ui_img_infor_png.cyclo ./Drivers/UI/images/ui_img_infor_png.d ./Drivers/UI/images/ui_img_infor_png.o ./Drivers/UI/images/ui_img_infor_png.su ./Drivers/UI/images/ui_img_infored_png.cyclo ./Drivers/UI/images/ui_img_infored_png.d ./Drivers/UI/images/ui_img_infored_png.o ./Drivers/UI/images/ui_img_infored_png.su ./Drivers/UI/images/ui_img_left_png.cyclo ./Drivers/UI/images/ui_img_left_png.d ./Drivers/UI/images/ui_img_left_png.o ./Drivers/UI/images/ui_img_left_png.su ./Drivers/UI/images/ui_img_pause_png.cyclo ./Drivers/UI/images/ui_img_pause_png.d ./Drivers/UI/images/ui_img_pause_png.o ./Drivers/UI/images/ui_img_pause_png.su ./Drivers/UI/images/ui_img_play_png.cyclo ./Drivers/UI/images/ui_img_play_png.d ./Drivers/UI/images/ui_img_play_png.o ./Drivers/UI/images/ui_img_play_png.su ./Drivers/UI/images/ui_img_power_png.cyclo ./Drivers/UI/images/ui_img_power_png.d ./Drivers/UI/images/ui_img_power_png.o ./Drivers/UI/images/ui_img_power_png.su ./Drivers/UI/images/ui_img_powered_png.cyclo ./Drivers/UI/images/ui_img_powered_png.d ./Drivers/UI/images/ui_img_powered_png.o ./Drivers/UI/images/ui_img_powered_png.su ./Drivers/UI/images/ui_img_seting_png.cyclo ./Drivers/UI/images/ui_img_seting_png.d ./Drivers/UI/images/ui_img_seting_png.o ./Drivers/UI/images/ui_img_seting_png.su ./Drivers/UI/images/ui_img_setinged_png.cyclo ./Drivers/UI/images/ui_img_setinged_png.d ./Drivers/UI/images/ui_img_setinged_png.o ./Drivers/UI/images/ui_img_setinged_png.su

.PHONY: clean-Drivers-2f-UI-2f-images

