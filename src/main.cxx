import window;
import types;
#include <cstdio>

auto main() -> i32
{
    auto window = cm::WindowBuilder().dimensions(640, 480).build();
    std::printf("Hello world\n");
    i32 x = 0;
    return 0;
}

