cmake_minimum_required(VERSION 3.5)
project(nns VERSION 0.2.0)

# to access to standard installation directory variables (CMAKE_INSTALL_xDIR).
include(GNUInstallDirs)

# indicate that the documentation build is an option (ON by default)
option(BUILD_DOC "Build documentation" ON)

# set the C++/C standard versions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# if not specified, set Release compilation
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set compilation flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

###############################################################################
# LIBRARY                                                                     #
###############################################################################

# find the source (i.e., .c) files
file(GLOB_RECURSE SOURCES sources/*.c)

# add the include directories (i.e., containing .h files)
include_directories(headers)

# create the library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# find OpenBLAS library
set(BLA_VENDOR OpenBLAS)
find_package(BLAS REQUIRED)

# find LAPACKE (C-LAPACK) library
find_package(LAPACK REQUIRED)

# find Umfpack library
find_package(SuiteSparse_config REQUIRED)
find_package(AMD)
find_package(UMFPACK REQUIRED)

# link the math and LAPACK libraries
target_link_libraries(${PROJECT_NAME} PRIVATE m LAPACK::LAPACK SuiteSparse::UMFPACK)

# if available, link openMP library
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

# install shared library file to /usr/local/lib
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install header files to /usr/local/include
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/headers/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

###############################################################################
# DOCUMENTATION                                                               #
###############################################################################

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

###############################################################################
# EXAMPLES                                                                    #
###############################################################################

# include examples directory
add_subdirectory(examples)

###############################################################################
# TESTS                                                                       #
###############################################################################

# include testing directory
add_subdirectory(tests)
