add_library(cm)

target_sources(cm
    PUBLIC
    FILE_SET CXX_MODULES FILES
    window.cxx
    types.cxx
    )

target_link_libraries(cm PUBLIC ${X11_LIBRARIES})
