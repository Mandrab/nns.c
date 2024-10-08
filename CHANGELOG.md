# Change Log

All notable changes to this project will be documented in this file.

## [v1.0.3] — 2024-09-20

### Added
### Changed
### Fixed
- Bug in generation of network topology.



## [v1.0.2] — 2024-09-19

### Added
### Changed
- Changed theme in doxygen documentation.
### Fixed
- Generation of the nanowire network is now more robust as the seed is set internally before the creation.



## [v1.0.1] — 2024-09-18

### Added
- GitHub workflow for the automatic generation of documentation.
### Changed
### Fixed



## [v1.0.0] — 2024-09-18

### Added
- Software license.
### Changed
### Fixed



## [v0.4.2-beta] — 2024-05-30

### Added
### Changed
### Fixed
- Reproducible network generation across different Unix distributions.



## [v0.4.1-beta] — 2024-03-11

### Added
### Changed
- LAPACK function call in order to work with older versions of the library.
### Fixed
- Parameters in examples.



## [v0.4.0-beta] — 2023-12-14

### Added
- The MEA structure for a more realistic connection of sources, grounds, and loads.
- New utility functions for the comparison, copy, creation, and destruction of data structures.
### Changed
### Fixed
- CMake automation and README instructions to work on macOS.



## [v0.3.0-alpha] — 2023-10-26

### Added
### Changed
- Used UMFPACK instead of LAPACK for the MNA solution: passed from solving a system of equations to solve a _sparse_ system of equations.
- Computation speed increased by ~500%.
### Fixed
- Bug in MNA solution when multiple sources are used.



## [v0.2.0-alpha] — 2023-10-12

### Added
- Connected component abstraction, allowing to use multiple connected components belonging to the same NN.
### Changed
- Linearized the simulation data: from array of arrays to array.
- Reduced the memory usage of the simulator by using wires and junctions index instead of containing the whole sparse matrix.
- Computation speed increased by ~10%.
### Fixed



## [v0.1.1-alpha] — 2023-09-27

### Added
### Changed
- Reduced the memory usage of the simulator.
### Fixed



## [v0.1.0-alpha] — 2023-09-27

### Added
- First alpha version of the NN simulator.
### Changed
### Fixed
