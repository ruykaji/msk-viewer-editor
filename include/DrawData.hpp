#ifndef __DRAW_DATA_H__
#define __DRAW_DATA_H__

#include <QPainter>

#include "abstract_syntax_tree.hpp"

void selectPenAndBrush(const Rect::Material t_material, const double& t_lineWidth, QPainter* t_painter, const uint8_t t_opacity, const bool t_penOpacity);

#endif