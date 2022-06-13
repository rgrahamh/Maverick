cmake_minimum_required(VERSION 3.22)

project(maverick_unit_tests VERSION 0.1.0 DESCRIPTION "Maverick Game Engine Unit Testing, Ryan Houck 2020-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast -fPIC")

set(LinkLibs
    SDL2
    SDL2_image
    SDL2_mixer
    GL
    gtest
)

set(MaverickPath
    "../../Maverick"
)

#Engine source
set(AnimationSrc
    ${MaverickPath}/Animation/Animation.cpp
)

set(AudioSrc
    ${MaverickPath}/Audio/Audio.cpp
    ${MaverickPath}/Audio/Music/Music.cpp
    ${MaverickPath}/Audio/SoundBoard/SoundBoard.cpp
)

set(FontSrc
    ${MaverickPath}/Font/Font.cpp
)

set(HashSrc
    ${MaverickPath}/HashTable/HashTable.cpp
    ${MaverickPath}/HashTable/AttributeHash/AttributeHash.cpp
    ${MaverickPath}/HashTable/SpriteHash/SpriteHash.cpp
    ${MaverickPath}/HashTable/MusicHash/MusicHash.cpp
    ${MaverickPath}/HashTable/SoundHash/SoundHash.cpp
    ${MaverickPath}/HashTable/FontHash/FontHash.cpp
)

set(HitboxSrc
    ${MaverickPath}/Animation/Hitbox/Hitbox.cpp
    ${MaverickPath}/Animation/Hitbox/HitEllipse/HitEllipse.cpp
    ${MaverickPath}/Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp
    ${MaverickPath}/Animation/Hitbox/HitRect/HitRect.cpp
    ${MaverickPath}/Animation/Hitbox/HitboxCollision.cpp
)

set(CameraSrc
    ${MaverickPath}/Camera/Camera.cpp
)

set(ControlSrc
    ${MaverickPath}/Control/Control.cpp
)

set(UISrc
    ${MaverickPath}/Entity/UI/UIElement.cpp
    ${MaverickPath}/Entity/UI/UIBorders/UIBorders.cpp
    ${MaverickPath}/Entity/UI/UIText/UIText.cpp
    ${MaverickPath}/Entity/UI/UITextBox/UITextBox.cpp
    ${MaverickPath}/Entity/UI/UIObjectFrame/UIObjectFrame.cpp
    ${FontSrc}
)

set(ObjectSrc
    ${MaverickPath}/Entity/Object/Object.cpp
    ${MaverickPath}/Entity/Object/CameraReference/CameraReference.cpp
    ${MaverickPath}/Entity/Object/LoadingZone/LoadingZone.cpp
)

set(EntitySrc
    ${MaverickPath}/Entity/Entity.cpp
    ${ObjectSrc}
    ${UISrc}
)

set(ZoneSrc
    ${MaverickPath}/Zone/Zone.cpp
)

set(ZoneFactorySrc
    ${MaverickPath}/Zone/ZoneFactory/ZoneFactory.cpp
)

set(FactorySrc
    ../../Cyberena/Factory/Factory.cpp
)

set(FileHandlerSrc
    ${MaverickPath}/FileHandler/Saver/Saver.cpp
    ${MaverickPath}/FileHandler/Loader/Loader.cpp
)

set(EngineSrc
    ${MaverickPath}/Engine/Engine.cpp
)

set(MotorSrc
    ${AnimationSrc}
    ${AudioSrc}
    ${HashSrc}
    ${HitboxSrc}
    ${CameraSrc}
    ${ControlSrc}
    ${EntitySrc}
    ${ZoneSrc}
    ${ZoneFactorySrc}
    ${FactorySrc}
    ${FileHandlerSrc}
    ${EngineSrc}
)

#Animation test
#add_executable(TestAnimation
#    ${AnimationSrc}
#    TestAnimation.cpp
#)
#
#target_link_libraries(TestAnimation
#    ${LinkLibs}
#)
#
##Audio test
#add_executable(TestAudio
#    ${MotorSrc}
#    TestAudio.cpp
#)
#
#target_link_libraries(TestAudio
#    ${LinkLibs}
#)
#
##Camera test
#add_executable(TestCamera
#    ${MotorSrc}
#    TestCamera.cpp
#)
#
#target_link_libraries(TestCamera
#    ${LinkLibs}
#)
#
##Control test
#add_executable(TestControl
#    ${MotorSrc}
#    TestControl.cpp
#)
#
#target_link_libraries(TestControl
#    ${LinkLibs}
#)
#
##Engine test
#add_executable(TestEngine
#    ${MotorSrc}
#    TestEngine.cpp
#)
#
#target_link_libraries(TestEngine
#    ${LinkLibs}
#)
#
##File handler test
#add_executable(TestFileHandler
#    ${MotorSrc}
#    TestFileHandler.cpp
#)
#
#target_link_libraries(TestFileHandler
#    ${LinkLibs}
#)

#Globals test
add_executable(TestGlobal
    TestGlobal.cpp
)

target_link_libraries(TestGlobal
    ${LinkLibs}
)

#Object test
#add_executable(TestObject
#    ${MotorSrc}
#    TestObject.cpp
#)
#
#target_link_libraries(TestObject
#    ${LinkLibs}
#)
#
##UIElement test
#add_executable(TestUIElement
#    ${MotorSrc}
#    TestUIElement.cpp
#)
#
#target_link_libraries(TestUIElement
#    ${LinkLibs}
#)
