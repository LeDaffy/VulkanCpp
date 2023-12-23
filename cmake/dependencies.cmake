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

FetchContent_MakeAvailable(json)



