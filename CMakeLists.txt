cmake_minimum_required(VERSION 3.16)

project(SableEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# GLM
add_subdirectory("vendor/glm")
include_directories("vendor/glm")

# Stb Image
set(STB_DIR "vendor/stb")
include_directories(${STB_DIR})

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Sable Engine
add_subdirectory("engine")

# Build the editor executable
add_executable(SableEditor ${SOURCES})

# Link the SableEngine library
target_link_libraries(${PROJECT_NAME} SableEngine)