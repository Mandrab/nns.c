# NNS: Nanowire Network Simulator

`nns.c` is a C library that provides types and functions for the high performance simulation of Nanowire Networks.
Specifically, it focuses on the randomly-created devices developed at the Polytechnic of Turin by Gianluca Milano, Enrique Miranda and Carlo Ricciardi.
The code allows performing analysis of the structural and functional connectivity of the memristive network.
The library is inspired by the code used in the paper [_Connectome of memristive nanowire networks through graph theory_](https://doi.org/10.1016/j.neunet.2022.02.022), which can be found [here](https://github.com/MilanoGianluca/Memristive_Nanowire_Networks_Connectome).

## Table of Contents
1. [Software design](https://github.com/Mandrab/nns.c?tab=readme-ov-file#software-design)
2. [Requirements](https://github.com/Mandrab/nns.c?tab=readme-ov-file#requirements)
3. [Installation](https://github.com/Mandrab/nns.c?tab=readme-ov-file#installation)
    1. Linux
    2. macOS
    3. Windows
4. [Usage](https://github.com/Mandrab/nns.c?tab=readme-ov-file#usage)
5. [Troubleshoot](https://github.com/Mandrab/nns.c?tab=readme-ov-file#troubleshoot)
6. [Credits](https://github.com/Mandrab/nns.c?tab=readme-ov-file#credits)

## Software design

The main components of the simulator are:
1. `datasheet`: Represents the static information of the Nanowire Network, such as the number of nanowires, their length, etc.
2. `network_topology`: Contains the physical distribution of the nanowires and their junctions.
3. `network_state`: Contains the dynamic (or instantaneous) state of the Nanowire Network, represented as the equivalent electrical circuit.
4. `connected_component`: Represents a connected group of nanowires inside the Nanowire Network, and contains the information to address them in the `network state'.

An example of their relationship is shown in the following diagram:

![Structural design of the simulator data](structure.drawio.png)

The stimulation of the NN uses the `interface` and `mea` components, that specify how sources, grounds, and loads connect to the NN.
Specifically, an interface allows to select all the possible nanowires as sources / grounds / loads.
A MEA limits instead the choice to nanowires positioned near to physical electrodes, allowing a more realistic configuration.

![Nanowire network connection logic](connection.drawio.png)

## Requirements
- GNU Scientific Library for reproducible random number generation
- LAPACK with the C interface LAPACKE
- OpenBLAS for the parallelization of the LAPACK routines
- OpenMP for the parallelization of some simulator routines
- [OPTIONAL] Doxygen for the generation of the documentation

## Installation

Overall, the steps to compile and install the simulator are the following:

1. To compile the source code, examples, and tests: `cmake . && make`</br>
2. To install the library system-wide: `sudo make install`</br>

Nevertheless, the installation slightly vary across different Operative Systems.
A brief (and not exhaustive) step-guide if given below for each tested OS.

### Linux

Tested on an Arch installation.

Due to the variety of Linux installations, a precise guide cannot be provided.
Assuming that you have installed all the required dependencies in the correct locations, the installation is expected to consist of the following steps:

```
$ cmake .
$ make
$ sudo make install
```

### macOS

Tested on a macOS Big Sur installation.

The installation under macOS requires some attentions.
Firstly, the Clang compiler used does not yet support the empty initialization of arrays (C23), and cause some problems with OpenMP.
To solve these issues, the use of a recent version of GCC and C++ compilers is recommended.
However, `gcc` in a macOS is a link to the `clang` compiler.
To overcome this problem, it is necessary to specify the gcc version, targeting a specific installation of the compiler.
An example is available below, where I assume GCC-13 e C++-13 to be installed.
Another issue relates some brew installations that are not directly accessible by CMake (e.g., OpenBLAS).
To allow the compilation to succeed, it is needed to specify the location of those installed libraries.
An example is available below.

Installing the dependencies:

```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
$ brew install cmake
$ brew install gsl
$ brew install openblas
$ brew install suitesparse
$ brew install lapack
```

Installing the simulator:

```
$ cmake . \
    -DCMAKE_C_COMPILER=/usr/local/bin/gcc-13
    -DCMAKE_CXX_COMPILER=/usr/local/bin/c++-13 \
    -DCMAKE_PREFIX_PATH="/usr/local/opt/openblas"
```

### Windows

Not tested (yet).

The simulator is not currently tested under Windows.
The use of the Windows Subsystem for Linux (WSL) is expected to work.

## Usage

Once the simulator is installed, you can write your code.
For a brief introduction, please check the test and example codes.
They represent typical usage of the library.

Running the tests:
```
$ cd tests
$ ctest
$ cd ..
```

Running the examples:

```
$ ./examples/[EXAMPLE NAME].elf
```

Linking the library in your code:

```
$ gcc [CODE NAME].c -lnns
```

Linking the library in your code (CMake):

```
$ find_library(NNS_LIB nns)
$ target_link_libraries(${PROJECT_NAME} ${NNS_LIB})
```

Running a Valgrind memory-leak test:

```
$ valgrind --leak-check=yes --log-file=valgrind.rpt [EXECUTABLE NAME].elf
```

## Troubleshoot
- If running `cmake .` you get something such as `Could NOT find BLAS (missing: BLAS_LIBRARIES)`, ensure that BLAS is installed in you system and add something such as `-DBLAS_LIBRARIES=/usr/lib64/libblas.so` to the call.
- If running `cmake .` you get something such as `Could NOT find LAPACK (missing: LAPACK_LIBRARIES)`, ensure that LAPACK is installed in you system and add something such as `-DLAPACK_LIBRARIES=/usr/lib32/liblapack.so` to the call.
- A segmentation fault may happen if too large networks are simulated. To solve this problem it is simply needed to increase the memory that the program can allocate. See: `ulimit -s 65535`.
- The Valgrind test [does not work correctly](https://medium.com/@auraham/pseudo-memory-leaks-when-using-openmp-11a383cc4cf9) when used with OpenMP. Therefore, to perform the test is necessary to compile the library without OpenMP and with a sequential implementation of BLAS.

## Credits

- [Noemi Valentini](https://github.com/noemival97) helped troubleshooting the CMake automation in macOS, and pointed out the lack of a known RNG producing the same results across different distributions.
