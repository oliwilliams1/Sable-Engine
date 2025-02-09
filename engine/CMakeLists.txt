cmake_minimum_required(VERSION 3.16)

project(SableEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

# GLM
add_subdirectory("../vendor/glm" "${CMAKE_BINARY_DIR}/glm")
include_directories("../vendor/glm")

# GLEW
add_subdirectory("../vendor/glew/build/cmake" "${CMAKE_BINARY_DIR}/glew")
include_directories("../vendor/glew/include")

# GLFW
add_subdirectory("../vendor/glfw" "${CMAKE_BINARY_DIR}/glfw")

# Stb Image
set(STB_DIR "../vendor/stb")
include_directories(${STB_DIR})

# Assimp
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable building all exporters by default")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable assimp install") # using as submodule
add_subdirectory("../vendor/assimp" "${CMAKE_BINARY_DIR}/assimp")

# Include source files from src and headers from include
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

# Build SableEngine as a shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# Copy shared libraries to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:glew>"
    "$<TARGET_FILE:glfw>"
    "$<TARGET_FILE:assimp>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)