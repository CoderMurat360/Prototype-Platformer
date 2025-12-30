cmake_minimum_required(VERSION 3.20)
project(RaylibGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE GAME_SOURCES
    src/*.cpp
)

add_executable(game src/main.cpp)
target_sources(game PRIVATE ${GAME_SOURCES})

find_package(raylib CONFIG REQUIRED)
target_link_libraries(game PRIVATE raylib)

if (WIN32)
    target_link_libraries(game PRIVATE winmm)
endif()
