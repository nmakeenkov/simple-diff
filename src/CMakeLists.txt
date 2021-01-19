add_library(simple_diff "")

target_sources(
        simple_diff
    PRIVATE
        longest_common_sub_sequence.cpp diff_computation.cpp diff_serialization.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/simple_diff.h
    )

target_include_directories(
    simple_diff
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )
