# create the testing file and list of tests
create_test_sourcelist(test_files run_all.c
    device_datasheet.c
    device_network.c
    device_wire.c
    interface_interface.c
    interface_mea.c
    io_de-serializer.c
    stimulator_mna.c
    util_components.c
    util_distributions.c
    util_measures.c
)

# add the testing executable
add_executable(run_all.elf ${test_files})

# link the nns library to the tests
target_link_libraries(run_all.elf PRIVATE nns gsl)

# remove the test driver source file
list(REMOVE_AT test_files 0)

# add all the ADD_TEST for each test
foreach(test ${test_files})
    get_filename_component(test_name ${test} NAME_WE)
    add_test(NAME ${test_name} COMMAND run_all.elf ${test_name})
endforeach()

# enable testing
enable_testing()
