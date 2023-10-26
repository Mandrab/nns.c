# build example programs using the nns library
add_executable(basic.elf basic.c)
target_link_libraries(basic.elf nns)

add_executable(io.elf io.c)
target_link_libraries(io.elf nns)

add_executable(load.elf load.c)
target_link_libraries(load.elf nns)

add_executable(mea.elf mea.c)
target_link_libraries(mea.elf nns)
