#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

import window;
import types;
import vke;

auto main() -> i32
{
    // auto window = cm::WindowBuilder()
    //                   .with_name("NCAD 3D")
    //                   .with_dimensions(1280, 720)
    //                   .with_position(1280/2, 720/2)
    //                   .build();
    // std::cout << window << std::endl;;
    // vke::Instance vkeinst;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    auto window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (int i = 0; i < glfwExtensionCount; i++) {
        std::cout << glfwExtensions[i] << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    std::cout << "Hello world\n" << std::endl;
    //sleep(3);

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

