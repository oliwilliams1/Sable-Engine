cmake_minimum_required(VERSION 3.16)

project(Sable VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp" "src/*.h")

include_directories("../engine/include" REQUIRED_FILES)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(Sable glfw SableEngine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)