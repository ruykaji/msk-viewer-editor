#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "parse_tree.hpp"

struct Rect {
    enum class Material {
        None,
        NW,
        DN,
        DP,
        PO,
        PO2,
        ME,
        CO,
        M2,
        VI,
        M3,
        V2,
        M4,
        V3,
        M5,
        V4,
        M6,
        V5
    };

    std::shared_ptr<Node> source {};

    int16_t left {};
    int16_t top {};
    int16_t width {};
    int16_t height {};
    Material material {};

    Rect() = default;
    ~Rect() = default;

    Rect(const int16_t& t_left, const int16_t& t_top, const int16_t& width, const int16_t& t_height, const Material& t_material)
        : left(t_left)
        , top(t_top)
        , width(width)
        , height(t_height)
        , material(t_material) {};
};

#endif