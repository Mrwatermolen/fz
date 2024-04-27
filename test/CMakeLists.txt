file(GLOB_RECURSE FZ_TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_custom_target(build-examples COMMAND ${CMAKE_COMMAND} -E echo "Build all examples")

set(XFDTD_EXAMPLES_LIST "")

# make example xxx
foreach(fz_test_source ${FZ_TEST_SOURCE})
    get_filename_component(fz_test_file_name ${fz_test_source} NAME)
    string(REPLACE ".cpp" "" fz_test_name ${fz_test_file_name})
    add_executable(${fz_test_name} EXCLUDE_FROM_ALL ${fz_test_source})
    target_link_libraries(${fz_test_name} fz)
    add_test(NAME ${fz_test_name}_test COMMAND ${fz_test_name})
endforeach()