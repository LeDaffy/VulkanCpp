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
#include <memory>

#include <xcb/xcb.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>



import types;
import carray;
import move_only_ptr;
import non_owning_ptr;
export module window;

struct XCBConnectionDeleter {
    void operator()(xcb_connection_t* ptr){ std::cout << "XCBConnectionDeleter()" << std::endl; xcb_disconnect(ptr); }
};
namespace window {

    template<typename T> struct WindowVec2 {
        T x;
        T y;
        WindowVec2() : x(0), y(0) {}
        WindowVec2(u32 x, u32 y) : x(x), y(y) {}
        friend std::ostream &operator<<(std::ostream &os, WindowVec2<T>& v) {
            return os << "[" << v.x << ", " << v.y << "]";
        }
    };

    struct Attributes {
        WindowVec2<u32> dimensions;
        WindowVec2<u32> position;
        CString name;
        Attributes() :
            dimensions(640, 480), position(0, 0), name("XCB Window") {}
        Attributes(WindowVec2<u32> dimensions, WindowVec2<u32> position, CString name) :
            dimensions(dimensions), position(position), name(name) {}
    };
    export struct Window {
        Attributes attributes;
        std::unique_ptr<xcb_connection_t, XCBConnectionDeleter> x_connection;
        ~Window() = default;
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
        private:
        Window() {}
        Window(Attributes attributes, std::unique_ptr<xcb_connection_t, XCBConnectionDeleter>&& x_connection) : attributes(attributes), x_connection(std::move(x_connection)) {}
    };
    std::ostream &operator<<(std::ostream &os, Window& w) {
        return os << "Window (" << &w << "): { " << "asdf";
    }
    export struct WindowBuilder {
        Attributes attributes;
        WindowBuilder() {};
        auto with_dimensions(u32 x, u32 y) -> WindowBuilder& {
            attributes.dimensions = WindowVec2<u32>(x, y);
            return *this;
        }
        auto with_position(u32 x, u32 y) -> WindowBuilder& {
            attributes.position = WindowVec2<u32>(x, y);
            return *this;
        }
        auto with_name(CString name) -> WindowBuilder& {
            attributes.name = name;
            return *this;
        }
        auto build() -> Window {
            /* Open the connection to the X server */
            std::unique_ptr<xcb_connection_t, XCBConnectionDeleter> x_connection(xcb_connect (nullptr, nullptr));

            NonOwningPtr<const xcb_setup_t> x_setup = xcb_get_setup(x_connection.get());
            xcb_screen_iterator_t x_iter            = xcb_setup_roots_iterator (x_setup);
            NonOwningPtr<xcb_screen_t> x_screen     = x_iter.data;
            xcb_window_t window = xcb_generate_id(x_connection.get());

            xcb_create_window (x_connection.get(),            // Connection          
                               XCB_COPY_FROM_PARENT,          // depth (same as root)
                               window,                        // window Id           
                               x_screen->root,                // parent window       
                               0, 0,                          // x, y                
                               150, 150,                      // width, height       
                               10,                            // border_width        
                               XCB_WINDOW_CLASS_INPUT_OUTPUT, // class               
                               x_screen->root_visual,         // visual              
                               0, NULL );                     // masks, not used yet 
             /* Map the window on the screen */
            xcb_map_window(x_connection.get(), window);
            xcb_flush(x_connection.get());


            return Window(attributes, std::move(x_connection));
        }
        private:
    };
}
