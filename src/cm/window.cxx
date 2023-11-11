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
#include <xcb/xcb.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vulkan_xcb.h>



import types;
import carray;
import move_only_ptr;

export module window;


template<typename T> struct WindowVec2 {
    T x;
    T y;
    WindowVec2() : x(0), y(0) {}
    WindowVec2(u32 x, u32 y) : x(x), y(y) {}
    friend std::ostream &operator<<(std::ostream &os, WindowVec2<T>& v) {
        return os << "[" << v.x << ", " << v.y << "]";
    }
};

export namespace cm {
    struct Window {
        ~Window() {
            if (display) {
                std::cout << "Destroying " << *this << std::endl;
                XUnmapWindow(display, xwindow);
                XDestroyWindow(display, xwindow);
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

        friend struct WindowBuilder;
        friend std::ostream &operator<<(std::ostream &os, Window& w);

        // members 
        MoveOnlyPointer<Display> display;
        MoveOnlyPointer<Screen>  screen;
        i32 screen_id;
        ::Window xwindow;
        private:
        // Default constructor
        Window() : display(nullptr), screen(nullptr), screen_id(0), xwindow(0), dimensions(640, 480), position(0, 0), border_width(0), name("X11 Window"), graphics_context(nullptr)  {}
        WindowVec2<u32> dimensions;
        WindowVec2<u32> position;
        u32 border_width;
        CString name;
        GC graphics_context;
    };
    std::ostream &operator<<(std::ostream &os, Window& w) {
        return os << "Window (" << &w << "): { "
            << "display: " << w.display << ",  "
            << "screen: " << w.screen << ",  "
            << "xwindow: " << w.xwindow << ",  "
            << "dimensions: " << w.dimensions << ",  "
            << "position: " << w.position << ",  "
            << "border_width: " << w.border_width << ",  "
            << "name: " << w.name << ",  "
            << "graphics_context: " << w.graphics_context << " }";
    }
    struct WindowBuilder {
        Window win;
        WindowBuilder() { 
            this->win.dimensions.x = 640;
            this->win.dimensions.y = 480;
            this->win.position.x = 0;
            this->win.position.y = 0;
            this->win.border_width = 0;
        };
        static auto create() -> WindowBuilder {
            WindowBuilder builder;
            return builder;
        }
        auto with_dimensions(u32 x, u32 y) -> WindowBuilder& {
            this->win.dimensions = WindowVec2<u32>(x, y);
            return *this;
        }
        auto with_position(u32 x, u32 y) -> WindowBuilder& {
            this->win.position = WindowVec2<u32>(x, y);
            return *this;
        }
        auto with_name(CString name) -> WindowBuilder& {
            this->win.name = name;
            return *this;
        }
        auto border_width(u32 border_width) -> WindowBuilder& {
            this->win.border_width = border_width;
            return *this;
        }
        auto build() -> Window {
            this->win.display = XOpenDisplay(nullptr);
            this->win.screen = XDefaultScreenOfDisplay(this->win.display);
            this->win.screen_id = XDefaultScreen(this->win.display);


            CArray<i32, i32, int(*)(void*)> depths([&]() -> std::tuple<i32*, i32> {
                    auto depths_size = 0;
                    auto depths_array = XListDepths(this->win.display, this->win.screen_id, &depths_size);
                    return std::make_tuple(depths_array, depths_size);
                    }, XFree);

            i32 depth = *std::ranges::max_element(depths);

            CArray<XVisualInfo, i32, int(*)(void*)> visuals(
                    [&]() -> std::tuple<XVisualInfo*, i32> { 
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
                    auto visual_array = XGetVisualInfo(this->win.display, VisualNoMask | VisualDepthMask, &wtemplate, &count);
                    return std::make_tuple(visual_array, count);
                    }, XFree);

            [[maybe_unused]] auto print_xvisual = [](XVisualInfo v) {
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
            this->win.xwindow = XCreateWindow(this->win.display, XDefaultRootWindow(this->win.display), this->win.position.x, this->win.position.y, this->win.dimensions.x, this->win.dimensions.y, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0, nullptr);
            XStoreName(this->win.display, this->win.xwindow, this->win.name);
            XMapWindow(this->win.display, this->win.xwindow);
            XGCValues gc_defaults = {};
            gc_defaults.foreground = XWhitePixel(this->win.display, this->win.screen_id);
            gc_defaults.background = XBlackPixel(this->win.display, this->win.screen_id);
            this->win.graphics_context = XCreateGC(this->win.display, this->win.xwindow, GCForeground | GCBackground, &gc_defaults);
            XFlush(this->win.display);



            return std::move(this->win);
        }
        private:
    };
}
