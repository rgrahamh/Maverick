cmake_minimum_required(VERSION 3.22)

project(red VERSION 0.1.0 DESCRIPTION "Asset editor, Ryan Houck 2021-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast")

#TODO: Once we're independent of the current level editor loading scheme, get rid of this lol
#Game source
set(CyberenaObjectSrc
    ../Cyberena/Entity/Object/Character/Character.cpp
    ../Cyberena/Entity/Object/Character/Race/Race.cpp
    ../Cyberena/Entity/Object/Character/Style/Style.cpp
    ../Cyberena/Entity/Object/GlassBall/GlassBall.cpp
)

set(CyberenaEntitySrc
    ${CyberenaObjectSrc}
    ${CyberenaUISrc}
)

set(CyberenaZonesSrc
    ../Cyberena/Zones/TestZone/TestZone.cpp
    ../Cyberena/Zones/global/global.cpp
)

#Editor source
set(LedZonesSrc
    ../led/Zones/led/led.cpp
)

set(LedUISrc
    ../led/Entity/UI/UILevelPort/UILevelPort.cpp
)

set(LedSrc
    ${LedZonesSrc}
    ${LedUISrc}
)

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

target_link_libraries(red
    SDL2
    SDL2_image
    SDL2_mixer
    GL
    maverick
)

target_include_directories(red PUBLIC
    ../../bin
)
