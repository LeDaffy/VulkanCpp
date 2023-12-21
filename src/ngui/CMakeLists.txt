add_library(ngui)
nce_set_compiler_warnings(ngui)
nce_set_sanitizers(ngui)

target_sources(ngui PRIVATE ngui.cxx)
target_precompile_headers(ngui REUSE_FROM pch)
target_include_directories(ngui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includeVK_CULL_MODE_NONE)
