module;
#include <string_view>
#include <iostream>
#include <string>
#include <format>
#include <string_view>
#include <ranges>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <optional>
#include <cstdio>
#include <functional>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>

import types;
import carray;
import move_only_ptr;

export module window;


template<typename T> struct WindowVec2 {
    T x;
    T y;
    WindowVec2() : x(0), y(0) {}
    WindowVec2(u32 x, u32 y) : x(x), y(y) {}
};
using CString = const char*;

export namespace cm {
    struct Window {
        MoveOnlyPointer<Display> display;
        MoveOnlyPointer<Screen>  screen;
        ::Window xwindow;

        ~Window() {
            if (display) {
                XCloseDisplay(display);
            }
        }

        // Move Constructor
        Window(Window&& o) = default;
        // Move assignment
        Window& operator=(Window&& o) = default;
        // Copy constructor
        Window(Window& o) = delete;
        // Copy assignment
        Window& operator=(Window& o) = delete;

        friend class WindowBuilder;
        private:
        // Default constructor
        Window()                                 : display(nullptr), screen(nullptr), dimensions(640, 480), position(0, 0), border_width(0), name("X11 Window") {}
        Window(Display* display, Screen* screen) : display(display), screen(screen),  dimensions(640, 480), position(0, 0), border_width(0), name("X11 Window") {}
        WindowVec2<u32> dimensions;
        WindowVec2<u32> position;
        u32 border_width;
        CString name;

    };
    struct WindowBuilder {
        Window win;
        WindowBuilder() { 
            this->win.dimensions.x = 640;
            this->win.dimensions.y = 480;
            this->win.position.x = 0;
            this->win.dimensions.y = 0;
            this->win.border_width = 0;
        };
        auto with_dimensions(u32 x, u32 y) -> WindowBuilder {
            this->win.dimensions = WindowVec2<u32>(640, 480);
            return std::move(*this);
        }
        auto with_name(CString name) -> WindowBuilder {
            this->win.name = name;
            return std::move(*this);
        }
        auto border_width(u32 border_width) -> WindowBuilder {
            this->win.border_width = border_width;
            return std::move(*this);
        }
        auto build() -> Window {
            auto display = XOpenDisplay(nullptr);
            auto screen = XDefaultScreenOfDisplay(display);
            auto screen_id = XDefaultScreen(display);
            auto wclass = InputOutput;


            CArray<i32, i32> depths([display, screen_id]() -> std::tuple<i32*, i32> {
                    auto depths_size = 0;
                    auto depths_array = XListDepths(display, screen_id, &depths_size);
                    return std::make_tuple(depths_array, depths_size);
                    }, XFree);

            i32 depth = *std::ranges::max_element(depths);

            CArray<XVisualInfo, i32> visuals(
                    [display, depth]() -> std::tuple<XVisualInfo*, i32> { 
                    auto count = 0;
                    XVisualInfo wtemplate = 
                    {
                    nullptr, //Visual *visual;
                    0, //VisualID visualid;
                    0, //int screen;
                    depth, //unsigned int depth;
                    0, //int class;
                    0, //unsigned long red_mask;
                    0, //unsigned long green_mask;
                    0, //unsigned long blue_mask;
                    0, //int colormap_size;
                    0 //int bits_per_rgb;
                    };
                    auto visual_array = XGetVisualInfo(display, VisualNoMask | VisualDepthMask, &wtemplate, &count);
                    return std::make_tuple(visual_array, count);
                    }, XFree);

            auto print_xvisual = [](XVisualInfo v) {
                std::printf("{ ");
                std::cout << "visual: " << v.visual << ",  ";
                std::cout << "visualid: " << v.visualid << ",  ";
                std::cout << "screen: " << v.screen << ",  ";
                std::cout << "depth: " << v.depth << ",  ";
                std::cout << "c_class: " << v.c_class << ",  ";
                std::cout << "red_mask: " << v.red_mask << ",  ";
                std::cout << "green_mask: " << v.green_mask << ",  ";
                std::cout << "blue_mask: " << v.blue_mask << ",  ";
                std::cout << "colormap_size: " << v.colormap_size << ",  ";
                std::cout << "bits_per_rgb: " << v.bits_per_rgb << ",  ";
                std::printf("}\n");
            };
            for (auto& x : visuals) {
                //print_xvisual(x);
            }
            auto window = XCreateWindow(display, XDefaultRootWindow(display), this->win.position.x, this->win.position.y, this->win.dimensions.x, this->win.dimensions.y, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0, nullptr);
            XStoreName(display, window, this->win.name);
            XMapWindow(display, window);
            GC gc = XCreateGC(display, window, 0, 0);
            XFlush(display);



            return std::move(Window(display, screen));
        }
        private:
    };
}
