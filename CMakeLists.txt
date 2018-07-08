cmake_minimum_required(VERSION 3.9)
project(Nephesh)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if (WIN32)
    include_directories(C:/SDL2-2.0.8/include)
    link_directories(C:/SDL2-2.0.8/lib)
    link_libraries(Mingw32 SDL2main SDL2_image SDL2_ttf SDL2_mixer SDL2)
else()
    link_libraries(SDL2 SDL2_image SDL2_ttf SDL2_mixer)
    include_directories(/usr/include/SDL2)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
endif ()

include_directories(BEFORE include)

include_directories(BEFORE tests)

add_executable(Nephesh src/Main.cpp
        src/Game.cpp
        include/Game.h
        src/Log.cpp
        include/Log.h
        src/Music.cpp
        include/Music.h
        src/Sprite.cpp
        include/Sprite.h
        src/Vec2.cpp
        include/Vec2.h
        src/Rect.cpp
        include/Rect.h
        src/Component.cpp
        include/Component.h
        src/GameObject.cpp
        include/GameObject.h
        src/Sound.cpp
        include/Sound.h
        src/TileSet.cpp
        include/TileSet.h
        src/TileMap.cpp
        include/TileMap.h
        src/Resources.cpp
        include/Resources.h
        src/InputManager.cpp
        include/InputManager.h
        src/Camera.cpp
        include/Camera.h
        src/CameraFollower.cpp
        include/CameraFollower.h
        src/Collider.cpp
        include/Collider.h
        src/Timer.cpp
        include/Timer.h
        src/State.cpp
        include/State.h
        src/Text.cpp
        include/Text.h
        src/IntervalTimer.cpp
        include/IntervalTimer.h
        include/WorldState.h
        src/WorldState.cpp
        src/CollisionMap.cpp
        include/CollisionMap.h
        src/Collidable.cpp
        include/Collidable.h
        src/GameData.cpp
        include/GameData.h
        tests/StageState.cpp
        tests/StageState.h
        tests/PenguinBody.cpp
        tests/PenguinBody.h
        tests/PenguinCannon.cpp
        tests/PenguinCannon.h
        src/BeamSkill.cpp
        include/BeamSkill.h
        src/LineSegment.cpp
        include/LineSegment.h
        src/Player.cpp
        include/Player.h
        src/Boss.cpp
        include/Boss.h
        src/MeleeAttack.cpp
        include/MeleeAttack.h
        src/Npc.cpp
        include/Npc.h
        src/TextBox.cpp
        include/TextBox.h
        src/Map.cpp
        include/Map.h
        src/TitleState.cpp
        include/TitleState.h
        src/MeleeAttack.cpp include/MeleeAttack.h src/Npc.cpp include/Npc.h src/TextBox.cpp include/TextBox.h src/Map.cpp include/Map.h src/TerrainMap.cpp include/TerrainMap.h src/FadeEffect.cpp include/FadeEffect.h src/RollingRocks.cpp include/RollingRocks.h
        src/Charge.cpp include/Charge.h src/Intersection.cpp include/Intersection.h src/Bar.cpp include/Bar.h)