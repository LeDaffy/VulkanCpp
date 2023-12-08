#include "nce/vke_macro.hxx"
#include <fmt/core.h>
#include <nce/vke.hxx>
#include <limits>
#include <algorithm>
#include <vulkan/vulkan_core.h>



namespace vke {
    void VKEImageViewDeleter::operator()(VkImageView ptr){ vkDestroyImageView(Instance::logical_device.get(), ptr, nullptr); } 
    void VKESwapChainDeleter::operator()(VkSwapchainKHR_T* ptr){ vkDestroySwapchainKHR(Instance::logical_device.get(), ptr, nullptr); } 
    void VKESurfaceDeleter::operator()(VkSurfaceKHR_T* ptr){ 
        vkDestroySurfaceKHR(Instance::instance.get(), ptr, nullptr); 
    } 

    //static members
    std::unique_ptr<VkInstance_T, VKEInstanceDeleter> Instance::instance = nullptr;
    std::unique_ptr<VkSurfaceKHR_T, VKESurfaceDeleter> Instance::surface = nullptr;
    VkPhysicalDevice Instance::physical_device = nullptr;
    std::unique_ptr<VkDevice_T, VKEDeviceDeleter> Instance::logical_device = nullptr;

    void Instance::create_graphics_pipeline() {

    }
    void Instance::create_swapchain() {
        SwapChainSupportDetails swapchain_support = query_swapchain_support(this->physical_device, this->surface.get());
        VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swapchain_support.formats);
        VkPresentModeKHR present_mode = choose_swap_present_mode(swapchain_support.present_modes);
        VkExtent2D extent = choose_swap_extent(swapchain_support.capabilities);

