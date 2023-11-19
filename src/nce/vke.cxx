#include <nce/vke.hxx>



namespace vke {
    Instance::Instance() : 
            instance(nullptr),
            info_app({
                    VK_STRUCTURE_TYPE_APPLICATION_INFO, // VkStructureType    sType;
                    nullptr,                            // const void* pNext;
                    "NCAD 3D",                          // const char* pApplicationName;
                    VK_MAKE_VERSION(1, 0, 0),           // uint32_t applicationVersion;
                    nullptr,                            // const char* pEngineName;
                    VK_MAKE_VERSION(1, 0, 0),           // uint32_t engineVersion;
                    VK_API_VERSION_1_3                  // uint32_t apiVersion;
                    }),
            info_create({
                    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // VkStructureType sType;
                    nullptr,                                // const void* pNext;
                    0,                                      // VkInstanceCreateFlags flags;
                    &(info_app),                            // const VkApplicationInfo* pApplicationInfo;
                    0,                                      // uint32_t enabledLayerCount;
                    nullptr,                                // const char* const* ppEnabledLayerNames;
                    extensions.size(),                      // uint32_t enabledExtensionCount;
                    extensions.data()                       // const char* const* ppEnabledExtensionNames;
                    }),
            physical_device(VK_NULL_HANDLE)
        {
            CArray<VkExtensionProperties, u32> extensions_available = available_extensions();

#if 0
            for (auto const& e : extensions_available) {
                LOGINFO(e.extensionName);
            }
#endif
            constexpr std::array<CString, 1> validation_layers = { "VK_LAYER_KHRONOS_validation" };
            if (use_validation_layers && !check_validation_layer_support(validation_layers)) {
                LOGERROR("Validation layer not supported");
                std::abort();
            }
            if (use_validation_layers) {
                info_create.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
                info_create.ppEnabledLayerNames = validation_layers.data();
                vke::Result result = vkCreateInstance(&info_create, nullptr, reinterpret_cast<VkInstance*>(&instance));
                VKE_RESULT_CRASH(result);
            } else {
                vke::Result result = vkCreateInstance(&info_create, nullptr, reinterpret_cast<VkInstance*>(&instance));
                VKE_RESULT_CRASH(result);
            }

            auto devices = available_physical_devices();
            // Use an ordered map to automatically sort candidates by increasing score
            std::multimap<int, VkPhysicalDevice> candidates;
            for (auto d : devices) {
                u32 score = rate_device(d);
                candidates.insert(std::make_pair(score, d));
            }

             // Check if the best candidate is suitable at all
            if (candidates.rbegin()->first > 0) {
                physical_device = candidates.rbegin()->second;
            } else {
                LOGERROR("GPU Not Supported");
                std::abort();
            }
            if (physical_device == VK_NULL_HANDLE) {
                LOGERROR("GPU Not Supported");
                std::abort();
            }

            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(physical_device, &device_properties);
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceFeatures(physical_device, &device_features);
            std::cout << "Selected Vulkan device: " << device_properties.deviceName << std::endl;

        }
    auto Instance::find_queue_families(VkPhysicalDevice device) -> QueueFamilyIndices {
        QueueFamilyIndices indices;

        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        CArray<VkQueueFamilyProperties, u32> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

        for (const auto [index, queue_family] : std::views::enumerate(queue_families) ) {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = index;
            }
            if (indices.has_value()) { return indices; }
        }

        return indices;
    }
    auto Instance::rate_device(VkPhysicalDevice device) -> u32 {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);
        u32 score = 0;

        // Discrete GPUs have a significant performance advantage
        if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += device_properties.limits.maxImageDimension2D;

        return score;
    }
    auto Instance::is_physical_device_suitable(VkPhysicalDevice device) -> bool {
        QueueFamilyIndices indices = find_queue_families(device);

        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);
        std::cout << device_properties.deviceName << '\n';

        return indices.has_value();
    }

    auto Instance::check_validation_layer_support(std::array<CString, 1> layers) -> bool {
        auto layers_available = available_validation_layers();
        for (auto layer_name : layers) {
            bool layer_found = false;
            for (auto layer_available : layers_available) {
                std::cout << "Layer_name: " <<  layer_available.layerName << std::endl;
                if (strcmp(layer_name, layer_available.layerName) == 0) {
                    layer_found = true;
                    break;
                }
            }
            if (!layer_found) {
                return false;
            }
        }
        return true;
    }
    auto Instance::available_physical_devices() -> CArray<VkPhysicalDevice, u32> {
        u32 device_count;
        vke::Result result = vkEnumeratePhysicalDevices(instance.get(), &device_count, nullptr);
        VKE_RESULT_CRASH(result);
        if (device_count == 0) {
            LOGERROR("Vulkan not supported on this device");
            std::abort();
        }
        CArray<VkPhysicalDevice, u32> devices_available(device_count);
        result = vkEnumeratePhysicalDevices(instance.get(), &device_count, devices_available.data());
        VKE_RESULT_CRASH(result);
        return devices_available;
    }
    auto Instance::available_validation_layers() -> CArray<VkLayerProperties, u32> {
        u32 layer_count = 0;
        vke::Result result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        VKE_RESULT_CRASH(result);
        CArray<VkLayerProperties, u32> layers_available(layer_count);
        result = vkEnumerateInstanceLayerProperties(&layer_count, layers_available.data());
        VKE_RESULT_CRASH(result);
        return layers_available;
    }
    auto Instance::available_extensions() -> CArray<VkExtensionProperties, u32> {
        u32 extension_count = 0;
        vke::Result result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        VKE_RESULT_CRASH(result);
        CArray<VkExtensionProperties, u32, CFreeDeleter> extensions_available(extension_count);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_available.data());
        VKE_RESULT_CRASH(result);
        return extensions_available;
    }
}
