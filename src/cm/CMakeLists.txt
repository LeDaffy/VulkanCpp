add_library(cm)
target_compile_definitions(cm
    PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    )

target_sources(cm
    PUBLIC
    FILE_SET CXX_MODULES FILES
    window.cxx
    types.cxx
    carray.cxx
    move_only_ptr.cxx
    non_owning_ptr.cxx
    vke.cxx
    )
target_include_directories(cm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cm PUBLIC 
    X11::xcb 
    X11::xcb_keysyms
    X11::xkbcommon
    X11::xkbcommon_X11
    )
