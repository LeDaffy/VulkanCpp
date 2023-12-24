#pragma once
#include <cstdlib>

#define todo(_msg) fmt::println("todo({}, {}, {}): {}\n", __FILE__, __func__, __LINE__, (_msg)); exit(EXIT_FAILURE);
