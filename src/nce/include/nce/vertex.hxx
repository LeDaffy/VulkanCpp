#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan_core.h>
#include <compare>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 tex_coords;
    Vertex() : pos({0.0f, 0.0f, 0.0f}), color({0.0f, 0.0f, 0.0f}), tex_coords({0.0f, 0.0f}) {}
    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && tex_coords == other.tex_coords;
    }
    bool operator<(const Vertex& other) const {
        return 
            pos.x < other.pos.x && 
            pos.y < other.pos.y && 
            pos.z < other.pos.z && 
            color.x < other.color.x && 
            color.y < other.color.y && 
            color.z < other.color.z && 
            tex_coords.x < other.tex_coords.x && 
            tex_coords.y < other.tex_coords.y;
    }
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
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.tex_coords) << 1);
        }
    };
}
