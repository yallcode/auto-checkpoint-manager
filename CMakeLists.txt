cmake_minimum_required(VERSION 3.20)
project(AutoCheckpointManager VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

add_geode_mod(
    AutoCheckpointManager
    SOURCES
        src/main.cpp
)