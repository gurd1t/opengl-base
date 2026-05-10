cmake_minimum_required(VERSION 3.16)

project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create executable
add_executable(prog src/main.cpp src/glad.c)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(prog
  glfw
  OpenGL::GL
)