        u32 image_count = swapchain_support.capabilities.minImageCount + 1;
        if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount) {
            image_count = swapchain_support.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = surface.get();
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = find_queue_families(this->physical_device);
        u32 queueFamilyIndices[] = {indices.graphics_family.value(), indices.present_family.value()};

        if (indices.graphics_family != indices.present_family) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0; // Optional
            create_info.pQueueFamilyIndices = nullptr; // Optional
        }
        create_info.preTransform = swapchain_support.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;
        VkSwapchainKHR temp_swapchain = nullptr;
        vke::Result result =  vkCreateSwapchainKHR(logical_device.get(), &create_info, nullptr, &temp_swapchain);
        VKE_RESULT_CRASH(result);
        auto x = VK_SUCCESS;

        this->swapchain.reset(temp_swapchain);

        vkGetSwapchainImagesKHR(this->logical_device.get(), this->swapchain.get(), &image_count, nullptr);
        swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(this->logical_device.get(), this->swapchain.get(), &image_count, swapchain_images.data());
        swapchain_image_format = surface_format.format;
        swapchain_extent = extent;
    }
    void Instance::create_image_views() {
        swapchain_image_views.resize(swapchain_images.size());
        for (const auto [index, image] : std::views::enumerate(swapchain_image_views) ) {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = swapchain_images[index];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = swapchain_image_format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            VkImageView_T* temp_image_views = nullptr;
            vke::Result result = vkCreateImageView(this->logical_device.get(), &create_info, nullptr, &temp_image_views);
            swapchain_image_views[index].reset(temp_image_views);
            if (!result) {
                fmt::println("Failed to create image view");
                VKE_RESULT_CRASH(result);
            }

        }
    }
    void Instance::create_instance() {
        CArray<VkExtensionProperties, u32> extensions_available = available_extensions();

#if 0
        for (auto const& e : extensions_available) {
            LOGINFO(e.extensionName);
        }
#endif
        if (use_validation_layers && !check_validation_layer_support(this->validation_layers)) {
            LOGERROR("Validation layer not supported");
            std::abort();
        }

        // create vulkan instance
        if (use_validation_layers) {
            this->info_create.enabledLayerCount = static_cast<uint32_t>(this->validation_layers.size());
            this->info_create.ppEnabledLayerNames = this->validation_layers.data();
            vke::Result result = vkCreateInstance(&this->info_create, nullptr, reinterpret_cast<VkInstance*>(&this->instance));
            VKE_RESULT_CRASH(result);
        } else {
            vke::Result result = vkCreateInstance(&this->info_create, nullptr, reinterpret_cast<VkInstance*>(&this->instance));
            VKE_RESULT_CRASH(result);
        }
    }
    void Instance::create_surface(const window::Window& window) {
        // create window surface
        VkXcbSurfaceCreateInfoKHR surface_create_info = {
            // VkStructureType               sType;
            // const void*                   pNext;
            // VkXcbSurfaceCreateFlagsKHR    flags;
            // xcb_connection_t*             connection;
            // xcb_window_t                  window;
        };
        surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        surface_create_info.connection = window.x_connection.get();
        surface_create_info.window = window.x_window;

        VkSurfaceKHR l_instance = nullptr;
        vke::Result result = vkCreateXcbSurfaceKHR(this->instance.get(), &surface_create_info, nullptr, &l_instance);
        VKE_RESULT_CRASH(result);
        if (l_instance == nullptr) {
            std::abort();
        }
        this->surface = std::unique_ptr<VkSurfaceKHR_T, VKESurfaceDeleter>(l_instance);
    }
    auto Instance::query_swapchain_support(VkPhysicalDevice device, NonOwningPtr<VkSurfaceKHR_T> surface) const -> SwapChainSupportDetails {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        u32 format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

        if (format_count != 0) {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
        }

        u32 present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

        if (present_mode_count != 0) {
            details.present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
        }

        return details;
    }

    void Instance::pick_physical_device() {
        auto devices = available_physical_devices();
        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;
        for (auto d : devices) {
            u32 score = rate_device(d);
            if (is_physical_device_suitable(d)) {
                candidates.insert(std::make_pair(score, d));
            }
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) {
            this->physical_device = candidates.rbegin()->second;
        } else {
            LOGERROR("GPU Not Supported");
            std::abort();
        }
        if (this->physical_device == VK_NULL_HANDLE) {
            LOGERROR("GPU Not Supported");
            std::abort();
        }

        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(this->physical_device, &device_properties);
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(this->physical_device, &device_features);
        fmt::println("Selected Vulkan device: {}", device_properties.deviceName);

    }
    Instance::Instance(const window::Window& window) : 
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
                &(this->info_app),                            // const VkApplicationInfo* pApplicationInfo;
                0,                                      // uint32_t enabledLayerCount;
                nullptr,                                // const char* const* ppEnabledLayerNames;
                this->extensions.size(),                      // uint32_t enabledExtensionCount;
                this->extensions.data()                       // const char* const* ppEnabledExtensionNames;
                })
        {
            create_instance();
            create_surface(window);
            pick_physical_device();
            create_logical_device();
            create_swapchain();
            create_image_views();

        }

    void Instance::create_logical_device() {
        // Specifying the queues to be created
        QueueFamilyIndices indices = find_queue_families(this->physical_device);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<u32> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

        const float queue_priority = 1.0f;
        for (auto queue_family : unique_queue_families) {
            VkDeviceQueueCreateInfo queue_create_info = {
                // VkStructureType             sType;
                // const void*                 pNext;
                // VkDeviceQueueCreateFlags    flags;
                // uint32_t                    queueFamilyIndex;
                // uint32_t                    queueCount;
                // const float*                pQueuePriorities;
            };
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        // Specifying used device features
        VkPhysicalDeviceFeatures device_features = {};

        // Creating the logical device
        VkDeviceCreateInfo create_info = {
            // VkStructureType                    sType;
            // const void*                        pNext;
            // VkDeviceCreateFlags                flags;
            // uint32_t                           queueCreateInfoCount;
            // const VkDeviceQueueCreateInfo*     pQueueCreateInfos;
            // uint32_t                           enabledLayerCount;
            // const char* const*                 ppEnabledLayerNames;
            // uint32_t                           enabledExtensionCount;
            // const char* const*                 ppEnabledExtensionNames;
            // const VkPhysicalDeviceFeatures*    pEnabledFeatures;
        };
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = static_cast<u32>(queue_create_infos.size());
        create_info.pEnabledFeatures = &device_features;

        create_info.enabledExtensionCount = static_cast<u32>(this->device_extensions.size());
        create_info.ppEnabledExtensionNames = this->device_extensions.data();

        if (enable_validation_layers) {
            create_info.enabledLayerCount = static_cast<u32>(this->validation_layers.size());
            create_info.ppEnabledLayerNames = this->validation_layers.data();
        } else {
            create_info.enabledLayerCount = 0;
        }


        //create logical device
        vke::Result result = vkCreateDevice(this->physical_device, &create_info, nullptr, reinterpret_cast<VkDevice*>(&this->logical_device));
        VKE_RESULT_CRASH(result)

            vkGetDeviceQueue(this->logical_device.get(), indices.graphics_family.value(), 0, &this->graphics_queue);
        vkGetDeviceQueue(this->logical_device.get(), indices.present_family.value(), 0, &this->present_queue);
    }
    auto Instance::find_queue_families(VkPhysicalDevice device) -> QueueFamilyIndices {
        QueueFamilyIndices indices;

        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        CArray<VkQueueFamilyProperties, u32> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

        VkBool32 present_support = 0;
        for (const auto [index, queue_family] : std::views::enumerate(queue_families) ) {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = index;
            }
            vkGetPhysicalDeviceSurfaceSupportKHR(device, index, this->surface.get(), &present_support);
            if (present_support) {
                indices.present_family = index;
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
        fmt::println("{}", device_properties.deviceName);

        bool extensions_supported = check_device_extension_support(device);

        bool swapchain_adequate = false;
        if (extensions_supported) {
            SwapChainSupportDetails swapchain_support = query_swapchain_support(device, this->surface.get());
            swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
        }

        return indices.has_value() && extensions_supported && swapchain_adequate;
    }
    auto Instance::check_device_extension_support(VkPhysicalDevice device) const -> bool {
        u32 extension_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        CArray<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> requiredExtensions(this->device_extensions.begin(), this->device_extensions.end());

        for (const auto& extension : available_extensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    auto Instance::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) const -> VkSurfaceFormatKHR {
        for (const auto& available_format : available_formats) {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available_format;
            }
        }
        return available_formats[0];
    }

    auto Instance::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes) const -> VkPresentModeKHR {
        // constexpr std::array<VkPresentModeKHR, 4> modes = {
        //     VK_PRESENT_MODE_IMMEDIATE_KHR, // no sync
        //     VK_PRESENT_MODE_FIFO_KHR, // vsync
        //     VK_PRESENT_MODE_FIFO_RELAXED_KHR, // 
        //     VK_PRESENT_MODE_MAILBOX_KHR // triple buffering
        // };
        VkPresentModeKHR desired = VK_PRESENT_MODE_FIFO_KHR; // guaranteed to exist
        for (const auto& mode : available_present_modes) {
            if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                desired = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
        return desired;
    }

    auto Instance::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) const -> VkExtent2D {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            u32 width = 1280;
            u32 height = 720;

            VkExtent2D actualExtent = { width, height };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    auto Instance::check_validation_layer_support(std::array<CString, 1> layers) -> bool {
        auto layers_available = available_validation_layers();
        for (auto layer_name : layers) {
            bool layer_found = false;
            for (auto layer_available : layers_available) {
                fmt::println("Layer_name: {}", layer_available.layerName);
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
    auto Instance::available_physical_devices() const -> CArray<VkPhysicalDevice, u32> {
        u32 device_count;
        vke::Result result = vkEnumeratePhysicalDevices(this->instance.get(), &device_count, nullptr);
        VKE_RESULT_CRASH(result);
        if (device_count == 0) {
            LOGERROR("Vulkan not supported on this device");
            std::abort();
        }
        CArray<VkPhysicalDevice, u32> devices_available(device_count);
        result = vkEnumeratePhysicalDevices(this->instance.get(), &device_count, devices_available.data());
        VKE_RESULT_CRASH(result);
        return devices_available;
    }
    auto Instance::available_validation_layers() const -> CArray<VkLayerProperties, u32> {
        u32 layer_count = 0;
        vke::Result result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        VKE_RESULT_CRASH(result);
        CArray<VkLayerProperties, u32> layers_available(layer_count);
        result = vkEnumerateInstanceLayerProperties(&layer_count, layers_available.data());
        VKE_RESULT_CRASH(result);
        return layers_available;
    }
    auto Instance::available_extensions() const -> CArray<VkExtensionProperties, u32> {
        u32 extension_count = 0;
        vke::Result result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        VKE_RESULT_CRASH(result);
        CArray<VkExtensionProperties, u32, CFreeDeleter> extensions_available(extension_count);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_available.data());
        VKE_RESULT_CRASH(result);
        return extensions_available;
    }
}
