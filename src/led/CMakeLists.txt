cmake_minimum_required(VERSION 3.22)

project(led VERSION 0.1.0 DESCRIPTION "led, Ryan Houck 2021-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast")

#Game source
set(CyberenaObjectSrc
    ../Cyberena/Entity/Object/Character/Character.cpp
    ../Cyberena/Entity/Object/Character/Race/Race.cpp
    ../Cyberena/Entity/Object/Character/Style/Style.cpp
    ../Cyberena/Entity/Object/GlassBall/GlassBall.cpp
)

set(CyberenaUISrc
)

set(CyberenaEntitySrc
    ${CyberenaObjectSrc}
    ${CyberenaUISrc}
)

set(CyberenaZonesSrc
    ../Cyberena/Zones/TestZone/TestZone.cpp
    ../Cyberena/Zones/global/global.cpp
)

set(CyberenaSrc
    ${CyberenaEntitySrc}
    ${CyberenaZonesSrc}
)

#Editor source
set(LedZonesSrc
    Zones/led/led.cpp
)

set(LedUISrc
    Entity/UI/UILevelPort/UILevelPort.cpp
)

set(LedSrc
    ${LedZonesSrc}
    ${LedUISrc}
)

add_executable(led
    ${CyberenaSrc}
    ${LedSrc}
    ../Cyberena/Cyberena.cpp
)

target_link_directories(led PUBLIC ../../bin)

target_link_libraries(led
    SDL2
    SDL2_image
    SDL2_mixer
    GL
    maverick
)
