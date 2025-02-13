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

# Stb Image
set(STB_DIR "vendor/stb")
include_directories(${STB_DIR})

# Assimp
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT
	OFF
	CACHE BOOL "Disable building all exporters by default")
set(ASSIMP_INSTALL
	OFF
	CACHE BOOL "Disable assimp install") # using as submodule
add_subdirectory("vendor/assimp")

include_directories("include/SB")
# Include source files from src and headers from include
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/SB/*.h")

# Build SableEngine as a shared library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Yaml
add_subdirectory("vendor/yaml")
include_directories("vendor/yaml/include")

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew yaml-cpp)

# Copy dependencies' .lib files to the output directory
foreach(dep IN LISTS GLEW_LIBRARIES GLFW_LIBRARIES ASSIMP_LIBRARIES)
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${dep}>
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()

# Copy GLEW DLL to output directory
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glew>
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)