################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.c \
../Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.c \
../Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.c \
../Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.c \
../Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.c 

OBJS += \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.o \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o 

C_DEPS += \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.d \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d \
./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/extra/libs/fsdrv/%.o Drivers/lvgl/src/extra/libs/fsdrv/%.su Drivers/lvgl/src/extra/libs/fsdrv/%.cyclo: ../Drivers/lvgl/src/extra/libs/fsdrv/%.c Drivers/lvgl/src/extra/libs/fsdrv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/lvgl" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UI" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/App" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SSD1963" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/GT911" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LVGL_PORTING" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UART" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SST26VF064" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FSP" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/Communicate" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/FATFS" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LOG" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FATFS_DRV" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/User_Message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv:
	-$(RM) ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.cyclo ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.su ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.cyclo ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.d ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.o ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.su ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.cyclo ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_posix.su ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.cyclo ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.su ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.cyclo ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o ./Drivers/lvgl/src/extra/libs/fsdrv/lv_fs_win32.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

