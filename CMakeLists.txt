cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch Geode CMake utils
FetchContent_Declare(
    Geode
    GIT_REPOSITORY https://github.com/geode-sdk/geode.git
    GIT_TAG main
)
FetchContent_MakeAvailable(Geode)

project(AutoCheckpointManager VERSION 1.0.0)

# Include main source files
add_geode_mod(
    AutoCheckpointManager
    SOURCES
        src/main.cpp
)
