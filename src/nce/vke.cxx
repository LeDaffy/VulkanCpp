module;
#include <array> 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <iostream>
#include <memory>
#include <tuple>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#include <vke_macro.hxx>

#include <log.hxx>

import types;
import carray;
import log;
#ifdef DEBUG
constexpr bool use_validation_layers = true;
#else
constexpr bool use_validation_layers = false;
#endif

export module vke;


namespace vke {
    export auto init() -> void {}
    auto loop() -> void {}
    auto cleanup() -> void {}

    export struct Result {
        VkResult result;

        Result() : result(VK_SUCCESS) {}
        ~Result() = default;

        Result(VkResult& r) : result(r) {}
        Result(VkResult&& r) : result(r) {}
        void operator=(VkResult& r) { result = r; }
        void operator=(VkResult&& r) { result = r; }

        bool operator==(VkResult& r) { return result == r; }
        bool operator==(VkResult&& r) { return result == r; }


        operator CString() const {
            switch(result) {
                case 0: return "VK_SUCCESS"; break;
                case 1: return "VK_NOT_READY"; break;
                case 2: return "VK_TIMEOUT"; break;
                case 3: return "VK_EVENT_SET"; break;
                case 4: return "VK_EVENT_RESET"; break;
                case 5: return "VK_INCOMPLETE"; break;
                case -1: return "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
                case -2: return "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
                case -3: return "VK_ERROR_INITIALIZATION_FAILED"; break;
                case -4: return "VK_ERROR_DEVICE_LOST"; break;
                case -5: return "VK_ERROR_MEMORY_MAP_FAILED"; break;
                case -6: return "VK_ERROR_LAYER_NOT_PRESENT"; break;
                case -7: return "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
                case -8: return "VK_ERROR_FEATURE_NOT_PRESENT"; break;
                case -9: return "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
                case -10: return "VK_ERROR_TOO_MANY_OBJECTS"; break;
                case -11: return "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
                case -12: return "VK_ERROR_FRAGMENTED_POOL"; break;
                case -13: return "VK_ERROR_UNKNOWN"; break;
                case -1000069000: return "VK_ERROR_OUT_OF_POOL_MEMORY"; break;
                case -1000072003: return "VK_ERROR_INVALID_EXTERNAL_HANDLE"; break;
                case -1000161000: return "VK_ERROR_FRAGMENTATION"; break;
                case -1000257000: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS"; break;
                case 1000297000: return "VK_PIPELINE_COMPILE_REQUIRED"; break;
                case -1000000000: return "VK_ERROR_SURFACE_LOST_KHR"; break;
                case -1000000001: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"; break;
                case 1000001003: return "VK_SUBOPTIMAL_KHR"; break;
                case -1000001004: return "VK_ERROR_OUT_OF_DATE_KHR"; break;
                case -1000003001: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"; break;
                case -1000011001: return "VK_ERROR_VALIDATION_FAILED_EXT"; break;
                case -1000012000: return "VK_ERROR_INVALID_SHADER_NV"; break;
                case -1000023000: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR"; break;
                case -1000023001: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR"; break;
                case -1000023002: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR"; break;
                case -1000023003: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR"; break;
                case -1000023004: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR"; break;
                case -1000023005: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR"; break;
                case -1000158000: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
                case -1000174001: return "VK_ERROR_NOT_PERMITTED_KHR"; break;
                case -1000255000: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"; break;
                case 1000268000: return "VK_THREAD_IDLE_KHR"; break;
                case 1000268001: return "VK_THREAD_DONE_KHR"; break;
                case 1000268002: return "VK_OPERATION_DEFERRED_KHR"; break;
                case 1000268003: return "VK_OPERATION_NOT_DEFERRED_KHR"; break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
                case -1000299000: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR"; break;
#endif
                case -1000338000: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT"; break;
                case 1000482000: return "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT"; break;
                //case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR"; break;
                //case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"; break;
                //case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION_EXT"; break;
                //case VK_ERROR_NOT_PERMITTED_KHR: return "VK_ERROR_NOT_PERMITTED_EXT"; break;
                //case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT"; break;
                //case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR"; break;
                //case VK_PIPELINE_COMPILE_REQUIRED: return "VK_PIPELINE_COMPILE_REQUIRED_EXT"; break;
                //case VK_PIPELINE_COMPILE_REQUIRED: return "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT"; break;
                case VK_RESULT_MAX_ENUM: return "VK_RESULT_MAX_ENUM"; break;
            }
        }
        friend std::ostream &operator<<(std::ostream &os, Result& r) { 
            return os << static_cast<CString>(r); 
        }
        
    };

    struct VKEInstanceDeleter {
        void operator()(VkInstance_T* ptr){ vkDestroyInstance(ptr, nullptr); }
    };
    export struct Instance {
        std::unique_ptr<VkInstance_T, VKEInstanceDeleter> instance;
        VkApplicationInfo info_app;
        VkInstanceCreateInfo info_create;

        constexpr static std::array<CString, 2> extensions = {"VK_KHR_surface", "VK_KHR_xcb_surface"};

        Instance() : 
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
                    })
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



        }
        bool check_validation_layer_support(std::array<CString, 1> layers) {
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
        CArray<VkLayerProperties, u32> available_validation_layers() {
            u32 layer_count = 0;
            vke::Result result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
            VKE_RESULT_CRASH(result);
            CArray<VkLayerProperties, u32> layers_available(layer_count);
            result = vkEnumerateInstanceLayerProperties(&layer_count, layers_available.data());
            VKE_RESULT_CRASH(result);
            return layers_available;
        }
        CArray<VkExtensionProperties, u32> available_extensions() {
            u32 extension_count = 0;
            vke::Result result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
            VKE_RESULT_CRASH(result);
            CArray<VkExtensionProperties, u32, CFreeDeleter> extensions_available(extension_count);
            result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_available.data());
            VKE_RESULT_CRASH(result);
            return extensions_available;
        }
    };

}
