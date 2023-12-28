add_library(nce)
nce_set_compiler_warnings(nce)
nce_set_sanitizers(nce)

target_sources(nce
    PRIVATE
    window.cxx
    vke.cxx
    )
target_include_directories(nce PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(nce 
    Vulkan::Vulkan
    fmt
    X11::xcb 
    X11::xcb_keysyms
    X11::xkbcommon
    X11::xkbcommon_X11
    X11::xcb_icccm
    stb_image
    tiny_obj
    )
target_precompile_headers(nce REUSE_FROM pch)
