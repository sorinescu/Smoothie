PROJECT = Smoothie

#****************************************************************************************
#*                              Start of user customizations                            *
#****************************************************************************************

# Actual platform used by Smoothie
SMOOTHIE_PLATFORM:=SMOOTHIE_PLATFORM_STM32F103
#SMOOTHIE_PLATFORM:=SMOOTHIE_PLATFORM_STM32F4XX
#SMOOTHIE_PLATFORM:=SMOOTHIE_PLATFORM_LPC17XX

# Set this to 1 if you prefer to use STL classes (like std::string and std::vector)
# instead of EASTL ones. STL has a higher overhead, but may be more stable on some
# platforms.
# By default, EASTL is used (smaller footprint).
#SMOOTHIE_USE_STL:=1

# Set this to 1 if you need to use files (e.g. config files, SD card etc), 0 otherwise.
# If not defined here, it will have the default value for the platform (see Platform.h).
#SMOOTHIE_USE_FILES:=1

# Set this to 1 if you need to use strings as config values.
# This is required if you have config files or if you need to set configuration fields
# from the console (serial interface).
# If not defined here, it will be 1 if SMOOTHIE_USE_FILES is enabled, otherwise it
# will be 0 (smaller footprint).
#SMOOTHIE_HAS_CONFIG_VALUE_STRING:=1

# Set this to 0 if you don't need a serial console.
# The serial console is used for receiving G-codes and other commands, so don't disable
# it unless you have some other way of passing commands to Smoothie.
# If not defined here, it will be 1.
#SMOOTHIE_HAS_SERIAL_CONSOLE:=0

#****************************************************************************************
#*                               End of user customizations                             *
#****************************************************************************************

$(warning $(SMOOTHIE_PLATFORM))

# List of sources to be compiled/assembled
CSRCS :=
ASRCS :=
CPPSRCS :=
INCDIRS :=
STM32_INCLUDES :=
PROJINCS :=
LSCRIPT :=
MCFLAGS :=
MLDFLAGS :=
MASFLAGS :=
DEFINES :=

# Get platform directory
ifeq "$(SMOOTHIE_PLATFORM)" "SMOOTHIE_PLATFORM_STM32F103"
PLATFORM_DIR:=src/platform/stm32/stm32f103
else 
ifeq "$(SMOOTHIE_PLATFORM)" "SMOOTHIE_PLATFORM_STM32F4XX"
PLATFORM_DIR:=src/platform/stm32/stm32f4xx
else
$(error "Unsupported platform $(SMOOTHIE_PLATFORM); please edit the makefile")
endif
endif

$(warning $(PLATFORM_DIR))

# Sources that are platform independent
CPPSRCS += \
    src/libs/Pauser.cpp \
    src/libs/Hook.cpp \
    src/libs/Config.cpp \
    src/libs/Adc.cpp \
    src/libs/Kernel.cpp \
    src/libs/ADC/adc.cpp \
    src/libs/Module.cpp \
    src/libs/utils.cpp \
    src/modules/utils/configurator/Configurator.cpp \
    src/modules/utils/pausebutton/PauseButton.cpp \
    src/modules/utils/currentcontrol/CurrentControl.cpp \
    src/modules/robot/Stepper.cpp \
    src/modules/robot/arm_solutions/CartesianSolution.cpp \
    src/modules/robot/arm_solutions/BaseSolution.cpp \
    src/modules/robot/Robot.cpp \
    src/modules/robot/Player.cpp \
    src/modules/robot/Block.cpp \
    src/modules/robot/Planner.cpp \
    src/modules/communication/utils/Gcode.cpp \
    src/modules/communication/GcodeDispatch.cpp

# Add file config source if files are used
ifneq "$(SMOOTHIE_USE_FILES)" "1"
CPPSRCS += \
    src/libs/ConfigSources/FileConfigSource.cpp
endif

# Add EASTL sources if STL is not used
ifneq "$(SMOOTHIE_USE_STL)" "1"
CPPSRCS += \
    src/platform/shared/EASTL/src/string.cpp \
    src/platform/shared/EASTL/src/allocator.cpp \
    src/platform/shared/EASTLGlue.cpp

INCDIRS += \
	src/platform/shared/EASTL/include
endif

# Fill in list of sources, includes etc. from platform
include $(PLATFORM_DIR)/Makefile.inc

