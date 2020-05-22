# CppND-System-Monitor
System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

Starter code here: https://github.com/udacity/CppND-System-Monitor-Project-Updated.git

![System Monitor](images/monitor.png)

## Requirements
* gcc minimum version 9 (Update cmakelists.txt to link to correct library if multiple versions installed on your system.)
* ncurses

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

To install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions
1. Install requirements

2. Clean up any previous builds: `make clean`

3. Build the project: `make build`

4. Run the resulting executable: `./build/monitor`
![My Working System Monitor](images/my_monitor.png)