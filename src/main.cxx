#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

import window;
import types;
import vke;

auto main() -> i32
{
    auto window = 
        cm::WindowBuilder()
                      .with_name("NCAD 3D")
                      .with_dimensions(1280, 720)
                      .with_position(1280/2, 720/2)
                      .build();
    std::cout << window << std::endl;;
    vke::init();


    std::cout << "Hello world\n" << std::endl;
    sleep(3);
    return 0;
}

