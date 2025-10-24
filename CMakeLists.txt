cmake_minimum_required(VERSION 3.16)
project(chromasnake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics)

add_executable(chromasnake
    App.cpp
    Grid.cpp
    Food.cpp
    Snake.cpp
    Game.cpp
)

target_include_directories(chromasnake PRIVATE src)
target_link_libraries(chromasnake PRIVATE sfml-system sfml-window sfml-graphics)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(chromasnake PRIVATE -Wall -Wextra -Wconversion -Wshadow -Wpedantic)
endif()
