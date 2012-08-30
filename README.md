This is a port of Smoothie on STMicro Cortex M3 chips (like STM32F103).

The original project README is below:

Smoothie is a free, opensource, high performance G-code interpreter and CNC controller written in Object-Oriented C++ for the LPC17xx micro-controller ( ARM Cortex M3 architecture ). It will run on a mBed, a LPCXpresso, a SmoothieBoard, R2C2 or any other LPC17xx-based board. The motion control part is a port of the awesome grbl.

Documentation can be found here : http://smoothieware.org/

You need the CodeSourcery toolchain to compile.
Then simply do : 

make clean
make
make flash
