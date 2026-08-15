cmake_minimum_required(VERSION 3.28)

# Prefer g++ when the caller has not chosen a compiler. Some images point
# /usr/bin/c++ at clang++ with a GCC libstdc++ that is not installed.
if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    find_program(QUADTREE_GXX NAMES g++)
    if(QUADTREE_GXX)
        set(CMAKE_CXX_COMPILER "${QUADTREE_GXX}")
    endif()
endif()

project(quadtree LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# This demo only draws 2D shapes, so skip the unused SFML modules.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(quadtree main.cpp)
target_compile_features(quadtree PRIVATE cxx_std_17)
target_link_libraries(quadtree PRIVATE SFML::Graphics)
