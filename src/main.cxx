#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds


// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>
#include <keycode.hxx>

import window;
import types;
import vke;

auto main() -> i32
{
    auto xwindow = window::WindowBuilder()
                       .with_name("NCAD 3D")
                       .with_dimensions(1280, 720)
                       .with_position(1280/2, 720/2)
                       .build();
    vke::Instance vkeinst;

    while (!xwindow.should_close()) {
        xwindow.poll_events();
        if (xwindow.keys.is_pressed(KeyCode::space)) {
            std::cout << "Pressed space" << std::endl;
        }
        if (xwindow.keys.is_pressed(KeyCode::q)) {
            std::cout << "q pressed" << std::endl;
            break;
        }
        if (xwindow.keys.is_pressed(KeyCode::a)) {
            std::cout << "a pressed" << std::endl;
        }
        if (xwindow.keys.is_pressed(KeyCode::s)) {
            std::cout << "s pressed" << std::endl;
        }
        if (xwindow.keys.is_pressed(KeyCode::d)) {
            std::cout << "d pressed" << std::endl;
        }
        if (xwindow.keys.is_pressed(KeyCode::f)) {
            std::cout << "f pressed" << std::endl;
        }
        if (xwindow.keys.is_down(KeyCode::w)) {
            std::cout << "w held" << std::endl;
        }
        if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_pressed(KeyCode::c)) {
            std::cout << "Ctrl-c pressed" << std::endl;
            break;
        }

    }
    return 0;
}

