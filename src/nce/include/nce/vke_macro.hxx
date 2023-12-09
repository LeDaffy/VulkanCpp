#pragma once
#include <cstdlib>
#include <cstdio>

#define VKE_RESULT_CRASH(_result) if ((_result) != VK_SUCCESS) { fmt::println("VkResult Error ({}, {}, {}): {}\n", __FILE__, __func__, __LINE__, static_cast<CString>((_result))); exit(EXIT_FAILURE); }
