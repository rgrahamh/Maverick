cmake_minimum_required(VERSION 3.19)

project(Cyberena VERSION 0.1.0 DESCRIPTION "Cyberena, Ryan Houck 2021-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -pipe -Wno-write-strings -Wno-int-to-pointer-cast")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")
set(CMAKE_BUILD_PARALLEL_LEVEL "16")

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

#Game source
set(CyberenaObjectSrc
    Cyberena/Object/Character/Character.cpp
    Cyberena/Object/Character/Race/Race.cpp
    Cyberena/Object/Character/Style/Style.cpp
    Cyberena/Object/GlassBall/GlassBall.cpp
    Cyberena/Object/LoadingZone/LoadingZone.cpp
)

set(CyberenaUISrc
)

set(CyberenaEntitySrc
    ${CyberenaObjectSrc}
    ${CyberenaUISrc}
)

set(CyberenaZonesSrc
    Cyberena/Zones/ZoneFactory/ZoneFactory.cpp
    Cyberena/Zones/TestZone/TestZone.cpp
    Cyberena/Zones/global/global.cpp
)

set(CyberenaSrc
    ${CyberenaEntitySrc}
    ${CyberenaZonesSrc}
    ./Cyberena.cpp
)

add_executable(cyberena
    ${CyberenaSrc}
)

target_include_directories(cyberena PUBLIC
    ../../bin
    ../../src
)

target_link_libraries(cyberena
    SDL2
    SDL2_image
    SDL2_mixer
    maverick
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(cyberena
        GL
    )
endif()