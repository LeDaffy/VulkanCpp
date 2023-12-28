#include <render_api/instance.hxx>
#include <render_api/result.hxx>

namespace render_api {
[[nodiscard]] auto available_extensions() -> std::vector<VkExtensionProperties> {
    u32 extension_count = 0;
    render_api::Result result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    VKE_RESULT_CRASH(result, "Failed to fetch instance extension count");

    std::vector<VkExtensionProperties> extensions_available;
    extensions_available.resize(extension_count);

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_available.data());
    VKE_RESULT_CRASH(result, "Failed to populate instnace extension vector");
    return extensions_available;
}

[[nodiscard]] constexpr auto create_application_info(CString application_name) -> VkApplicationInfo  {
    return
        VkApplicationInfo({
                VK_STRUCTURE_TYPE_APPLICATION_INFO, // VkStructureType    sType;
                nullptr,                            // const void* pNext;
                application_name,                          // const char* pApplicationName;
                VK_MAKE_VERSION(1, 0, 0),           // uint32_t applicationVersion;
                nullptr,                            // const char* pEngineName;
                VK_MAKE_VERSION(1, 0, 0),           // uint32_t engineVersion;
                VK_API_VERSION_1_3                  // uint32_t apiVersion;
                });
}

[[nodiscard]] auto create_instance(VALIDATION_LAYERS_ENABLE validation_layers_enable, const std::vector<CString>& validation_layers, const std::vector<CString>& extensions) -> std::unique_ptr<VkInstance_T, VKEInstanceDeleter> {
    std::unique_ptr<VkInstance_T, VKEInstanceDeleter> instance(nullptr); ///< Vulkan Instance
                                                                         ///
    VkApplicationInfo application_info = create_application_info("NCAD 3D");

    auto extensions_available = available_extensions();

    if (validation_layers_enable == ENABLE_VALIDATION_LAYERS && !validation_layers_supported(validation_layers)) {
        fmt::println("Validation layer not supported");
        std::abort();
    }

    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.enabledExtensionCount = static_cast<u32>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();
    // create vulkan instance
    if (validation_layers_enable) {
        instance_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        instance_create_info.ppEnabledLayerNames = validation_layers.data();
        render_api::Result result = vkCreateInstance(&instance_create_info, nullptr, reinterpret_cast<VkInstance*>(&instance));
        VKE_RESULT_CRASH(result, "Failed to create Vulkan Instance");
    } else {
        render_api::Result result = vkCreateInstance(&instance_create_info, nullptr, reinterpret_cast<VkInstance*>(&instance));
        VKE_RESULT_CRASH(result, "Failed to create Vulkan Instance");
    }

    fmt::println("---- END CREATE INSTANCE");
    return instance;
}
[[nodiscard]] auto validation_layers_supported(std::vector<CString> layers) -> bool {
    auto layers_available = available_validation_layers();
    for (const auto& layer_name : layers) {
        bool layer_found = false;
        layer_found = std::ranges::find_if(layers_available, 
                [&layer_name](const auto& s1) { 
                    if (strcmp(layer_name, s1.layerName) == 0) { return true; } 
                    return false;
                }) != std::ranges::end(layers_available);
        if (layer_found == false) {
            return false;
        } else {
            fmt::println("Layer_name: {}", layer_name);
        }
    }
    return true;
}
[[nodiscard]] auto available_validation_layers() -> std::vector<VkLayerProperties> {
    u32 layer_count = 0;
    render_api::Result result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    VKE_RESULT_CRASH(result, "Failed to get number of validation layers");
    std::vector<VkLayerProperties> layers_available;
    layers_available.resize(layer_count);
    result = vkEnumerateInstanceLayerProperties(&layer_count, layers_available.data());
    VKE_RESULT_CRASH(result, "Failed to populate validation layer vector");
    return layers_available;
}
}
