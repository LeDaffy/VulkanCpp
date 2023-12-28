#pragma once

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#include <render_api/deleters.hxx>

namespace render_api {
    auto create_surface();
}
