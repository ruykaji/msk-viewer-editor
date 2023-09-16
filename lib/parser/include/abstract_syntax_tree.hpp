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
        ND,
        PD,
        PO,
        CO,
        PO2,
        M1,
        M2,
        M3,
        M4,
        M5,
        M6
    };

    std::shared_ptr<Node> source {};

    int16_t left {};
    int16_t top {};
    int16_t width {};
    int16_t height {};
    Material material {};

    Rect() = default;
    ~Rect() = default;

    Rect(int16_t& t_left, int16_t& t_top, int16_t& width, int16_t& t_height, Material& t_material)
        : left(t_left)
        , top(t_top)
        , width(width)
        , height(t_height)
        , material(t_material) {};
};

#endif