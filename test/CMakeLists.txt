add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

#  keeps CACHE cleaner
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(fz_package_add_test TESTNAME FILES LIBRARIES)
    message(STATUS "Adding test ${TESTNAME} with files ${FILES} and libraries ${LIBRARIES}")
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${FILES})
    # link the Google test infrastructure, mocking library, and a default main function to
    target_link_libraries(${TESTNAME} gtest_main ${LIBRARIES})
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER test)
endmacro()

file(GLOB_RECURSE FZ_TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(XFDTD_EXAMPLES_LIST "")

# make example xxx
foreach(fz_test_source ${FZ_TEST_SOURCE})
    get_filename_component(fz_test_file_name ${fz_test_source} NAME)
    string(REPLACE ".cpp" "" fz_test_name ${fz_test_file_name})
    # add_executable(${fz_test_name} EXCLUDE_FROM_ALL ${fz_test_source})
    fz_package_add_test(${fz_test_name} ${fz_test_source} fz)
endforeach()
