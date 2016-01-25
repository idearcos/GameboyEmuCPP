# GameboyEmuCPP
GameBoy emulator in C++. Still under construction.

The goal of this emulator is purely educational. The implementation is written as accurate as possible to the real hardware, in order to learn how a real GameBoy works inside.
Performance is not a primary goal of this emulator, therefore no tricks (such as scheduling the next VBlank interrupt instead of checking interrupts after every instruction) are used to speed up the emulation.
This code is still under construction, therefore it has not been through severe optimization yet.

Current implementation level:

* CPU: all instructions implemented. Passes Blargg's *cpu_instrs.gb* test.
* Graphics: fully implemented (Background, Window, Sprites).
* Timer: implemented.
* OAM DMA: implemented.
* Memory Bank Controller types:
  - No MBC
  - MBC1

Pending:

* Audio
* Link cable
* Other MBC types

Not intended for implementation

* GameBoy Color hardware

## Build

CMakeLists.txt files are provided to build the projects with CMake. (The iniGB_Test is currently not included for build, as Blargg's test ROMs are used as primary test method)

Paths for GLFW and GLEW include/libraries must be provided in CMake. Working versions of both libraries are included in the Dependencies directory, which can be built following their own build instructions.

## Usage

Execute the iniGB executable. Currently the path of the ROM to be executed is hardcoded in the *GameBoy.cpp* file.

The GameBoy keypad is currently mapped to:

* Control D-Pad -> WASD 
* A-B buttons -> J-K
* Start -> Space
* Select -> Left Ctrl

## Author

Iñigo de Arcos [inigodearcos@gmail.com](mailto:inigodearcos@gmail.com)
