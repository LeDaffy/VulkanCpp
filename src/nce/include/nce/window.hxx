#pragma once
#include <limits>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_util.h>

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>
#include <xkbcommon/xkbcommon-keysyms.h>


#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#include <nce/log.hxx>
#include <nce/keycode.hxx>

#include <nce/types.hxx>
#include <nce/carray.hxx>
#include <nce/non_owning_ptr.hxx>
#include <nce/log.hxx>

#include <X11/Xutil.h>
#include <fmt/format.h>



struct XCBConnectionDeleter { void operator()(xcb_connection_t* ptr){ xcb_disconnect(ptr); } };
struct XCBAtomDeleter { void operator()(xcb_intern_atom_reply_t* ptr){ free(ptr); } };
struct XCBKeySymDeleter { void operator()(xcb_key_symbols_t* ptr){ xcb_key_symbols_free(ptr); } };
struct XKBStateDeleter { void operator()(xkb_state* ptr){ xkb_state_unref(ptr); } };
struct XKBKeyMapDeleter { void operator()(xkb_keymap* ptr){ xkb_keymap_unref(ptr); } };
struct XKBContextDeleter { void operator()(xkb_context* ptr){ xkb_context_unref(ptr); } };
struct KeyPressQueue {
    bool prev;
    bool curr;

    KeyPressQueue() : prev(false), curr(false) {}
    void push(bool e) { std::swap(prev, curr); curr = e; }
    void invalidate() { prev = false; curr = false; }
};
struct KeyState {
    KeyPressQueue pressed;
    bool key_down;
    void invalidate() { pressed.invalidate(); key_down = false; }
};
struct KeyMap {
    std::unordered_map<nce::KeyCode, KeyState> keys;
    bool is_pressed(nce::KeyCode code);
    bool is_down(nce::KeyCode code);
    void invalidate() { for (auto& e : keys) { e.second.invalidate(); } };
};

namespace window {
    template<typename T> struct WindowVec2 {
        T x;
        T y;
        WindowVec2() : x(0), y(0) {}
        WindowVec2(T x, T y) : x(x), y(y) {}
    };
    template<typename T> struct WindowRGB {
        T r;
        T g;
        T b;
        WindowRGB() : r(0), g(0), b(0) {}
        WindowRGB(T r, T g, T b) : r(r), g(g), b(b) {}
    };

    struct Attributes {
        WindowRGB<u8> background_color;
        WindowVec2<u32> dimensions;
        WindowVec2<u32> position;
        CStr<u8> name;
        bool resizable;
        Attributes() :
            background_color(25, 25, 25), dimensions(640, 480), position(0, 0), name("XCB Window"), resizable(false) {}
        Attributes(WindowRGB<u8> bg_color, WindowVec2<u32> dimensions, WindowVec2<u32> position, CStr<u8> name, bool resizable) :
            background_color(bg_color), dimensions(dimensions), position(position), name(name), resizable(resizable) {}
    };

    struct EventQueue {
        std::unique_ptr<xcb_generic_event_t, CFreeDeleter> prev;
        std::unique_ptr<xcb_generic_event_t, CFreeDeleter> curr;
        std::unique_ptr<xcb_generic_event_t, CFreeDeleter> next;

        EventQueue() : prev(nullptr), curr(nullptr), next(nullptr) {}
        void push(std::unique_ptr<xcb_generic_event_t, CFreeDeleter>&& e) {
            prev.swap(e);
            prev.swap(curr);
            curr.swap(next);
        }
    };

    struct Window {
        Attributes attributes;
        std::unique_ptr<xcb_connection_t, XCBConnectionDeleter> x_connection;
        xcb_window_t x_window;
        std::unique_ptr<xkb_state, XKBStateDeleter> kb_state;
        EventQueue event_queue;
        KeyMap keys;
        std::function<void(u32 width, u32 height, void* user_data)> resize_callback = nullptr;
        void* user_data_ptr = nullptr;

        i32 kb_device_id;
        std::unique_ptr<xkb_keymap, XKBKeyMapDeleter> keymap;

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
        void poll_events();
        bool should_close();

