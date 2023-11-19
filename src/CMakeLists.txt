target_sources(ncad PUBLIC main.cxx)
add_subdirectory(nce)
target_precompile_headers(ncad REUSE_FROM nce)
