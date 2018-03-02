cmake_minimum_required(VERSION 3.5)
project(huffman)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wconversion -Wpedantic")

set(COMMON_SOURCES
        src/common.h
        src/huffman.cpp src/huffman.h
        src/huffman_exception.cpp src/huffman_exception.h
        src/encoder.cpp src/encoder.h
        src/decoder.cpp src/decoder.h
)

add_executable(encode
    ${COMMON_SOURCES}
        src/encode.cpp
)

add_executable(decode
    ${COMMON_SOURCES}
        src/decode.cpp)
