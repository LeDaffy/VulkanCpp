#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

import window;
import types;

auto main() -> i32
{
    auto window = cm::WindowBuilder()
                      .with_name("NCAD 3D")
                      .with_dimensions(640, 480)
                      .build();


    std::cout << "Hello world\n" << std::endl;
    sleep(3);
    return 0;
}

