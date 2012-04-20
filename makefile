PROJECT = Smoothie
# Default project source to be located in current directory.
ifndef SRC
SRC=.
endif

# Default the init of stdio/stdout/stderr to occur before global constructors.
ifndef GCC4MBED_DELAYED_STDIO_INIT
GCC4MBED_DELAYED_STDIO_INIT=0
endif

# List of sources to be compiled/assembled
CSRCS = $(wildcard $(SRC)/*.c $(SRC)/*/*.c $(SRC)/*/*/*.c $(SRC)/*/*/*/*.c $(SRC)/*/*/*/*/*.c $(SRC)/*/*/*/*/*/*.c)
ASRCS =  $(wildcard $(SRC)/*.S $(SRC)/*/*.S $(SRC)/*/*/*.S $(SRC)/*/*/*/*.S $(SRC)/*/*/*/*/*.S)
CPPSRCS = $(wildcard $(SRC)/*.cpp $(SRC)/*/*.cpp $(SRC)/*/*/*.cpp $(SRC)/*/*/*/*.cpp $(SRC)/*/*/*/*/*.cpp)

# List of the objects files to be compiled/assembled
OBJECTS= $(CSRCS:.c=.o) $(ASRCS:.S=.o) $(CPPSRCS:.cpp=.o)
LSCRIPT=./src/stm32f4.ld

# Location of external library and header dependencies.
#EXTERNAL_DIR = $(GCC4MBED_DIR)/external

# Include path which points to external library headers and to subdirectories of this project which contain headers.
SUBDIRS = $(wildcard $(SRC)/* $(SRC)/*/* $(SRC)/*/*/* $(SRC)/*/*/*/* $(SRC)/*/*/*/*/*)
PROJINCS = $(sort $(dir $(SUBDIRS)))
#INCDIRS += $(PROJINCS) $(EXTERNAL_DIR)/mbed $(EXTERNAL_DIR)/mbed/LPC1768 $(EXTERNAL_DIR)/FATFileSystem $(GCC4MBED_DIR)/mri
INCDIRS += $(PROJINCS) 

STM32_INCLUDES = ./Libraries/STM32F4-Discovery \
	./src/libs/stm32f4/CMSIS/ST/STM32F4xx/Include/ \
	./src/libs/stm32f4/CMSIS/Include/ \
	./src/libs/stm32f4/STM32F4xx_StdPeriph_Driver/inc/ \
	./src/libs/stm32f4/STM32_USB_Device_Library/Class/hid/inc \
	./src/libs/stm32f4/STM32_USB_Device_Library/Core/inc/ \
	./src/libs/stm32f4/STM32_USB_OTG_Driver/inc/ \
	./src/libs/stm32f4/ext

INCDIRS += $(STM32_INCLUDES)	

# DEFINEs to be used when building C/C++ code
DEFINES = -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DMANGUSTA_DISCOVERY\
	   -DUSE_USB_OTG_FS -DHSE_VALUE=8000000

# Libraries to be linked into final binary
# TODO: need to make these work for ST if need be
# LIBS = $(LIBS_PREFIX) $(GCC4MBED_DIR)/mri/mri.ar $(EXTERNAL_DIR)/mbed/LPC1768/mbed.ar $(EXTERNAL_DIR)/mbed/LPC1768/capi.ar $(EXTERNAL_DIR)/FATFileSystem/LPC1768/FATFileSystem.ar $(LIBS_SUFFIX)

# Optimization level
OPTIMIZATION = 2

#  Compiler Options
MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb-interwork

#GPFLAGS = -O$(OPTIMIZATION) -gstabs+3 $(MCFLAGS) -fshort-wchar -ffunction-sections -fdata-sections \
 		  -fpromote-loop-indices -Wall -Wextra -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls \
 		  -Wshadow -Wcast-qual -Wcast-align -fno-exceptions
 		  
GPFLAGS = -O$(OPTIMIZATION) -gstabs+3 $(MCFLAGS) -fshort-wchar -ffunction-sections -fdata-sections \
 		  -Wall -Wextra -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls \
 		  -Wshadow -Wcast-qual -Wcast-align -fno-exceptions
 		  
GPFLAGS += $(patsubst %,-I%,$(INCDIRS))
GPFLAGS += $(DEFINES)

#LDFLAGS = -mcpu=cortex-m3 -mthumb -O$(OPTIMIZATION) -Wl,-Map=$(PROJECT).map,--cref,--gc-sections,\
--no-wchar-size-warning -T$(LSCRIPT) -L $(EXTERNAL_DIR)/gcc/LPC1768
LDFLAGS = -mcpu=$(MCU) -mthumb -O$(OPTIMIZATION) -Wl,-Map=$(PROJECT).map,--cref,--gc-sections,--no-wchar-size-warning \
	-T$(LSCRIPT) 
ASFLAGS = $(LISTING) -mcpu=$(MCU) -mthumb -x assembler-with-cpp
ASFLAGS += $(patsubst %,-I%,$(INCDIRS))

#  Compiler/Assembler/Linker Paths
GPP = arm-none-eabi-g++
AS = arm-none-eabi-gcc
LD = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
REMOVE = rm

# Switch to cs-rm on Windows and make sure that cmd.exe is used as shell.
ifeq "$(MAKE)" "cs-make"
REMOVE = cs-rm
SHELL=cmd.exe
endif

#########################################################################
.PHONY: all clean deploy

all:: $(PROJECT).hex $(PROJECT).bin $(PROJECT).disasm

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex
	
$(PROJECT).disasm: $(PROJECT).elf
	$(OBJDUMP) -d $(PROJECT).elf >$(PROJECT).disasm
	
$(PROJECT).elf: $(LSCRIPT) $(OBJECTS) $(LIBS)
	$(LD) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

clean:
	$(REMOVE) -f $(OBJECTS)
	$(REMOVE) -f $(PROJECT).hex
	$(REMOVE) -f $(PROJECT).elf
	$(REMOVE) -f $(PROJECT).map
	$(REMOVE) -f $(PROJECT).bin
	$(REMOVE) -f $(PROJECT).disasm

ifdef LPC_DEPLOY
DEPLOY_COMMAND = $(subst PROJECT,$(PROJECT),$(LPC_DEPLOY))
deploy:
	$(DEPLOY_COMMAND)
endif

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  and assemble .s files to .o

.c.o :
	$(GPP) $(GPFLAGS) -c $< -o $(<:.c=.o)

.cpp.o :
	$(GPP) $(GPFLAGS) -c $< -o $(<:.cpp=.o)

.S.o :
	$(AS) $(ASFLAGS) -c $< -o $(<:.S=.o)

#########################################################################
