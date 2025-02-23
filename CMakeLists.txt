cmake_minimum_required(VERSION 3.30)
project(computation_1)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt REQUIRED)

add_executable(computation_1 main.cpp
        columns_reorder.cpp)

target_link_libraries(computation_1 PRIVATE fmt::fmt)
