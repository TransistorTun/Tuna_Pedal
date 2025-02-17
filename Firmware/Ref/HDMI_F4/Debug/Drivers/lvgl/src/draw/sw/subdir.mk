################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/draw/sw/lv_draw_sw.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_img.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_line.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.c \
../Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.c 

OBJS += \
./Drivers/lvgl/src/draw/sw/lv_draw_sw.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.o \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.o 

C_DEPS += \
./Drivers/lvgl/src/draw/sw/lv_draw_sw.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.d \
./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/draw/sw/%.o Drivers/lvgl/src/draw/sw/%.su Drivers/lvgl/src/draw/sw/%.cyclo: ../Drivers/lvgl/src/draw/sw/%.c Drivers/lvgl/src/draw/sw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/fonts" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/lvgl" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UI" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/App" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SSD1963" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/GT911" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LVGL_PORTING" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/UART" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/SST26VF064" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FSP" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/Communicate" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/FATFS" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/LOG" -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/FATFS_DRV" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/UserData/Onedrive/Project_D/My Project/DATN/Pedal/Firmware/Ref/HDMI_F4/Drivers/User_Message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw

clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw:
	-$(RM) ./Drivers/lvgl/src/draw/sw/lv_draw_sw.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_arc.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_blend.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_dither.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_gradient.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_img.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_layer.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_letter.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_line.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_polygon.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_rect.su ./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.cyclo ./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.d ./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.o ./Drivers/lvgl/src/draw/sw/lv_draw_sw_transform.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw

