add_library(render_api 
    instance.cxx
    surface.cxx
    )
nce_set_compiler_warnings(render_api)
nce_set_sanitizers(render_api)
target_include_directories(render_api PUBLIC include)
target_precompile_headers(render_api REUSE_FROM pch)
target_link_libraries(render_api nce)

