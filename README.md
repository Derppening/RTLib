[![Build Status](https://travis-ci.org/Derppening/RTLib.svg?branch=master)](https://travis-ci.org/Derppening/RTLib)

# RTLib
Experimental hardware abstraction layer for STM32F103 and STM32F407 devices, based on 
[libopencm3](https://github.com/libopencm3/libopencm3).

This project is an in-house R&D project for the [HKUST Robotics Team](https://robotics.ust.hk/).

## Documentation

All APIs in this project is documented within the header in the most comprehensive way possible, with some references 
back to the libopencm3 library. We strongly recommend using our [Doxygen docs](https://Derppening.github.io/RTLib/).

## Prerequisites
You will be needing the following to compile:

- [arm-none-eabi-gcc](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) (Or your preferred compiler, in that 
case provide your own toolchain file)
- [CMake](https://cmake.org/)
- [GNU Make](https://www.gnu.org/software/make/)
- [GNU awk](https://www.gnu.org/software/gawk/)

Optional software that will make your life easier:
- [Git](https://git-scm.com/)
- [Jetbrains CLion](https://www.jetbrains.com/clion/)
- One of the following flashing tools:
    - [JLink](https://www.segger.com/products/debug-probes/j-link/technology/flash-download/)
    - [stlink](https://github.com/texane/stlink)
    - [stm32flash](https://sourceforge.net/p/stm32flash/wiki/Home/)

## Usage

1. Add the library files to your project by adding it as a submodule to your project:

```bash
git submodule add git://github.com/Derppening/RTLib
```

2. Initialize all the submodule dependencies

```bash
git submodule update --init --recursive
```

3. Make LibOpenCM3

```bash
cd RTLib/libopencm3
make
```

4. Modify your `CMakeLists.txt`

You need to let your compiler know about `libopencm3` and `RTLib`, checkout the `CMakeLists.txt` for an example on how 
to your write yours!

5. Building your program

```bash
cd RTLib                        # Switch to the RTLib root directory first if you aren't already
mkdir build && cd build         # Then create a build directory where all the generate build files will be output

# Change the -G option according to your build system, might be different for example if you were using MINGW
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-toolchain.cmake -G "CodeBlocks - Unix Makefiles" ../

# Finally run the make command to initate the build process
cmake --build .
```

A `.bin` file and `.elf` file will be generated inside the `build` directory.

If you are using CLion, then usually the `Build All` configuration is all you need to build the project and you skip all 
that terminal rubbish.

### Testing on a Device

You should have either an ST-Link, USB-TTL, or JLink for your device. Check your mainboard if you are unsure.

All following code assumes your current working directory is at the root of the project.

#### ST-Link

```bash
cd scripts/
./flash_stlink.sh ../build/[target].bin  # Replace target.bin with the appropriate file
```

#### JLink

```bash
cd scripts/
./flash.sh <device> ./jlink/[debug|release].jlink  # Replace [debug|release] with the file you want to flash
```

#### USB-TTL

```bash
cd build/

# Replace:
# [target.bin] with the appropriate file
# [115200] with the target baud rate
# [/dev/ttyUSB0] with the target device
sudo stm32flash -w [target.bin] -v -g 0x0 -b [115200] [/dev/ttyUSB0] 
```

Note: Please turn your device into BOOT mode when flashing via USB-TTL.

### Updating RTLib

```bash 
cd RTLib                        # Switch to the RTLib root directory first if you aren't already
git pull origin master          # Pull the latest upstream commits
cd ..                           # Go back to your project directory
git status                      # Check that there are changes to RTLib
git add RTLib                   # Add the RTLib directory to the change list
git commit                      # Commit the update
```

## Contributors

- **David Mak** - [Derppening](https://github.com/Derppening/)
- **waicool20** - [waicool20](https://github.com/waicool20)

## License

This project is licensed under LGPLv3 - see [LICENSE](LICENSE.md) for details.

[libopencm3](https://github.com/libopencm3/libopencm3) is licensed under 
[LGPLv3](https://github.com/libopencm3/libopencm3/blob/master/COPYING.LGPL3).

