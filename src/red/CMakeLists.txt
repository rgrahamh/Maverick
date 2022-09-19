cmake_minimum_required(VERSION 3.19)

project(red VERSION 0.1.0 DESCRIPTION "Asset editor, Ryan Houck 2021-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fstandalone-debug -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")

set(LIBS_DIR "../../libs")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    link_directories(
        ${LIBS_DIR}/Linux/SDL2/bin
        ${LIBS_DIR}/Linux/SDL2_image/bin
        ${LIBS_DIR}/Linux/SDL2_mixer/bin
        ../../bin
    )
    include_directories(
        ${LIBS_DIR}/Linux/SDL2/include
        ${LIBS_DIR}/Linux/SDL2/include/SDL2
        ${LIBS_DIR}/Linux/SDL2_image/include
        ${LIBS_DIR}/Linux/SDL2_mixer/include
    )
else()
    link_directories(
        ${LIBS_DIR}/Windows/SDL2/bin
        ${LIBS_DIR}/Windows/SDL2_image/bin
        ${LIBS_DIR}/Windows/SDL2_mixer/bin
        ../../bin
    )
    include_directories(
        ${LIBS_DIR}/Windows/SDL2/include
        ${LIBS_DIR}/Windows/SDL2/include/SDL2
        ${LIBS_DIR}/Windows/SDL2_image/include
        ${LIBS_DIR}/Windows/SDL2_mixer/include
    )
endif()

#Actual source
set(redSrc
    ./red.cpp
)

add_executable(red
    ${redSrc}
    ${CyberenaEntitySrc}
    ${CyberenaZonesSrc}
    ${LedSrc}
)

target_link_directories(red PUBLIC
    ../../bin
)

target_include_directories(red PUBLIC
    ../../bin
    ../../src
)

target_link_libraries(red
    SDL2
    SDL2_image
    SDL2_mixer
    maverick
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(red
        GL
    )
endif()