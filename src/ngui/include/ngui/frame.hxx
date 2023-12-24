#pragma once

#include <ngui/types.hxx>
#include <glm/glm.hpp>
#include <ngui/btree.hxx>

struct Vert {
    glm::vec2 pos;
    constexpr Vert(f32 x, f32 y) : pos(x, y) {}
};

struct Frame {
    enum class Split {
        HORIZONTAL,
        VERTICAL
    };
    glm::vec2 origin; // bottom left
    f32 width;
    f32 height;
    Split split;
    Frame() : origin(0.0f, 0.0f), width(0.0f), height(0.0f), split(Split::VERTICAL) {}
    Frame(f32 width, f32 height) : origin(0.0f, 0.0f), width(width), height(height), split(Split::VERTICAL) {}

    constexpr auto inside(f32 x, f32 y) const -> bool {
        if (x >= origin.x && x <= width &&
            y >= origin.y && y <= height) {
            return true;
        }
        return false;
    }



    private:
        static constexpr std::array<Vert, 4> verts = { Vert(0.0, 0.0), Vert(1.0, 0.0), Vert(1.0, 1.0), Vert(0.0, 1.0) };
        static constexpr std::array<u32, 6> indices = { 0, 1, 2, 0, 2, 3 };
};

struct UI {
    ngui::FullBTree<Frame> frames;
    ngui::BTreeNode<Frame>* active;
    static f32 max_width;
    static f32 max_height;

    UI(f32 width, f32 height) : frames(Frame(width, height)), active(frames.head()) {}
    auto split_vertical() -> void {
        frames.split(active);
        active->left->data.value().width /= 2.0f;
        active->right->data.value().width /= 2.0f;
        active->right->data.value().origin.x += active->right->data.value().width;
    }
    auto split_horizontal() -> void {
        frames.split(active);
        active->left->data.value().height /= 2.0f;
        active->right->data.value().height /= 2.0f;
        active->left->data.value().origin.y += active->left->data.value().height;
    }
};
