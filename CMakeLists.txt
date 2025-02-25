cmake_minimum_required(VERSION 3.16)

project(SableEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Vulkan
find_package(Vulkan REQUIRED)

# Include directories for libraries
include_directories("vendor/glm")
include_directories("vendor/stb")
include_directories("vendor/glfw/include")

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# GLFW
add_subdirectory("vendor/glfw")

# ImGui
set(IMGUI_DIR "vendor/imgui")
set(IMGUI_BACKEND_DIR "${IMGUI_DIR}/backends")
include_directories(${IMGUI_DIR} ${IMGUI_BACKEND_DIR})

# Collect ImGui source files
file(GLOB IMGUI_SOURCE "${IMGUI_DIR}/*.cpp" "${IMGUI_DIR}/*.h")

# Collect backend source files
file(GLOB IMGUI_BACKENDS_SOURCE "${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp"
    "${IMGUI_BACKEND_DIR}/imgui_impl_vulkan.cpp")

# Create the ImGui library
add_library(imgui ${IMGUI_SOURCE} ${IMGUI_BACKENDS_SOURCE})

target_link_libraries(imgui Vulkan::Vulkan)

# Sable Engine
add_subdirectory("engine")

# Build the editor executable
add_executable(SableEditor ${SOURCES})

# Link the engine
target_link_libraries(SableEditor SableEngine glfw Vulkan::Vulkan imgui)

target_include_directories(SableEditor PUBLIC "engine/include")

# Copy dependencies to output directory
add_custom_command(
    TARGET SableEditor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw>
            $<TARGET_FILE_DIR:SableEditor>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SableEngine>
            $<TARGET_FILE_DIR:SableEditor>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:SableEditor>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/projects
            $<TARGET_FILE_DIR:SableEditor>/projects)