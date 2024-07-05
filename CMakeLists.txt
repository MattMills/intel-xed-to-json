cmake_minimum_required(VERSION 3.10)

project(decode_xed)

set(CMAKE_CXX_STANDARD 17)

# Set XED_ROOT to the provided path
if (NOT XED_ROOT)
    set(XED_ROOT ${CMAKE_SOURCE_DIR}/xed)
endif()

add_subdirectory(${XED_ROOT} ${CMAKE_BINARY_DIR}/xed)

include_directories(${XED_ROOT}/include)
link_directories(${XED_ROOT}/lib)

add_executable(decode_xed src/decode_xed.cpp)

target_link_libraries(decode_xed PRIVATE xed)
