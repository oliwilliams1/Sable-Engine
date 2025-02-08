cmake_minimum_required(VERSION 3.16)

project(SableEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

# GLM
add_subdirectory("vendor/glm")
include_directories("vendor/glm")

# GLEW
add_subdirectory("vendor/glew/build/cmake")
include_directories("vendor/glew/include")
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# GLFW
add_subdirectory("vendor/glfw")

# ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")
set(IMGUI_BACKEND_DIR "${IMGUI_DIR}/backends")
include_directories(${IMGUI_DIR} ${IMGUI_BACKEND_DIR})

file(GLOB IMGUI_SOURCE "${IMGUI_DIR}/*.cpp" "${IMGUI_DIR}/*.h")
file(GLOB IMGUI_BACKENDS_SOURCE "${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp" "${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp")
    
add_library(imgui ${IMGUI_SOURCE} ${IMGUI_BACKENDS_SOURCE})

# ImGui File Dialog
set(IMGUI_FILE_DIALOG_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui-file-dialog")
include_directories(${IMGUI_FILE_DIALOG_DIR})
file(GLOB IMGUI_FILE_DIALOG_SOURCE "${IMGUI_FILE_DIALOG_DIR}/*.cpp" "${IMGUI_FILE_DIALOG_DIR}/*.h")

add_library(imguiFileDialog ${IMGUI_FILE_DIALOG_SOURCE})

# Stb Image
set(STB_DIR "${CMAKE_SOURCE_DIR}/vendor/stb")
include_directories(${STB_DIR})

set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable building all exporters by default")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable assimp install") # using as submodule

add_subdirectory("vendor/assimp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
)

# Copy shared libraries to the executable directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:glew>"
    "$<TARGET_FILE:glfw>"
    "$<TARGET_FILE:assimp>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)