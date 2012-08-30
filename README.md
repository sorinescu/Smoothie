# Smoothie for STM32

This is a port of Smoothie on STMicro Cortex M3 and M4 chips.

There are several notable differences between this port and the original Smoothie project:
* It is easier to port to a new chip, because the core implementation is completely independent from the hardware-related code
* Some features (such as file support) are now optional, selected through configuration defines, in order to optimize the footprint on smaller devices
* Config files are not strictly necessary anymore - the runtime behavior and hardware mapping is defined at compile time (in src/platform/DefaultCNCConfig.h).
Of course, it is still possible to use config files (when file support is enabled)
* the original architectures, LPC17xx (arthurwolf) and STM32F4xx (centerumnet) are included but _not maintained_ and _not tested_
* the STM32F103 port is the only one maintained and tested (but not yet stable or complete)
* the code can be built either with a GCC variant (CodeSourcery, Linaro) or with Keil MDK

This is still _work in progress_, so don't expect it to work just yet.

# Original README

Smoothie is a free, opensource, high performance G-code interpreter and CNC controller written in Object-Oriented C++ for the LPC17xx micro-controller ( ARM Cortex M3 architecture ). It will run on a mBed, a LPCXpresso, a SmoothieBoard, R2C2 or any other LPC17xx-based board. The motion control part is a port of the awesome grbl.

Documentation can be found here : http://smoothieware.org/

You need the CodeSourcery toolchain to compile.
Then simply do : 

make clean
make
make flash
