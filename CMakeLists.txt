cmake_minimum_required(VERSION 3.28)
# Turning off extensions avoids and issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-fdiagnostics-color=always)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cgm)


find_package(X11 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
add_executable(ncad)

add_subdirectory(src)

target_link_libraries(ncad PUBLIC nce Vulkan::Vulkan glfw)
install(FILES
    ${CMAKE_SOURCE_DIR}/build/compile_commands.json
    DESTINATION ${CMAKE_SOURCE_DIR}
    )
