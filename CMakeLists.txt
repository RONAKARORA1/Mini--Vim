cmake_minimum_required(VERSION 3.15)

# Project Name and Version
project(MiniVim VERSION 1.0 LANGUAGES CXX)

# Enforce Modern C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the executable and its source files
add_executable(mini_vim
    src/main.cpp
    src/terminal.cpp
    src/buffer.cpp
    src/renderer.cpp
    src/editor.cpp
)

# Add Include Directories
target_include_directories(mini_vim PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Apply strict compiler warnings for production-grade code
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mini_vim PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        -O3 
    )
endif()
