################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/FATFS_SD.c \
../FATFS/diskio.c \
../FATFS/fatfs.c \
../FATFS/ff.c \
../FATFS/ff_gen_drv.c \
../FATFS/user_diskio.c 

OBJS += \
./FATFS/FATFS_SD.o \
./FATFS/diskio.o \
./FATFS/fatfs.o \
./FATFS/ff.o \
./FATFS/ff_gen_drv.o \
./FATFS/user_diskio.o 

C_DEPS += \
./FATFS/FATFS_SD.d \
./FATFS/diskio.d \
./FATFS/fatfs.d \
./FATFS/ff.d \
./FATFS/ff_gen_drv.d \
./FATFS/user_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/%.o FATFS/%.su FATFS/%.cyclo: ../FATFS/%.c FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/STM32CubeIDE/HDMI_F4/Drivers/lvgl" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/UI" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/App" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/SSD1963" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/GT911" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/LVGL_PORTING" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/UART" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/SST26VF064" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/FSP" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/Communicate" -I"D:/STM32CubeIDE/HDMI_F4/FATFS" -I"D:/STM32CubeIDE/HDMI_F4/Drivers/LOG" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FATFS

clean-FATFS:
	-$(RM) ./FATFS/FATFS_SD.cyclo ./FATFS/FATFS_SD.d ./FATFS/FATFS_SD.o ./FATFS/FATFS_SD.su ./FATFS/diskio.cyclo ./FATFS/diskio.d ./FATFS/diskio.o ./FATFS/diskio.su ./FATFS/fatfs.cyclo ./FATFS/fatfs.d ./FATFS/fatfs.o ./FATFS/fatfs.su ./FATFS/ff.cyclo ./FATFS/ff.d ./FATFS/ff.o ./FATFS/ff.su ./FATFS/ff_gen_drv.cyclo ./FATFS/ff_gen_drv.d ./FATFS/ff_gen_drv.o ./FATFS/ff_gen_drv.su ./FATFS/user_diskio.cyclo ./FATFS/user_diskio.d ./FATFS/user_diskio.o ./FATFS/user_diskio.su

.PHONY: clean-FATFS

