cmake_minimum_required(VERSION 3.9)
project(Nephesh)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
link_libraries(SDL2 SDL2_image SDL2_ttf SDL2_mixer)
include_directories(/usr/include/SDL2)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

include_directories(BEFORE include)

add_executable(Nephesh src/Game.cpp
        include/Game.h
        src/Log.cpp
        include/Log.h
        src/Main.cpp
        src/Music.cpp
        include/Music.h
        src/Sprite.cpp
        include/Sprite.h
        src/Vec2.cpp include/Vec2.h src/Rect.cpp include/Rect.h src/Component.cpp include/Component.h src/GameObject.cpp include/GameObject.h src/Sound.cpp include/Sound.h include/Component.h src/TileSet.cpp include/TileSet.h src/TileMap.cpp include/TileMap.h src/Resources.cpp include/Resources.h src/InputManager.cpp include/InputManager.h src/Camera.cpp include/Camera.h src/CameraFollower.cpp include/CameraFollower.h src/Collider.cpp include/Collider.h include/Collision.h src/Timer.cpp include/Timer.h src/State.cpp include/State.h src/Text.cpp include/Text.h src/IntervalTimer.cpp include/IntervalTimer.h include/TitleState.h src/TitleState.cpp src/BeamSkill.cpp include/BeamSkill.h)
