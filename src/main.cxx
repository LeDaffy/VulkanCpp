#include <nce/keycode.hxx>

#include <nce/window.hxx>
#include <nce/types.hxx>
#include <nce/vke.hxx>

auto main() -> i32
{
    auto xwindow = window::WindowBuilder()
        .with_name("NCAD 3D")
        .with_dimensions(1280, 720)
        .with_position(1280/2, 720/2)
        .build();
    vke::Instance vkeinst(xwindow);

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
        // if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_pressed(KeyCode::c)) {
        //     std::cout << "Ctrl-c pressed" << std::endl;
        //     break;
        // }
        if (xwindow.keys.is_down(KeyCode::Control_L) && xwindow.keys.is_down(KeyCode::Alt_L) && xwindow.keys.is_down(KeyCode::Shift_L) && (xwindow.keys.is_pressed(KeyCode::C) || xwindow.keys.is_pressed(KeyCode::c))) {
            std::cout << "Ctrl + Alt + Shift + C" << std::endl;
        }

    }
    return 0;
}

