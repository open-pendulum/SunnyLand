cmake_minimum_required(VERSION 3.20.0)
project(SunnyLand)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

include(cmake/compiler_settings.cmake)
include(cmake/dependencies.cmake)
include(cmake/runtime_path.cmake)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(glm REQUIRED)
# find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCES
        src/main.cpp
        src/engine/core/game_app.h
        src/engine/core/game_app.cpp
        src/engine/core/time.h
        src/engine/core/time.cpp
        src/engine/core/config.h
        src/engine/core/config.cpp
        src/engine/core/context.h
        src/engine/core/context.cpp
        src/common/logger.hpp
        src/engine/utils/math.hpp
        src/engine/utils/alignment.h
        src/engine/resource/resource_manager.h
        src/engine/resource/resource_manager.cpp
        src/engine/resource/audio_manager.h
        src/engine/resource/audio_manager.cpp
        src/engine/resource/texture_manager.h
        src/engine/resource/texture_manager.cpp
        src/engine/resource/font_manager.h
        src/engine/resource/font_manager.cpp
        src/engine/render/renderer.h
        src/engine/render/renderer.cpp
        src/engine/render/camera.h
        src/engine/render/camera.cpp
        src/engine/render/sprite.h
        src/engine/render/sprite.cpp
        src/engine/input/input_manager.h
        src/engine/input/input_manager.cpp
        src/engine/component/component.h
        src/engine/component/transform_component.h
        src/engine/component/transform_component.cpp
        src/engine/component/sprite_component.h
        src/engine/component/sprite_component.cpp
        src/engine/object/game_object.h
        src/engine/object/game_object.cpp
        src/engine/scene/scene.h
        src/engine/scene/scene.cpp
        src/engine/scene/scene_manager.h
        src/engine/scene/scene_manager.cpp

        src/game/scene/game_scene.h
        src/game/scene/game_scene.cpp
)

add_subdirectory(third_party)

add_executable(${TARGET}
        ${SOURCES}
)

target_include_directories(${TARGET} PRIVATE
        src/
        src/common
        src/engine
        src/game
)


target_link_libraries(${TARGET} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
        glm::glm
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 针对 MSVC（Visual Studio 编译器）：用 MSVC 支持的警告等级 /W4（等价于 GCC 的 -Wall -Wextra）
if(MSVC)
        target_compile_options(${TARGET} PRIVATE "/Zc:preprocessor")  # 关键：启用新预处理，支持 __VA_OPT__
        target_compile_options(${TARGET} PRIVATE "/W4" "/wd4100" "/wd4267")  
endif()