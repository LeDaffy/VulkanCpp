add_library(stb_image stb_image.cxx)
add_library(tiny_obj tiny_obj.cxx)
target_include_directories(tiny_obj PUBLIC ${TINY_OBJ_SRC_DIR})
