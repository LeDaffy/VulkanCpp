function(nce_set_sanitizers
        TARGET)
    target_compile_options(${TARGET} INTERFACE $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=leak -fsanitize=undefined>)
    target_link_options(${TARGET} INTERFACE $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=leak -fsanitize=undefined>)
endfunction()
