#include <fmt/core.h>
#include <nce/window.hxx>
#include <xcb/xproto.h>



bool KeyMap::is_pressed(nce::KeyCode code) {
    if (keys.find(code) == keys.end()) {
        return false;
    }
    auto val = keys[code].pressed;
    bool ret_val = val.curr && !val.prev;
    if (ret_val) { // consume the key press
        keys[code].pressed.push(false);
    }
    return ret_val;
}
bool KeyMap::is_down(nce::KeyCode code) {
    if (keys.find(code) == keys.end()) {
        return false;
    }
    return keys[code].key_down;
}

namespace window {
void Window::poll_events() {
    std::unique_ptr<xcb_generic_event_t, CFreeDeleter> x_event(xcb_poll_for_event(x_connection.get()));
    if (!x_event) return;

    event_queue.push(std::move(x_event));
    if (!event_queue.curr) return;
    //fmt::println("Event Sequence numbers [{}, {}]", event_queue.curr->sequence, event_queue.next->sequence);


    switch (event_queue.next->response_type & ~0x80) {
        case XCB_KEY_PRESS: {
                                [[maybe_unused]] NonOwningPtr<xcb_key_press_event_t> event = reinterpret_cast<xcb_key_press_event_t*>(event_queue.next.get());
                                xkb_keysym_t keysym = xkb_state_key_get_one_sym(kb_state.get(), event->detail);
                                xkb_state_update_key(kb_state.get(), event->detail, XKB_KEY_DOWN);

                                keys.keys[static_cast<nce::KeyCode>(keysym)].pressed.push(true);
                                keys.keys[static_cast<nce::KeyCode>(keysym)].key_down = true;
#if 0
                                fmt::println("Down Sequence numbers [{}]", event->time);
                                char keysym_name[64];
                                xkb_keysym_get_name(keysym, keysym_name, sizeof(keysym_name));
                                fmt::println("{} : {}", keysym_name, keysym);
                                LOGINFO(keysym_name);
#endif
                                break;
                            } 
        case XCB_KEY_RELEASE: {
                                [[maybe_unused]] NonOwningPtr<xcb_key_release_event_t> event = reinterpret_cast<xcb_key_release_event_t*>(event_queue.next.get());
                                xkb_keysym_t keysym = xkb_state_key_get_one_sym(kb_state.get(), event->detail);
                                xkb_state_update_key(kb_state.get(), event->detail, XKB_KEY_UP);

                                keys.keys[static_cast<nce::KeyCode>(keysym)].pressed.push(false);
                                if ((event_queue.curr->response_type & ~0x80) == XCB_KEY_PRESS || 
                                        (event_queue.curr->response_type & ~0x80) == XCB_KEY_RELEASE) {
                                    NonOwningPtr<xcb_key_press_event_t> event_prev = reinterpret_cast<xcb_key_press_event_t*>(event_queue.curr.get());
                                    if (event->time != event_prev->time) {
                                        keys.keys[static_cast<nce::KeyCode>(keysym)].key_down = false;
                                    }
                                }
#if 0
                                fmt::println("Up Sequence numbers   [{}]", event->time);
#endif
                                break;
                            }
        case XCB_BUTTON_PRESS: {
                                NonOwningPtr<xcb_button_press_event_t> event = reinterpret_cast<xcb_button_press_event_t*>(event_queue.next.get());
                                break;
                            }
        case XCB_BUTTON_RELEASE: {
                                NonOwningPtr<xcb_button_release_event_t> event = reinterpret_cast<xcb_button_release_event_t*>(event_queue.next.get());
                                break;
                            }
        case XCB_EXPOSE: {
            keys.invalidate();
            std::unique_ptr<xkb_state, XKBStateDeleter> kb_state_temp(xkb_x11_state_new_from_device(keymap.get(), x_connection.get(), kb_device_id));
            kb_state.swap(kb_state_temp);

            NonOwningPtr<xcb_expose_event_t> event = reinterpret_cast<xcb_expose_event_t*>(event_queue.next.get());
            if(event->window == this->x_window) {
                printf("EXPOSE: Rect(%i, %i, %i, %i)\n", event->x, event->y, event->width, event->height);
                attributes.dimensions.x = event->width;
                attributes.dimensions.y = event->height;
                attributes.position.x = event->x;
                attributes.position.y = event->y;
                resize_callback(event->width, event->height, user_data_ptr);
            }
        } break;
    }
}
bool Window::should_close() {
    return false;
}
}
