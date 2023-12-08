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
        .with_dimensions(1280, 720)
        .with_position(1280/2, 720/2)
        .build();
    vke::Instance vkeinst(xwindow);

    while (!xwindow.should_close()) {
        xwindow.poll_events();
        if (xwindow.keys.is_pressed(KeyCode::space)) {
            fmt::println("Pressed space");
            break;
        }
        if (xwindow.keys.is_pressed(KeyCode::q)) {
            fmt::println("q pressed");
            break;
        }
        if (xwindow.keys.is_pressed(KeyCode::a)) {
            fmt::println("a pressed");
        }
        if (xwindow.keys.is_pressed(KeyCode::s)) {
            fmt::println("s pressed");
        }
        if (xwindow.keys.is_pressed(KeyCode::d)) {
            fmt::println("d pressed");
        }
        if (xwindow.keys.is_pressed(KeyCode::f)) {
            fmt::println("f pressed");
        }
        if (xwindow.keys.is_down(KeyCode::w)) {
            fmt::println("w held");
        }
        // if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_pressed(KeyCode::c)) {
        //     fmt::println("Ctrl-c pressed");
        //     break;
        // }
        if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_down(KeyCode::Alt_L) && xwindow.keys.is_down(KeyCode::Shift_L) && (xwindow.keys.is_pressed(KeyCode::C) || xwindow.keys.is_pressed(KeyCode::c))) {
            fmt::println("Ctrl + Alt + Shift + C");
        }

    }
    return 0;
}

