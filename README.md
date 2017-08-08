# RTLib
You will love it :)

# Usage

1. Add the library files to your project by adding it as a submodule to your project:

```bash
git submodule add git://github.com/waicool20/RTLib
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

4. Building your program

```bash
cd RTLib                        # Switch to the RTLib root directory first if you aren't already
mkdir build && cd build         # Then create a build directory where all the generate build files will be output

# Change the -G option according to your build system, might be different for example if you were using MINGW
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake/arm-toolchain.cmake -G "CodeBlocks - Unix Makefiles" ../

# Finally run the make command to initate the build process
make
```

A `.bin` file and `.elf` file will be generated inside the `build` directory

If you are using CLion, then usually the `Build All` configuration is all you need to build the project and you skip all that terminal rubbish

# Updating RTLib

```bash 
cd RTLib                        # Switch to the RTLib root directory first if you aren't already
git pull origin master          # Pull the latest upstream commits
cd ..                           # Go back to your project directory
git status                      # Check that there are changes to RTLib
git add RTLib                   # Add the RTLib directory to the change list
git commit                      # Commit the update
```
