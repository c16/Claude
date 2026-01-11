cmake_minimum_required(VERSION 3.10)
project(DnD1993 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/dice.cpp
    src/character.cpp
    src/combat.cpp
    src/inventory.cpp
    src/adventure.cpp
    src/game.cpp
    src/main.cpp
)

# Executable
add_executable(dnd_1993 ${SOURCES})

# Installation
install(TARGETS dnd_1993 DESTINATION bin)
install(DIRECTORY data/ DESTINATION share/dnd_1993/data)
