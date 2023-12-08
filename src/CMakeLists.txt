target_sources(ncad PUBLIC main.cxx)
add_subdirectory(nce)
add_subdirectory(shaders)
target_precompile_headers(ncad REUSE_FROM nce)
