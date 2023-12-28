#pragma once

#include <vulkan/vulkan.h>

struct VKEInstanceDeleter { void operator()(VkInstance_T* ptr){ vkDestroyInstance(ptr, nullptr); } };

struct VKESurfaceDeleter { 
    VkInstance instance = nullptr;
    void operator()(VkSurfaceKHR_T* ptr){ 
        vkDestroySurfaceKHR(instance, ptr, nullptr); 
    } 
};
