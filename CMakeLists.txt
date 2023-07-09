cmake_minimum_required(VERSION 3.12)
project(nns)

# set the C standard version
set(CMAKE_C_STANDARD 99)

# find the source (i.e., .c) files
file(GLOB_RECURSE SOURCES source/*.c)

# add the include directories (i.e., containing .h files)
include_directories(include)

# create the library
add_library(nns STATIC ${SOURCES})

# link the math library
target_link_libraries(nns PRIVATE m)

# if available, link openMP library
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    target_link_libraries(nns PUBLIC OpenMP::OpenMP_CXX)
endif()

# build example programs using the nns library
add_executable(example1.elf example/example_1.c)
target_link_libraries(example1.elf nns)
