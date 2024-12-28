cmake_minimum_required(VERSION 3.18)

add_executable(example
    main.cc
)

target_include_directories(example PRIVATE ../src)
target_link_libraries(example PRIVATE cpp-pratt-parser-expr)
