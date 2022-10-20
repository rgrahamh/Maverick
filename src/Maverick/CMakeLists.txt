cmake_minimum_required(VERSION 3.19)

project(maverick VERSION 0.1.0 DESCRIPTION "Maverick Game Engine, Ryan Houck 2020-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -shared -Wall -pipe -pthread -Wno-write-strings -Wno-int-to-pointer-cast -shared -fPIC")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin")
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

#Engine source
set(AnimationSrc
    Animation/Animation.cpp
)

set(AudioSrc
    Audio/Audio.cpp
    Audio/Music/Music.cpp
    Audio/SoundBoard/SoundBoard.cpp
)

set(FontSrc
    Font/Font.cpp
)

set(HitboxSrc
    Animation/Hitbox/Hitbox.cpp
    Animation/Hitbox/HitEllipse/HitEllipse.cpp
    Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp
    Animation/Hitbox/HitRect/HitRect.cpp
    Animation/Hitbox/HitboxCollision.cpp
)

set(CameraSrc
    Camera/Camera.cpp
)

set(ControlSrc
    Control/Control.cpp
)

set(UISrc
    UIElement/UIElement.cpp
    UIElement/UIBorders/UIBorders.cpp
    UIElement/UIText/UIText.cpp
    UIElement/UITextBox/UITextBox.cpp
    UIElement/UIObjectFrame/UIObjectFrame.cpp
    UIElement/UITabbedWindow/UITabbedWindow.cpp
    ${FontSrc}
)

set(ObjectSrc
    Object/Object.cpp
    Object/CameraReference/CameraReference.cpp
)

set(ZoneSrc
    Zone/Zone.cpp
)

set(FactorySrc
    ../Cyberena/Factory/Factory.cpp
)

set(FileHandlerSrc
    FileHandler/Saver/Saver.cpp
    FileHandler/Loader/Loader.cpp
)

set(EngineSrc
    Engine/Engine.cpp
)

set(MotorSrc
    ${AnimationSrc}
    ${AudioSrc}
    ${HashSrc}
    ${HitboxSrc}
    ${CameraSrc}
    ${ControlSrc}
    ${ObjectSrc}
    ${UISrc}
    ${ZoneSrc}
    ${ZoneFactorySrc}
    ${FactorySrc}
    ${FileHandlerSrc}
    ${EngineSrc}
    ${CyberenaObjectSrc}
    ${CyberenaZonesSrc}
    ${LedObjectSrc}
    ${LedZonesSrc}
)

#Dynamic lib compilation
add_library(maverick
    ${MotorSrc}
)

target_link_libraries(maverick
    SDL2
    SDL2_image
    SDL2_mixer
)

target_include_directories(maverick PUBLIC
    ../../src
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(maverick
        GL
    )
endif()

set_target_properties(maverick PROPERTIES VERSION ${PROJECT_VERSION})