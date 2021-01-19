add_executable(
        unit_tests
        longest_common_sub_sequence_tests.cpp
        diff_computation_tests.cpp
)

target_link_libraries(
        unit_tests
        gtest_main
        simple_diff
)

add_test(
        NAME
        unit
        COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)
