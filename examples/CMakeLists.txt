# build example programs using the nns library
add_executable(basic.elf basic.c)
target_link_libraries(basic.elf nns)

add_executable(load.elf load.c)
target_link_libraries(load.elf nns)