        friend struct WindowBuilder;
        friend std::ostream &operator<<(std::ostream &os, Window& w) {
            return os << "Window (" << &w << "): { " << "asdf";
        }
        private:
        Window() {}
        Window(Attributes attributes, std::unique_ptr<xcb_connection_t, XCBConnectionDeleter>&& x_connection, xcb_window_t x_window, std::unique_ptr<xkb_state, XKBStateDeleter>&& kb_state, std::function<void(u32 width, u32 height, void* user_data)> resize_callback, i32 kb_device_id, std::unique_ptr<xkb_keymap, XKBKeyMapDeleter>&& keymap)
            : attributes(attributes), x_connection(std::move(x_connection)), 
            x_window(x_window),
            kb_state(std::move(kb_state)),
            resize_callback(resize_callback),
            kb_device_id(kb_device_id),
            keymap(std::move(keymap))
        {
            xcb_change_property (this->x_connection.get(),
                    XCB_PROP_MODE_REPLACE,
                    this->x_window,
                    XCB_ATOM_WM_NAME,
                    XCB_ATOM_STRING,
                    8,
                    this->attributes.name.size(),
                    this->attributes.name);


        }
    };
    struct WindowBuilder {
        Attributes attributes;
        std::function<void(u32 width, u32 height, void* user_data)> resize_callback = nullptr;
        WindowBuilder() {};
        auto with_dimensions(u32 x, u32 y) -> WindowBuilder& {
            attributes.dimensions = WindowVec2<u32>(x, y);
            return *this;
        }
        auto with_bg_color(u8 r, u8 g, u8 b) -> WindowBuilder& {
            attributes.background_color = WindowRGB<u8>(r, g, b);
            return *this;
        }
        auto with_resizable(bool resizable) -> WindowBuilder& {
            attributes.resizable = resizable;
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
        auto with_resize_callback(std::function<void(u32 width, u32 height, void* user_data)> resize_callback) -> WindowBuilder& {
            this->resize_callback = resize_callback;
            return *this;
        }
        [[nodiscard]] auto build() -> Window {
            /* Open the connection to the X server */
            std::unique_ptr<xcb_connection_t, XCBConnectionDeleter> x_connection(xcb_connect (nullptr, nullptr));

            // Conncet to WM
            std::string WM_PROTOCOLS_PROPERTY_NAME = "WM_PROTOCOLS";
            xcb_intern_atom_cookie_t intern_atom_cookie = xcb_intern_atom(x_connection.get(), true, static_cast<u16>(WM_PROTOCOLS_PROPERTY_NAME.size()),
                    WM_PROTOCOLS_PROPERTY_NAME.c_str());
            std::unique_ptr<xcb_intern_atom_reply_t, XCBAtomDeleter> intern_atom_reply(xcb_intern_atom_reply(x_connection.get(), intern_atom_cookie, NULL));
            // [unused var] xcb_atom_t window_manager_protocols_property = intern_atom_reply->atom;

            // keyboard setup
            std::unique_ptr<xkb_context, XKBContextDeleter> kb_context(xkb_context_new(XKB_CONTEXT_NO_FLAGS));
            if (!kb_context) { LOGERROR("Couldn't create keyboard context"); std::abort(); }

            xkb_x11_setup_xkb_extension(x_connection.get(), XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, nullptr,  nullptr,  nullptr,  nullptr);

            i32 kb_device_id = xkb_x11_get_core_keyboard_device_id(x_connection.get());
            if (kb_device_id == -1) { LOGERROR("Couldn't get kb device id"); std::abort(); }

            std::unique_ptr<xkb_keymap, XKBKeyMapDeleter> keymap(xkb_x11_keymap_new_from_device(kb_context.get(), x_connection.get(), kb_device_id, XKB_KEYMAP_COMPILE_NO_FLAGS));
            if (!keymap) { LOGERROR("Couldn't get kb device id"); std::abort(); }



            std::unique_ptr<xkb_state, XKBStateDeleter> kb_state(xkb_x11_state_new_from_device(keymap.get(), x_connection.get(), kb_device_id));
            if (!kb_state) { LOGERROR("Couldn't get kb device id"); std::abort(); }

            NonOwningPtr<const xcb_setup_t> x_setup = xcb_get_setup(x_connection.get());
            xcb_screen_iterator_t x_iter            = xcb_setup_roots_iterator (x_setup);
            NonOwningPtr<xcb_screen_t> x_screen     = x_iter.data;
            xcb_window_t window = xcb_generate_id(x_connection.get());




            xcb_create_window_value_list_t value_window = {};
            value_window.background_pixel = static_cast<u32>(attributes.background_color.b) << 16u 
                | static_cast<u32>(attributes.background_color.g) << 8u  
                | static_cast<u32>(attributes.background_color.b) << 0u 
                | 255u << 24u;

            value_window.event_mask = XCB_EVENT_MASK_KEY_PRESS
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
                | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
                | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
                | XCB_EVENT_MASK_FOCUS_CHANGE
                | XCB_EVENT_MASK_PROPERTY_CHANGE
                | XCB_EVENT_MASK_COLOR_MAP_CHANGE
                | XCB_EVENT_MASK_OWNER_GRAB_BUTTON;
            if (!attributes.resizable) {
                value_window.event_mask |= XCB_EVENT_MASK_RESIZE_REDIRECT;
            }

            xcb_create_window_aux(x_connection.get(),
                    x_screen->root_depth,
                    window,
                    x_screen->root,
                    0, 0, 1280, 720,
                    0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    x_screen->root_visual,
                    XCB_CW_EVENT_MASK | XCB_CW_BACK_PIXEL, 
                    &value_window);

            fmt::println("Resizable is {}", attributes.resizable);
            if (!attributes.resizable) {
                xcb_size_hints_t window_size_hints{};
                xcb_icccm_size_hints_set_min_size(&window_size_hints, static_cast<i32>(attributes.dimensions.x), static_cast<i32>(attributes.dimensions.y));
                xcb_icccm_size_hints_set_max_size(&window_size_hints, static_cast<i32>(attributes.dimensions.x), static_cast<i32>(attributes.dimensions.y));

                xcb_icccm_set_wm_size_hints(x_connection.get(), window, XCB_ATOM_WM_NORMAL_HINTS, &window_size_hints);
            }

            /* Map the window on the screen */
            xcb_map_window(x_connection.get(), window);
            xcb_flush(x_connection.get());


            return Window(attributes, std::move(x_connection), window, std::move(kb_state), resize_callback, kb_device_id, std::move(keymap));
        }
    };
}
