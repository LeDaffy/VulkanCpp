file(WRITE ${CMAKE_BINARY_DIR}/null.cxx "")
add_library(pch ${CMAKE_BINARY_DIR}/null.cxx)
nce_set_compiler_warnings(pch)
nce_set_sanitizers(pch)
target_precompile_headers(pch
    PRIVATE
    <algorithm>
    <any>
    <array>
    <bitset>
    <cstddef>
    <cstdint>
    <cstdio>
    <filesystem>
    <fmt/format.h>
    <format>
    <fstream>
    <functional>
    <iostream>
    <limits>
    <map>
    <memory>
    <optional>
    <stack>
    <ranges>
    <set>
    <string>
    <string_view>
    <tuple>
    <unistd.h>
    <unordered_map>
    <vector>
    )