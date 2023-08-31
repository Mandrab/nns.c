# NNS: Nanowire Network Simulator

`nns.c` is a C library that provides types and functions for the high performance simulation of Nanowire Networks.
Specifically, it focuses on the randomly-created devices developed at the Polytechnic of Turin by Gianluca Milano, Enrique Miranda and Carlo Ricciardi.
The code allows performing analysis of the structural and functional connectivity of the memristive network.
The library is inspired by the code used in the paper [_Connectome of memristive nanowire networks through graph theory_](https://doi.org/10.1016/j.neunet.2022.02.022), which can be found here [here](https://github.com/MilanoGianluca/Memristive_Nanowire_Networks_Connectome).

## Requirements
- LAPACK with the C interface LAPACKE
- OpenBLAS for the parallelization of the LAPACK routines
- OpenMP for the parallelization of some simulator routines
- [OPTIONAL] Doxygen for the generation of the documentation

## Instructions
To install the library system-wide: `cmake . && make && sudo make install`</br>
To run the examples: `cmake . && make && time ./examples/[EXAMPLE NAME].elf`</br>
To run the tests: `cmake . && make && cd tests && ctest; cd ..`</br>
To visualize the saved networks, for examples produced by io.elf: `analyzers/state.py device_[ID]/constant.nn.nns device_[ID]/instant_[STEP].it.nns device_[ID]/instant_[STEP].ns.nns device_[ID]/instant_[STEP + 1].ns.nns ...`

## Troubleshoot
- [IMPORTANT] A segmentation fault may happen if too large networks are simulated. To solve this problem it is simply needed to increase the memory that the program can allocate. See: `ulimit -s 32767`.
