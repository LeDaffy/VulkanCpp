#include <vulkan/vulkan_core.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include <nce/vke_macro.hxx>

#include <nce/log.hxx>
#include <nce/types.hxx>
#include <nce/log.hxx>
#include <nce/window.hxx>
#include <nce/vertex.hxx>

namespace vke {
#ifndef NDEBUG
constexpr bool use_validation_layers = true;
#else
constexpr bool use_validation_layers = false;
#endif

/**
 *  @brief A container storing a VkResult.
 *  vke::Result provides functionality to print the error type from VkResult.
 */
struct Result {
    VkResult result;

    Result() : result(VK_SUCCESS) {}
    ~Result() = default;

    Result(VkResult& r) : result(r) {}
    Result(VkResult&& r) : result(std::move(r)) {}
    void operator=(VkResult& r) { result = r; }
    void operator=(VkResult&& r) { result = std::move(r); }

    bool operator==(VkResult& r) { return result == r; }
    bool operator==(VkResult&& r) { return result == std::move(r); }

    bool operator!=(VkResult& r) { return result != r; }
    bool operator!=(VkResult&& r) { return result != std::move(r); }
    operator bool() const { return result == VK_SUCCESS; }


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
            case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR"; break;
            case VK_RESULT_MAX_ENUM: return "VK_RESULT_MAX_ENUM"; break;
        }
        return "Not Implemented";
    }
    friend std::ostream &operator<<(std::ostream &os, Result& r) { 
        return os << static_cast<CString>(r); 
    }
};


struct VKEInstanceDeleter { void operator()(VkInstance_T* ptr){ vkDestroyInstance(ptr, nullptr); } };
struct VKEDeviceDeleter { void operator()(VkDevice_T* ptr){ vkDestroyDevice(ptr, nullptr); } };
struct VKESwapChainDeleter { void operator()(VkSwapchainKHR_T* ptr); };
struct VKEImageViewDeleter { void operator()(VkImageView ptr); };
struct VKESurfaceDeleter { void operator()(VkSurfaceKHR_T* ptr); };
struct VKEShaderModuleDeleter { void operator()(VkShaderModule_T* ptr); };
struct VKEPipelineLayoutDeleter { void operator()(VkPipelineLayout_T* ptr); };
struct VKERenderPassDeleter { void operator()(VkRenderPass_T* ptr); };
struct VKEGraphicsPipelineDeleter { void operator()(VkPipeline_T* ptr); };
struct VKEFramebufferDeleter { void operator()(VkFramebuffer_T* ptr); };
struct VKECommandPoolDeleter { void operator()(VkCommandPool_T* ptr); };
struct VKESemaphoreDeleter { void operator()(VkSemaphore_T* ptr); };
struct VKEFenceDeleter { void operator()(VkFence_T* ptr); };
struct VKEBufferDeleter { void operator()(VkBuffer_T* ptr); };
struct VKEMemoryDeleter { void operator()(VkDeviceMemory_T* ptr); };
struct VKEDescriptorSetLayoutDeleter { void operator()(VkDescriptorSetLayout_T* ptr); };
struct VKEDescriptorPoolDeleter { void operator()(VkDescriptorPool_T* ptr); };

struct QueueFamilyIndices {
    std::optional<u32> graphics_family;
    std::optional<u32> present_family;
    auto has_value() -> bool { return graphics_family.has_value() && present_family.has_value(); }
    auto has_value() const -> bool { return graphics_family.has_value() && present_family.has_value(); }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

/**
 *  @brief Container that initializes and holds a vulkan instance.
 */
struct Instance {
    // Static Members
    /// @brief Required extensions for drawing with vulkan
    constexpr static std::array<CString, 1> validation_layers = { "VK_LAYER_KHRONOS_validation" };
    constexpr static std::array<CString, 2> extensions = { "VK_KHR_surface", "VK_KHR_xcb_surface" };
    constexpr static std::array<CString, 1> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    constexpr static u32 MAX_FRAMES_IN_FLIGHT = 2;


    //members
    VkApplicationInfo info_app; ///< Properties of the vulkan application
    VkInstanceCreateInfo info_create; ///< Vulkan Instance creation parameters
    static std::unique_ptr<VkInstance_T, VKEInstanceDeleter> instance; ///< Vulkan Instance
    static std::unique_ptr<VkSurfaceKHR_T, VKESurfaceDeleter> surface;
    static VkPhysicalDevice physical_device;
    static std::unique_ptr<VkDevice_T, VKEDeviceDeleter> logical_device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    std::unique_ptr<VkSwapchainKHR_T, VKESwapChainDeleter> swapchain;
    std::vector<VkImage> swapchain_images;
    std::vector<std::unique_ptr<VkImageView_T, VKEImageViewDeleter>> swapchain_image_views;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_extent;
    std::unique_ptr<VkRenderPass_T, VKERenderPassDeleter> render_pass;
    std::unique_ptr<VkDescriptorSetLayout_T, VKEDescriptorSetLayoutDeleter> descriptor_set_layout;
    std::unique_ptr<VkPipelineLayout_T, VKEPipelineLayoutDeleter> pipeline_layout;
    std::unique_ptr<VkPipeline_T, VKEGraphicsPipelineDeleter> graphics_pipeline;
    std::vector<std::unique_ptr<VkFramebuffer_T, VKEFramebufferDeleter>> swapchain_framebuffers;
    std::unique_ptr<VkCommandPool_T, VKECommandPoolDeleter> command_pool;
    std::vector<VkCommandBuffer> command_buffers;

