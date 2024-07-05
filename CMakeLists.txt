cmake_minimum_required(VERSION 3.10)

project(decode_xed)

set(CMAKE_CXX_STANDARD 17)

find_package(XED REQUIRED)

add_executable(decode_xed src/decode_xed.cpp)

target_include_directories(decode_xed PRIVATE ${XED_INCLUDE_DIRS})
target_link_libraries(decode_xed PRIVATE ${XED_LIBRARIES})
