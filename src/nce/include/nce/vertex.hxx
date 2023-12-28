#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 tex_coords;

    static auto get_binding_description() -> VkVertexInputBindingDescription {
        VkVertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = sizeof(Vertex);
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return binding_description;
    }
    static auto get_attribute_desc() -> std::array<VkVertexInputAttributeDescription, 3> {
        std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions{};
        // position
        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[0].offset = offsetof(Vertex, pos);
        // color
        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[1].offset = offsetof(Vertex, color);
        // tex coords
        attribute_descriptions[2].binding = 0;
        attribute_descriptions[2].location = 2;
        attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;;
        attribute_descriptions[2].offset = offsetof(Vertex, tex_coords);


        return attribute_descriptions;
    }
};
