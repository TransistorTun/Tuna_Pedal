################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_DEVICE/Core/Src/usb_device.c \
../USB_DEVICE/Core/Src/usbd_cdc.c \
../USB_DEVICE/Core/Src/usbd_cdc_if.c \
../USB_DEVICE/Core/Src/usbd_conf.c \
../USB_DEVICE/Core/Src/usbd_core.c \
../USB_DEVICE/Core/Src/usbd_ctlreq.c \
../USB_DEVICE/Core/Src/usbd_desc.c \
../USB_DEVICE/Core/Src/usbd_ioreq.c 

OBJS += \
./USB_DEVICE/Core/Src/usb_device.o \
./USB_DEVICE/Core/Src/usbd_cdc.o \
./USB_DEVICE/Core/Src/usbd_cdc_if.o \
./USB_DEVICE/Core/Src/usbd_conf.o \
./USB_DEVICE/Core/Src/usbd_core.o \
./USB_DEVICE/Core/Src/usbd_ctlreq.o \
./USB_DEVICE/Core/Src/usbd_desc.o \
./USB_DEVICE/Core/Src/usbd_ioreq.o 

C_DEPS += \
./USB_DEVICE/Core/Src/usb_device.d \
./USB_DEVICE/Core/Src/usbd_cdc.d \
./USB_DEVICE/Core/Src/usbd_cdc_if.d \
./USB_DEVICE/Core/Src/usbd_conf.d \
./USB_DEVICE/Core/Src/usbd_core.d \
./USB_DEVICE/Core/Src/usbd_ctlreq.d \
./USB_DEVICE/Core/Src/usbd_desc.d \
./USB_DEVICE/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
USB_DEVICE/Core/Src/%.o USB_DEVICE/Core/Src/%.su USB_DEVICE/Core/Src/%.cyclo: ../USB_DEVICE/Core/Src/%.c USB_DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/HDMI_F4/fonts" -I"D:/STM32CubeIDE/HDMI_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/STM32CubeIDE/HDMI_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src" -I"D:/STM32CubeIDE/HDMI_F4/USB_DEVICE/Core/Inc" -I"D:/STM32CubeIDE/HDMI_F4/USB_DEVICE/Core/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USB_DEVICE-2f-Core-2f-Src

clean-USB_DEVICE-2f-Core-2f-Src:
	-$(RM) ./USB_DEVICE/Core/Src/usb_device.cyclo ./USB_DEVICE/Core/Src/usb_device.d ./USB_DEVICE/Core/Src/usb_device.o ./USB_DEVICE/Core/Src/usb_device.su ./USB_DEVICE/Core/Src/usbd_cdc.cyclo ./USB_DEVICE/Core/Src/usbd_cdc.d ./USB_DEVICE/Core/Src/usbd_cdc.o ./USB_DEVICE/Core/Src/usbd_cdc.su ./USB_DEVICE/Core/Src/usbd_cdc_if.cyclo ./USB_DEVICE/Core/Src/usbd_cdc_if.d ./USB_DEVICE/Core/Src/usbd_cdc_if.o ./USB_DEVICE/Core/Src/usbd_cdc_if.su ./USB_DEVICE/Core/Src/usbd_conf.cyclo ./USB_DEVICE/Core/Src/usbd_conf.d ./USB_DEVICE/Core/Src/usbd_conf.o ./USB_DEVICE/Core/Src/usbd_conf.su ./USB_DEVICE/Core/Src/usbd_core.cyclo ./USB_DEVICE/Core/Src/usbd_core.d ./USB_DEVICE/Core/Src/usbd_core.o ./USB_DEVICE/Core/Src/usbd_core.su ./USB_DEVICE/Core/Src/usbd_ctlreq.cyclo ./USB_DEVICE/Core/Src/usbd_ctlreq.d ./USB_DEVICE/Core/Src/usbd_ctlreq.o ./USB_DEVICE/Core/Src/usbd_ctlreq.su ./USB_DEVICE/Core/Src/usbd_desc.cyclo ./USB_DEVICE/Core/Src/usbd_desc.d ./USB_DEVICE/Core/Src/usbd_desc.o ./USB_DEVICE/Core/Src/usbd_desc.su ./USB_DEVICE/Core/Src/usbd_ioreq.cyclo ./USB_DEVICE/Core/Src/usbd_ioreq.d ./USB_DEVICE/Core/Src/usbd_ioreq.o ./USB_DEVICE/Core/Src/usbd_ioreq.su

.PHONY: clean-USB_DEVICE-2f-Core-2f-Src

