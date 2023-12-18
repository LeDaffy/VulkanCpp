#include <fmt/core.h>
#include <nce/keycode.hxx>
#include <fmt/format.h>

#include <nce/window.hxx>
#include <nce/types.hxx>
#include <nce/vke.hxx>

auto main() -> i32
{
    fmt::println("Hello world!");
    auto xwindow = window::WindowBuilder()
        .with_name("NCAD 3DX")
        .with_bg_color(32, 14, 58)
        .with_resizable(false)
        .with_dimensions(1280, 720)
        .with_position(1280/2, 720/2)
        .build();
    // vke::Instance vkeinst(xwindow);

    while (!xwindow.should_close()) {
        xwindow.poll_events();
        // vkeinst.draw_frame();
        if (xwindow.keys.is_pressed(nce::KeyCode::space)) {
            fmt::println("Pressed space");
        }
        if (xwindow.keys.is_pressed(nce::KeyCode::q)) {
            fmt::println("q pressed");
            break;
        }
        if (xwindow.keys.is_pressed(nce::KeyCode::a)) {
            fmt::println("a pressed");
        }
        if (xwindow.keys.is_pressed(nce::KeyCode::s)) {
            fmt::println("s pressed");
        }
        if (xwindow.keys.is_pressed(nce::KeyCode::d)) {
            fmt::println("d pressed");
        }
        if (xwindow.keys.is_pressed(nce::KeyCode::f)) {
            fmt::println("f pressed");
        }
        if (xwindow.keys.is_down(nce::KeyCode::w)) {
            fmt::println("w held");
        }
        // if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_pressed(KeyCode::c)) {
        //     fmt::println("Ctrl-c pressed");
        //     break;
        // }
        if (xwindow.keys.is_down(nce::KeyCode::Control_L) && xwindow.keys.is_down(nce::KeyCode::Alt_L) && xwindow.keys.is_down(nce::KeyCode::Shift_L) && (xwindow.keys.is_pressed(nce::KeyCode::C) || xwindow.keys.is_pressed(nce::KeyCode::c))) {
            fmt::println("Ctrl + Alt + Shift + C");
        }

    }
    // vkDeviceWaitIdle(vkeinst.logical_device.get());
    return 0;
}

