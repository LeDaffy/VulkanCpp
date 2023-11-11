cmake_minimum_required(VERSION 3.28)
# Turning off extensions avoids and issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wall")
#add_compile_options (-fdiagnostics-color=always)
add_compile_options (-fcolor-diagnostics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cgm)


find_package(X11 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
add_executable(ncad)

add_subdirectory(src)

target_link_libraries(ncad PUBLIC cm Vulkan::Vulkan glfw)
