[![Build Status](https://travis-ci.org/Derppening/RTLib.svg?branch=master)](https://travis-ci.org/Derppening/RTLib)

# RTLib
Experimental hardware abstraction layer for STM32F103, STM32F105 and STM32F407 devices, based on 
[libopencm3](https://github.com/libopencm3/libopencm3).

Initially an in-house R&D project for the [HKUST Robotics Team](https://robotics.ust.hk/), this project is now
independently maintained by two former members of the team.

## Documentation

All APIs in this project is documented within the header in the most comprehensive way possible, with some references 
back to the libopencm3 library. We strongly recommend using our [Doxygen docs](https://Derppening.github.io/RTLib/).

## Prerequisites
You will be needing the following to compile:

- [arm-none-eabi-gcc](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) (Or your preferred compiler, in that 
case provide your own toolchain file)
- [CMake](https://cmake.org/)
- [GNU Make](https://www.gnu.org/software/make/)
- [Python](https://www.python.org/)

Optional software that will make your life easier:
- [Git](https://git-scm.com/)
- [Jetbrains CLion](https://www.jetbrains.com/clion/)
- One of the following flashing tools:
    - [JLink](https://www.segger.com/products/debug-probes/j-link/technology/flash-download/)
    - [stlink](https://github.com/texane/stlink)
    - [stm32flash](https://sourceforge.net/p/stm32flash/wiki/Home/)

## Usage

1. Add the library files to your project by adding it as a Git submodule to your project:

```bash
git submodule add https://github.com/Derppening/RTLib.git
```

Alternately, add the files by cloning from this repository (useful if you aren't using Git in your project):

```bash
git clone https://github.com/Derppening/RTLib.git
```

2. Initialize all the submodule dependencies.

```bash
cd RTLib
git submodule update --init --recursive
```

3. Make LibOpenCM3.

This step is required by RTLib to recognize which targets are supported by libopencm3. Subsequent builds can skip this 
step, as building RTLib will implicitly build libopencm3.

```bash
cd libopencm3
make
```

4. Modify `CMakeLists.txt`

RTLib needs to know what MCU and board configuration your application is targeting, so modify that as necessary. 
Normally, editing the following lines should be enough:
```cmake
# Choose target device here
set(DEVICE STM32F407VET6)

# Choose target mainboard pin configuration here
set(MAINBOARD_CONFIG STM32F407_DEV)
```

If the pin configuration of your board is not identified by RTLib, you may need to provide your own configuration 
header. See the [examples](https://github.com/Derppening/RTLib/tree/master/src/config) to learn how to write your own.

5. Create your `CMakeLists.txt`.

An example `CMakeLists.txt` is provided in the 
[RTLib-Template repository](https://github.com/Derppening/RTLib-Examples/blob/master/CMakeLists.txt).

5. Create your `main.cpp`.

Remember to define a `main` function!

```cpp
int main() {}
```

5. Building your program

```bash
# Create a new folder to house all CMake-related files.
mkdir cmake-build && cd cmake-build

# Change the -G option according to your build system, might be different for example if you were using MinGW.
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../RTLib/cmake/arm-toolchain.cmake -G "CodeBlocks - Unix Makefiles" ..

# Finally run the make command to initate the build process. This will build everything into the "cmake-build" folder.
cmake --build . --target all
```

The resulting binaries (`RTLib-Template_Debug.bin` and `RTLib-Template_Debug.elf`) will be located in the same 
directory.

If you are using CLion, you can skip all the terminal commands. However, you will have to specify the 
`CMAKE_TOOLCHAIN_FILE` variable manually in `Settings > Build, Execution, Deployment > CMake > CMake Options`.

### Testing on a Device

You should have either an ST-Link, USB-TTL, or JLink for your device. Check your mainboard if you are unsure.

All following code assumes your current working directory is at the root of the project.

#### ST-Link

```bash
../RTLib/scripts/flash_stlink.sh ../[target].bin  # Replace target.bin with the appropriate file
```

#### JLink

```bash
../RTLib/scripts/flash_jlink.sh <device> ./[target].bin  # Replace target.bin with the appropriate file
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

### Versioning

This library uses [SemVer](https://semver.org/) for versioning. Version constants are available in 
[version.h](src/version.h) and are automatically generated for every release.

For the versions available, see the [tags on this repository](https://github.com/Derppening/RTLib/tags).

## Contributors

- **David Mak** - [Derppening](https://github.com/Derppening/)
- **waicool20** - [waicool20](https://github.com/waicool20)

## License

This project is licensed under LGPLv3 - see [COPYING](COPYING) and [COPYING.LESSER](COPYING.LESSER) for details.

[libopencm3](https://github.com/libopencm3/libopencm3) is licensed under 
[LGPLv3](https://github.com/libopencm3/libopencm3/blob/master/COPYING.LGPL3).

