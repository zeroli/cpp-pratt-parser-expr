cmake_minimum_required(VERSION 3.18)

add_executable(ut
    main.cc
    parser_test.cc
)

target_include_directories(ut PRIVATE ../src)
target_link_libraries(ut PRIVATE cpp-pratt-parser-expr)
target_link_libraries(ut PRIVATE gtest)
