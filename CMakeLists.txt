cmake_minimum_required(VERSION 3.25)
project(Blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(blocks 
    src/main.cpp
    src/SceneObject.cpp
    src/Camera.cpp
    )
