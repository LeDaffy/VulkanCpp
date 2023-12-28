#pragma once

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>
#include <render_api/deleters.hxx>

namespace render_api {

    enum VALIDATION_LAYERS_ENABLE : u8 {
        ENABLE_VALIDATION_LAYERS,
        DISABLE_VALIDATION_LAYERS
    };
    [[nodiscard]] auto available_extensions() -> std::vector<VkExtensionProperties>;
    [[nodiscard]] auto validation_layers_supported(std::vector<CString> layers) -> bool;
    [[nodiscard]] auto available_validation_layers() -> std::vector<VkLayerProperties>;
    [[nodiscard]] constexpr auto create_application_info(CString application_name) -> VkApplicationInfo;
    [[nodiscard]] auto create_instance(VALIDATION_LAYERS_ENABLE validation_layers_enable, const std::vector<CString>& validation_layers, const std::vector<CString>& extensions) -> std::unique_ptr<VkInstance_T, VKEInstanceDeleter>;

    struct Instance {
        std::unique_ptr<VkInstance_T, VKEInstanceDeleter> instance;
    };
}

