cmake_minimum_required(VERSION 3.16)

project(SableEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Vulkan
find_package(Vulkan REQUIRED)

# GLM
add_subdirectory("vendor/glm")
include_directories("vendor/glm")

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
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} yaml-cpp Vulkan::Vulkan)

# Copy dependencies' .lib files to the output directory
foreach(dep IN LISTS GLEW_LIBRARIES GLFW_LIBRARIES ASSIMP_LIBRARIES)
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${dep}>
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()

# Gather shaders for copying
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
file(GLOB SHADERS "${SHADER_DIR}/*.spv")

# Custom command to copy shaders to the build output and editor source directory for development
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SHADERS}
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SHADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders/
    DEPENDS ${SHADERS}
    COMMENT "Copying shaders to build output and parent directory"
)

# Ensure shaders are copied
add_custom_target(copy_shaders_engine ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_dependencies(SableEngine copy_shaders_engine)