# List of the objects files to be compiled/assembled
OBJECTS := $(CSRCS:src/%.c=obj-gcc/%.o) $(ASRCS:src/%.s=obj-gcc/%.o) $(CPPSRCS:src/%.cpp=obj-gcc/%.o)

# Include path which points to external library headers and to subdirectories of this project which contain headers.
INCDIRS += $(PROJINCS) 
INCDIRS += $(STM32_INCLUDES)	

# Optimization level
OPTIMIZATION = s

#  Compiler Options
CFLAGS_COM = -O$(OPTIMIZATION) -gstabs+3 $(MCFLAGS) -fshort-wchar -ffunction-sections -fdata-sections \
			-Wall -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls \
			-Wshadow -Wcast-qual -Wcast-align -fno-exceptions -nostdlib -ffreestanding 
 		  
CFLAGS_COM += $(patsubst %,-I%,$(INCDIRS))
CFLAGS_COM += $(DEFINES)

CFLAGS = $(CFLAGS_COM) -Wimplicit
CXXFLAGS = $(CFLAGS_COM) -fpermissive

LDFLAGS = $(MLDFLAGS) -O$(OPTIMIZATION) -Wl,-Map=$(PROJECT).map,--cref,--gc-sections,--no-wchar-size-warning \
		-nostartfiles -T$(LSCRIPT) 
ASFLAGS = $(LISTING) $(MASFLAGS)
ASFLAGS += $(patsubst %,-I%,$(INCDIRS))

#  Compiler/Assembler/Linker Paths
CROSS := arm-none-eabi-
GC = $(CROSS)gcc
GPP = $(CROSS)g++
AS = $(CROSS)as
LD = $(CROSS)g++
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
SIZE = $(CROSS)size
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

src/config.h: src/config.h.in
	@cp $< $@
	@sed -i -r "s/@SMOOTHIE_PLATFORM@/#define SMOOTHIE_PLATFORM $(SMOOTHIE_PLATFORM)/" $@; fi
	@if [ -n "$(SMOOTHIE_USE_STL)" ]; then sed -i -r "s/@SMOOTHIE_USE_STL@/#define SMOOTHIE_USE_STL $(SMOOTHIE_USE_STL)/" $@; fi
	@if [ -n "$(SMOOTHIE_USE_FILES)" ]; then sed -i -r "s/@SMOOTHIE_USE_FILES@/#define SMOOTHIE_USE_FILES $(SMOOTHIE_USE_FILES)/" $@; fi
	@if [ -n "$(SMOOTHIE_HAS_CONFIG_VALUE_STRING)" ]; then sed -i -r "s/@SMOOTHIE_HAS_CONFIG_VALUE_STRING@/#define SMOOTHIE_HAS_CONFIG_VALUE_STRING $(SMOOTHIE_HAS_CONFIG_VALUE_STRING)/" $@; fi
	@if [ -n "$(SMOOTHIE_HAS_SERIAL_CONSOLE)" ]; then sed -i -r "s/@SMOOTHIE_HAS_SERIAL_CONSOLE@/#define SMOOTHIE_HAS_SERIAL_CONSOLE $(SMOOTHIE_HAS_SERIAL_CONSOLE)/" $@; fi
	@sed -i -r "s^@([A-Z_0-9]+)@^//#define \\1^" $@

clean:
	$(REMOVE) -f $(OBJECTS)
	$(REMOVE) -f $(PROJECT).hex
	$(REMOVE) -f $(PROJECT).elf
	$(REMOVE) -f $(PROJECT).map
	$(REMOVE) -f $(PROJECT).bin
	$(REMOVE) -f $(PROJECT).disasm
	$(REMOVE) -f src/config.h

ifdef LPC_DEPLOY
DEPLOY_COMMAND = $(subst PROJECT,$(PROJECT),$(LPC_DEPLOY))
deploy:
	$(DEPLOY_COMMAND)
endif

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  and assemble .s files to .o

$(OBJECTS): src/config.h

obj-gcc/%.o: src/%.c
	@mkdir -p `dirname $@`
	$(GC) $(CFLAGS) -c $< -o $@

obj-gcc/%.o: src/%.cpp
	@mkdir -p `dirname $@`
	$(GPP) $(CXXFLAGS) -c $< -o $@

obj-gcc/%.o: src/%.s
	@mkdir -p `dirname $@`
	$(AS) $(ASFLAGS) -c $< -o $@

#########################################################################
