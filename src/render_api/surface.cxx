#include <render_api/surface.hxx>
#include <render_api/result.hxx>

#include <nce/window.hxx>


namespace render_api {
    auto create_surface(const std::unique_ptr<VkInstance_T, VKEInstanceDeleter>& instance, const window::Window& window) {
        VkXcbSurfaceCreateInfoKHR surface_create_info{};
        surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        surface_create_info.connection = window.x_connection.get();
        surface_create_info.window = window.x_window;

        VkSurfaceKHR surface = nullptr;
        render_api::Result result = vkCreateXcbSurfaceKHR(instance.get(), &surface_create_info, nullptr, &surface);
        VKE_RESULT_CRASH(result, "Failed to create XCB Surface");
        if (surface == nullptr) {
            std::abort();
        }
        auto surface_owner = std::unique_ptr<VkSurfaceKHR_T, VKESurfaceDeleter>(surface);
        surface_owner.get_deleter().instance = instance.get();
    }
}
