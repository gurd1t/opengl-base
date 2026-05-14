cmake_minimum_required(VERSION 3.16)

project(opengl_base)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(prog
        src/main.cpp
        src/glad.c
)

target_include_directories(prog PRIVATE include)

target_link_libraries(prog PRIVATE
        glfw
        OpenGL::GL
)