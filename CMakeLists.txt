cmake_minimum_required(VERSION 3.16)

project(SableEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
)