    std::vector<std::unique_ptr<VkSemaphore_T, VKESemaphoreDeleter>> image_available_semaphores;
    std::vector<std::unique_ptr<VkSemaphore_T, VKESemaphoreDeleter>> render_finished_semaphores;
    std::vector<std::unique_ptr<VkFence_T, VKEFenceDeleter>> in_flight_fences;
    u32 current_frame = 0;
    bool frame_buffer_resized = false;
    const window::Window& window;
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    std::unique_ptr<VkBuffer_T, VKEBufferDeleter> vertex_buffer;
    std::unique_ptr<VkDeviceMemory_T, VKEMemoryDeleter> vertex_buffer_memory;
    std::unique_ptr<VkBuffer_T, VKEBufferDeleter> index_buffer;
    std::unique_ptr<VkDeviceMemory_T, VKEMemoryDeleter> index_buffer_memory;
    std::vector<std::unique_ptr<VkBuffer_T, VKEBufferDeleter>> uniform_buffers;
    std::vector<std::unique_ptr<VkDeviceMemory_T, VKEMemoryDeleter>> uniform_buffers_memory;
    std::vector<void*> uniform_buffers_mapped;

    std::unique_ptr<VkDescriptorPool_T, VKEDescriptorPoolDeleter> descriptor_pool;
    std::vector<VkDescriptorSet> descriptor_sets;




    /// @brief Creates an Instance.
    /// Itializes Vulkan, selects a physical devices
    Instance(window::Window& window);
    void update_uniform_buffer(u32 current_image);
    void create_instance();
    void create_surface(const window::Window& window);
    void pick_physical_device();
    void create_logical_device();
    void create_swapchain();
    void create_image_views();
    void create_render_pass();
    void create_descriptor_set_layout();
    void create_graphics_pipeline();
    void create_framebuffers();
    void create_command_pool();
    void create_command_buffers();
    void record_command_buffer(VkCommandBuffer command_buffer, u32 image_index);
    void draw_frame();
    void create_sync_objects();
    void create_vertex_buffer();
    void create_index_buffer();
    void create_uniform_buffers();
    void create_descriptor_pool();
    void create_descriptor_sets();
    void recreate_swapchain();
    void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, std::unique_ptr<VkBuffer_T, VKEBufferDeleter>& buffer, std::unique_ptr<VkDeviceMemory_T, VKEMemoryDeleter>& buffer_memory);

    auto copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size) const -> void;
    [[nodiscard]] auto find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties) const -> u32;
    [[nodiscard]] auto check_device_extension_support(VkPhysicalDevice device) const -> bool;
    [[nodiscard]] auto find_queue_families(VkPhysicalDevice device) -> QueueFamilyIndices;
    /// @brief Rate GPUs based on needed features
    auto rate_device(VkPhysicalDevice device) -> u32;
    [[nodiscard]] auto is_physical_device_suitable(VkPhysicalDevice device) -> bool;
    [[nodiscard]] auto check_validation_layer_support(std::array<CString, 1> layers) -> bool;
    /// @brief Query vulkan for the avaiable GPUs
    [[nodiscard]] auto available_physical_devices() const -> std::vector<VkPhysicalDevice>;
    /// @brief Query vulkan for the avaiable validation layers
    [[nodiscard]] auto available_validation_layers() const -> std::vector<VkLayerProperties>;
    /// @brief Query vulkan for the avaiable extensions
    [[nodiscard]] auto available_extensions() const -> std::vector<VkExtensionProperties>;
    [[nodiscard]] auto query_swapchain_support(VkPhysicalDevice device, NonOwningPtr<VkSurfaceKHR_T> surface) const -> SwapChainSupportDetails;
    [[nodiscard]] auto choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) const -> VkSurfaceFormatKHR;
    [[nodiscard]] auto choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes) const -> VkPresentModeKHR;
    [[nodiscard]] auto choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) const -> VkExtent2D;
    [[nodiscard]] auto create_shader_module(const std::vector<std::byte>& shader_code) const -> std::unique_ptr<VkShaderModule_T, VKEShaderModuleDeleter>;
};


}
