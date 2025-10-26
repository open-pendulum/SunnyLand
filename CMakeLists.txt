cmake_minimum_required(VERSION 4.0.0)
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
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCES
        src/main.cpp
        src/engine/core/game_app.h
        src/engine/core/game_app.cpp
        src/engine/core/time.h
        src/engine/core/time.cpp
        src/utils/logger.hpp
        src/engine/resource/resource_manager.h
        src/engine/resource/resource_manager.cpp
        src/engine/resource/audio_manager.h
        src/engine/resource/audio_manager.cpp
        src/engine/resource/texture_manager.h
        src/engine/resource/texture_manager.cpp
        src/engine/resource/font_manager.h
        src/engine/resource/font_manager.cpp
)

add_subdirectory(third_party)

add_executable(${TARGET}
        ${SOURCES}
)

target_include_directories(${TARGET} PRIVATE
        src/utils
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

target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
