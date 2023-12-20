cmake_minimum_required(VERSION 3.27)
project(N3Dx)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(compiler_warnings)
include(sanitizers)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



find_package(X11 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(ncad)
nce_set_compiler_warnings(ncad)
nce_set_sanitizers(ncad)

add_custom_command(TARGET ncad 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ncad> ${CMAKE_BINARY_DIR})

add_subdirectory(src)

target_link_libraries(ncad nce Vulkan::Vulkan fmt)
