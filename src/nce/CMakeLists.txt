add_library(cm)
target_compile_definitions(cm
    PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    )

target_sources(cm
    PRIVATE
    window.cxx
    vke.cxx
    )
target_include_directories(cm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cm PUBLIC 
    X11::xcb 
    X11::xcb_keysyms
    X11::xkbcommon
    X11::xkbcommon_X11
    )
