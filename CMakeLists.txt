cmake_minimum_required(VERSION 3.10)

project(decode_xed)

set(CMAKE_CXX_STANDARD 17)

# Specify the XED root directory
set(XED_ROOT ${CMAKE_SOURCE_DIR}/../xed)

# Include the XED headers
include_directories(${XED_ROOT}/include)
include_directories(${XED_ROOT}/obj/include)

# Link to the XED library
link_directories(${XED_ROOT}/lib)

add_executable(decode_xed src/decode_xed.cpp)

# Link XED library
target_link_libraries(decode_xed xed)
