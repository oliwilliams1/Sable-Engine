cmake_minimum_required(VERSION 3.16)

project(SableEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for GLM, Stb Image, and GLEW
include_directories("vendor/glm")
include_directories("vendor/stb")
include_directories("vendor/glew/include")

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# GLFW
add_subdirectory("vendor/glfw")

# Sable Engine
add_subdirectory("engine")
include_directories("engine/include")

# Build the editor executable
add_executable(SableEditor ${SOURCES})

# Link the SableEngine library
target_link_libraries(SableEditor 
	SableEngine 
	glfw
	glew
)

# Copy DLLs to output directory
add_custom_command(TARGET SableEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glew> $<TARGET_FILE_DIR:SableEditor>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:SableEditor>)