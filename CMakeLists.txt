cmake_minimum_required(VERSION 3.16)

project(SableEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for GLM and Stb Image
include_directories("vendor/glm")
include_directories("vendor/stb")

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Sable Engine
add_subdirectory("engine")
include_directories("engine/include")

# Build the editor executable
add_executable(SableEditor ${SOURCES})

# Link the SableEngine library
target_link_libraries(SableEditor PRIVATE SableEngine)