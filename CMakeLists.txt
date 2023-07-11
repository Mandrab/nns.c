cmake_minimum_required(VERSION 3.12)
project(nns)

# indicate that the documentation build is an option (ON by default)
option(BUILD_DOC "Build documentation" ON)

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

# if available, build doxygen docs
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN Doxyfile)
    set(DOXYGEN_OUT Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(
        doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
endif (DOXYGEN_FOUND)

# build example programs using the nns library
add_executable(example1.elf example/example_1.c)
target_link_libraries(example1.elf nns)
