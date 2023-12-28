add_library(render_api render_api.cxx)
nce_set_compiler_warnings(render_api)
nce_set_sanitizers(render_api)
target_precompile_headers(nce REUSE_FROM pch)

target_include_directories(render_api PUBLIC include)
