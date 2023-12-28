add_library(ngui)
nce_set_compiler_warnings(ngui)
nce_set_sanitizers(ngui)

target_sources(ngui PRIVATE btree.cxx frame.cxx)
target_precompile_headers(ngui REUSE_FROM pch)
target_include_directories(ngui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(btree_test btree_test.cxx)
add_test(NAME btree_tester COMMAND btree_test)
target_link_libraries(btree_test PRIVATE Catch2::Catch2WithMain ngui fmt)
catch_discover_tests(btree_test)
nce_set_compiler_warnings(btree_test)
nce_set_sanitizers(btree_test)

target_precompile_headers(btree_test REUSE_FROM pch)
target_precompile_headers(ngui REUSE_FROM pch)
