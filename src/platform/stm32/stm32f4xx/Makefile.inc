STM32_INCLUDES += \
    src/Libraries/STM32F4-Discovery \
	src/libs/stm32f4/CMSIS/ST/STM32F4xx/Include/ \
	src/libs/stm32f4/CMSIS/Include/ \
	src/libs/stm32f4/STM32F4xx_StdPeriph_Driver/inc/ \
	src/libs/stm32f4/STM32_USB_Device_Library/Core/inc/ \
	src/libs/stm32f4/STM32_USB_OTG_Driver/inc/ \
	src/libs/stm32f4/ext/ \
	src/libs/stm32f4/STM32_USB_Device_Library/Class/cdc/inc

SUBDIRS = $(wildcard src/* src/*/* src/*/*/* src/*/*/*/* src/*/*/*/*/*)
PROJINCS += $(sort $(dir $(SUBDIRS)))

LSCRIPT := src/platform/stm32/stm32f4xx/stm32f4.ld

MCFLAGS := -mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 
MLDFLAGS := -mcpu=cortex-m4 -mthumb
MASFLAGS := -mcpu=cortex-m4 -mthumb

DEFINES += -DUSE_USB_OTG_FS=1 -DUSE_STDPERIPH_DRIVER 

CSRCS += \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Core/src/usbd_req.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Core/src/usbd_ioreq.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/msc/src/usbd_storage_template.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/msc/src/usbd_msc_bot.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/msc/src/usbd_msc_core.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/msc/src/usbd_msc_data.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/msc/src/usbd_msc_scsi.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/dfu/src/usbd_otp_if.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/dfu/src/usbd_dfu_core.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/dfu/src/usbd_flash_if.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/dfu/src/usbd_dfu_mal.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/dfu/src/usbd_mem_if_template.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/hid/src/usbd_hid_core.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/audio/src/usbd_audio_out_if.c \
    src/platform/stm32/stm32f4xx/STM32_USB_Device_Library/Class/audio/src/usbd_audio_core.c \
    src/platform/stm32/stm32f4xx/STM32_USB_OTG_Driver/src/usb_dcd.c \
    src/platform/stm32/stm32f4xx/ext/usbd_req.c \
    src/platform/stm32/stm32f4xx/ext/usbd_core.c \
    src/platform/stm32/stm32f4xx/ext/usbd_cdc_core.c \
    src/platform/stm32/stm32f4xx/ext/usb_bsp.c \
    src/platform/stm32/stm32f4xx/ext/usbd_usr.c \
    src/platform/stm32/stm32f4xx/ext/usbd_ioreq.c \
    src/platform/stm32/stm32f4xx/ext/usbd_cdc_vcp.c \
    src/platform/stm32/stm32f4xx/ext/usb_core.c \
    src/platform/stm32/stm32f4xx/ext/usbd_desc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_tdes.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_can.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/misc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_des.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_md5.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_iwdg.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_aes.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dbgmcu.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_crc.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_sha1.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c \
    src/platform/stm32/stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_wwdg.c \
    src/platform/stm32/stm32f4xx/glue/stm32f4_discovery.c

CPPSRCS += \
    src/platform/stm32/stm32f4xx/ext/serial.cpp \
    src/platform/stm32/stm32f4xx/ext/usb_dcd_int.cpp \
    src/platform/stm32/stm32f4xx/SlowTicker.cpp \
    src/platform/stm32/stm32f4xx/StepTicker.cpp \
    src/platform/stm32/stm32f4xx/main.cpp \
    src/platform/stm32/stm32f4xx/glue/stm32fxxx_it.cpp \
    src/modules/utils/simpleshell/SimpleShell.cpp \
    src/modules/communication/SerialConsole.cpp \
    src/modules/tools/switch/Switch.cpp \
    src/modules/tools/switch/SwitchPool.cpp \
    src/modules/tools/laser/Laser.cpp \
    src/modules/tools/extruder/Extruder.cpp \
    src/modules/tools/temperaturecontrol/TemperatureControl.cpp \
    src/modules/tools/temperaturecontrol/TemperatureControlPool.cpp

