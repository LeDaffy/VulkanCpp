include(FetchContent)
include(ExternalProject)
FetchContent_Declare(
  json
  GIT_REPOSITORY      https://github.com/nlohmann/json.git
)
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.5.0 # or a later release
# )
FetchContent_Declare(
  tiny_obj
  URL https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/release/tiny_obj_loader.h
  DOWNLOAD_NO_EXTRACT TRUE
)

FetchContent_MakeAvailable(tiny_obj)

FetchContent_GetProperties(
  tiny_obj
  SOURCE_DIR TINY_OBJ_SRC_DIR
)


