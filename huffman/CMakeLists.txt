cmake_minimum_required(VERSION 3.5)
project(huffman)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wconversion -Wpedantic")

set(COMMON_SOURCES
    common.h
    huffman.cpp huffman.h
    huffman_exception.cpp huffman_exception.h
    encoder.cpp encoder.h
    decoder.cpp decoder.h
)

add_executable(encode
    ${COMMON_SOURCES}
    encode.cpp
)

add_executable(decode
    ${COMMON_SOURCES}
    decode.cpp)
