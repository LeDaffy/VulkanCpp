#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#include <nce/log.hxx>
#include <nce/keycode.hxx>

#include <nce/types.hxx>
#include <nce/carray.hxx>
#include <nce/move_only_ptr.hxx>
#include <nce/non_owning_ptr.hxx>
#include <nce/log.hxx>

struct XCBConnectionDeleter { void operator()(xcb_connection_t* ptr){ xcb_disconnect(ptr); } };
struct XCBKeySymDeleter { void operator()(xcb_key_symbols_t* ptr){ xcb_key_symbols_free(ptr); } };
struct XKBStateDeleter { void operator()(xkb_state* ptr){ xkb_state_unref(ptr); } };
struct XKBKeyMapDeleter { void operator()(xkb_keymap* ptr){ xkb_keymap_unref(ptr); } };
struct XKBContextDeleter { void operator()(xkb_context* ptr){ xkb_context_unref(ptr); } };
struct KeyPressQueue {
    bool prev;
    bool curr;

    KeyPressQueue() : prev(false), curr(false) {}
    void push(bool e) { std::swap(prev, curr); curr = e; }
};
struct KeyState {
    KeyPressQueue pressed;
    bool key_down;
};
struct KeyMap {
    std::unordered_map<KeyCode, KeyState> keys;
    bool is_pressed(KeyCode code);
    bool is_down(KeyCode code);
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
        Window(Attributes attributes, std::unique_ptr<xcb_connection_t, XCBConnectionDeleter>&& x_connection, xcb_window_t x_window, std::unique_ptr<xkb_state, XKBStateDeleter>&& kb_state) 
            : attributes(attributes), x_connection(std::move(x_connection)), 
            x_window(x_window),
            kb_state(std::move(kb_state))
        {}
    };
    struct WindowBuilder {
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
        [[nodiscard]] auto build() -> Window {
            /* Open the connection to the X server */
            std::unique_ptr<xcb_connection_t, XCBConnectionDeleter> x_connection(xcb_connect (nullptr, nullptr));

            // keyboard setup
            std::unique_ptr<xkb_context, XKBContextDeleter> kb_context(xkb_context_new(XKB_CONTEXT_NO_FLAGS));
            if (!kb_context) { LOGERROR("Couldn't create keyboard context"); std::abort(); }

            xkb_x11_setup_xkb_extension(x_connection.get(), XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, nullptr,  nullptr,  nullptr,  nullptr);

            i32 kb_device_id = xkb_x11_get_core_keyboard_device_id(x_connection.get());
            if (kb_device_id == -1) { LOGERROR("Couldn't get kb device id"); std::abort(); }

            std::unique_ptr<xkb_keymap, XKBKeyMapDeleter> keymap(xkb_x11_keymap_new_from_device(kb_context.get(), x_connection.get(), kb_device_id, XKB_KEYMAP_COMPILE_NO_FLAGS));
            if (!keymap) { LOGERROR("Couldn't get kb device id"); std::abort(); }


            xkb_x11_keymap_new_from_device(kb_context.get(), x_connection.get(), kb_device_id,  (xkb_keymap_compile_flags)0);

            std::unique_ptr<xkb_state, XKBStateDeleter> kb_state(xkb_x11_state_new_from_device(keymap.get(), x_connection.get(), kb_device_id));
            if (!kb_state) { LOGERROR("Couldn't get kb device id"); std::abort(); }

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


            return Window(attributes, std::move(x_connection), window, std::move(kb_state));
        }
        private:
    };
}
