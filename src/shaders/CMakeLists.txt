make_directory(${CMAKE_BINARY_DIR}/shaders)

add_custom_command(
    OUTPUT 
    ${CMAKE_BINARY_DIR}/shaders/hello.vert.spv
    ${CMAKE_BINARY_DIR}/shaders/hello.frag.spv
    DEPENDS hello.vert hello.frag
    COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/hello.vert -o ${CMAKE_BINARY_DIR}/shaders/hello.vert.spv
    COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/hello.frag -o ${CMAKE_BINARY_DIR}/shaders/hello.frag.spv
    )
add_custom_target(hello_shader DEPENDS
    ${CMAKE_BINARY_DIR}/shaders/hello.vert.spv
    ${CMAKE_BINARY_DIR}/shaders/hello.frag.spv
    )


add_dependencies(ncad hello_shader)
