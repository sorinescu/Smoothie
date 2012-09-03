PROJINCS += \
    src \
    src/platform/stm32/CMSIS/Device/ST/STM32F10x/Include \
    src/platform/stm32/CMSIS/Include \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/inc \
    src/platform/stm32/stm32f103/STM32F10x_StdPeriph_Driver/inc \
    src/platform/stm32/stm32f103/glue

LSCRIPT := src/platform/stm32/stm32f103/glue/gcc/stm32f103cb.ld

MCFLAGS := -mcpu=cortex-m3 -mthumb -mlittle-endian
MLDFLAGS := -mcpu=cortex-m3 -mthumb
MASFLAGS := -mcpu=cortex-m3 -mthumb

DEFINES += -DSTM32F10X_MD -D__USE_C99_MATH

CSRCS += \
    src/platform/stm32/stm32f103/STM32F10x_StdPeriph_Driver/src/misc.c \
    src/platform/stm32/stm32f103/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
    src/platform/stm32/stm32f103/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
    src/platform/stm32/stm32f103/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
    src/platform/stm32/stm32f103/glue/system_stm32f10x.c \
    src/platform/stm32/stm32f103/glue/stm32_it.c \
    src/platform/stm32/stm32f103/glue/hw_config.c \
    src/platform/stm32/syscalls.c

CPPSRCS += \
    src/platform/stm32/stm32f103/SlowTicker.cpp \
    src/platform/stm32/stm32f103/StepTicker.cpp \
    src/platform/stm32/stm32f103/Platform.cpp \
    src/platform/stm32/stm32f103/main.cpp

ifneq "$(SMOOTHIE_HAS_SERIAL_CONSOLE)" "0"

CSRCS += \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/otgd_fs_pcd.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_init.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_sil.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/otgd_fs_cal.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_regs.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/otgd_fs_dev.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_int.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/otgd_fs_int.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_mem.c \
    src/platform/stm32/stm32f103/STM32_USB-FS-Device_Driver/src/usb_core.c \
    src/platform/stm32/stm32f103/glue/usb_desc.c \
    src/platform/stm32/stm32f103/glue/usb_endp.c \
    src/platform/stm32/stm32f103/glue/usb_istr.c \
    src/platform/stm32/stm32f103/glue/usb_prop.c \
    src/platform/stm32/stm32f103/glue/usb_pwr.c

CPPSRCS += \
    src/platform/stm32/stm32f103/USBSerial.cpp

endif # SMOOTHIE_HAS_SERIAL_CONSOLE

ASRCS += \
    src/platform/stm32/stm32f103/glue/gcc/startup_stm32f10x_md.s
