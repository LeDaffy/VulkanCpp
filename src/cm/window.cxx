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
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>


#define CRASH(msg) std::cout << (msg) << std::endl; std::abort();

import types;
import carray;
import move_only_ptr;
import non_owning_ptr;
export module window;

struct XCBConnectionDeleter {
    void operator()(xcb_connection_t* ptr){ std::cout << "XCBConnectionDeleter()" << std::endl; xcb_disconnect(ptr); }
};
struct XCBKeySymDeleter {
    void operator()(xcb_key_symbols_t* ptr){ std::cout << "XCBKeySymDeleter()" << std::endl; xcb_key_symbols_free(ptr); }
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
        std::unique_ptr<xcb_key_symbols_t, XCBKeySymDeleter> x_keys;
        xcb_window_t x_window;
        NonOwningPtr<xkb_state> kb_state;
        NonOwningPtr<xcb_generic_event_t> x_event;

        ~Window() = default;
        // Move Constructor
        Window(Window&& o) = default;
        // Move assignment
        Window& operator=(Window&& o) = default;
        // Copy constructor
        Window(Window& o) = delete;
        // Copy assignment
        Window& operator=(Window& o) = delete;

        // methods
        void poll_events() {
            std::unique_ptr<xcb_generic_event_t, CFreeDeleter> x_event(xcb_poll_for_event(x_connection.get()));
            if (!x_event) return;

            switch (x_event->response_type & ~0x80) {
                case XCB_KEY_PRESS: {
                    // std::cout << "XCB_KEY_PRESS" << std::endl;
                    [[maybe_unused]] NonOwningPtr<xcb_key_press_event_t> event = reinterpret_cast<xcb_key_press_event_t*>(x_event.get());
                    xkb_keysym_t keysym = xkb_state_key_get_one_sym(kb_state, event->detail);
                    char keysym_name[64];
                    xkb_keysym_get_name(keysym, keysym_name, sizeof(keysym_name));
                    std::cout << keysym_name << ": " << keysym << std::endl;

                    break;
                } case XCB_KEY_RELEASE: {
                    std::cout << "XCB_KEY_RELEASE" << std::endl;
                    [[maybe_unused]] NonOwningPtr<xcb_key_release_event_t> event = reinterpret_cast<xcb_key_release_event_t*>(x_event.get());
                    break;
                } case XCB_BUTTON_PRESS: {
                    std::cout << "XCB_BUTTON_PRESS" << std::endl;
                    [[maybe_unused]] NonOwningPtr<xcb_button_press_event_t> event = reinterpret_cast<xcb_button_press_event_t*>(x_event.get());
                    break;
                } case XCB_BUTTON_RELEASE: {
                    std::cout << "XCB_BUTTON_RELEASE" << std::endl;
                    [[maybe_unused]] NonOwningPtr<xcb_button_release_event_t> event = reinterpret_cast<xcb_button_release_event_t*>(x_event.get());
                    break;
                }
            }
        }
        bool should_close() {
            return false;
        }

        friend struct WindowBuilder;
        friend std::ostream &operator<<(std::ostream &os, Window& w);
        private:
        Window() {}
        Window(Attributes attributes, std::unique_ptr<xcb_connection_t, XCBConnectionDeleter>&& x_connection, xcb_window_t x_window, NonOwningPtr<xkb_state> kb_state) 
            : attributes(attributes), x_connection(std::move(x_connection)), 
            x_keys(xcb_key_symbols_alloc(x_connection.get())),
            x_window(x_window),
            kb_state(kb_state)
        {}
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

            // keyboard setup
            NonOwningPtr<xkb_context> kb_ktx(xkb_context_new(XKB_CONTEXT_NO_FLAGS));
            if (!kb_ktx) {
                CRASH("Couldn't create keyboard context");
            }
            xkb_x11_setup_xkb_extension(x_connection.get(), XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, nullptr,  nullptr,  nullptr,  nullptr);

            i32 kb_device_id = xkb_x11_get_core_keyboard_device_id(x_connection.get());
            if (kb_device_id == -1) { CRASH("Couldn't get kb device id"); }

            NonOwningPtr<xkb_keymap> keymap = xkb_x11_keymap_new_from_device(kb_ktx, x_connection.get(), kb_device_id, XKB_KEYMAP_COMPILE_NO_FLAGS);
            if (!keymap) { CRASH("Couldn't get kb device id"); }


            xkb_x11_keymap_new_from_device(kb_ktx.get(), x_connection.get(), kb_device_id,  (xkb_keymap_compile_flags)0);

            NonOwningPtr<xkb_state> kb_state = xkb_x11_state_new_from_device(keymap, x_connection.get(), kb_device_id);
            if (!kb_state) { CRASH("Couldn't get kb device id"); }

            NonOwningPtr<const xcb_setup_t> x_setup = xcb_get_setup(x_connection.get());
            xcb_screen_iterator_t x_iter            = xcb_setup_roots_iterator (x_setup);
            NonOwningPtr<xcb_screen_t> x_screen     = x_iter.data;
            xcb_window_t window = xcb_generate_id(x_connection.get());


            xcb_cw_t event_mask = XCB_CW_EVENT_MASK;
            const i32 event_valwin[] = { XCB_EVENT_MASK_KEY_PRESS
                                       | XCB_EVENT_MASK_KEY_RELEASE
                                       | XCB_EVENT_MASK_BUTTON_PRESS
                                       | XCB_EVENT_MASK_BUTTON_RELEASE
                                       | XCB_EVENT_MASK_ENTER_WINDOW
                                       | XCB_EVENT_MASK_LEAVE_WINDOW
                                       | XCB_EVENT_MASK_POINTER_MOTION
                                       | XCB_EVENT_MASK_POINTER_MOTION_HINT
                                       | XCB_EVENT_MASK_BUTTON_1_MOTION
                                       | XCB_EVENT_MASK_BUTTON_2_MOTION
                                       | XCB_EVENT_MASK_BUTTON_3_MOTION
                                       | XCB_EVENT_MASK_BUTTON_4_MOTION
                                       | XCB_EVENT_MASK_BUTTON_5_MOTION
                                       | XCB_EVENT_MASK_BUTTON_MOTION
                                       | XCB_EVENT_MASK_KEYMAP_STATE
                                       | XCB_EVENT_MASK_EXPOSURE
                                       | XCB_EVENT_MASK_VISIBILITY_CHANGE
                                       | XCB_EVENT_MASK_STRUCTURE_NOTIFY
                                       | XCB_EVENT_MASK_RESIZE_REDIRECT
                                       | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
                                       | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
                                       | XCB_EVENT_MASK_FOCUS_CHANGE
                                       | XCB_EVENT_MASK_PROPERTY_CHANGE
                                       | XCB_EVENT_MASK_COLOR_MAP_CHANGE
                                       | XCB_EVENT_MASK_OWNER_GRAB_BUTTON
                                       };

            xcb_create_window(x_connection.get(),            // Connection          
                              XCB_COPY_FROM_PARENT,          // depth (same as root)
                              window,                        // window Id           
                              x_screen->root,                // parent window       
                              0, 0,                          // x, y                
                              150, 150,                      // width, height       
                              10,                            // border_width        
                              XCB_WINDOW_CLASS_INPUT_OUTPUT, // class               
                              x_screen->root_visual,         // visual              
                              event_mask, 
                              reinterpret_cast<const void*>(event_valwin) );
             /* Map the window on the screen */
            xcb_map_window(x_connection.get(), window);
            xcb_flush(x_connection.get());


            return Window(attributes, std::move(x_connection), window, kb_state);
        }
        private:
    };
}
