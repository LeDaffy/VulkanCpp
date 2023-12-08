#pragma once
#include <cstdlib>
#include <cstdio>

#define VKE_RESULT_CRASH(_result) if ((_result) != VK_SUCCESS) { fmt::println("VkResult Error ({}, {}, {}): {}\n", __FILE__, __func__, __LINE__, static_cast<CString>((_result))); exit(EXIT_FAILURE); }
#ifdef DEBUG
    [[maybe_unused]] constexpr bool enable_validation_layers = true;
#else
    [[maybe_unused]] constexpr bool enable_validation_layers = false;
#endif
