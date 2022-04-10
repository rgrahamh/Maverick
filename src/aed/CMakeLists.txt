cmake_minimum_required(VERSION 3.22)

project(aed VERSION 0.1.0 DESCRIPTION "Asset editor, Ryan Houck 2021-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast")

#Game source
set(aedSrc
    ./aed.cpp
)

add_executable(aed
    ${aedSrc}
)

target_include_directories(aed
    ../../bin
)