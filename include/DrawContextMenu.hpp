#ifndef __DRAW_CONTEXT_MENU_H__
#define __DRAW_CONTEXT_MENU_H__

#include <QWidget>
#include <QMenu>

#include "abstract_syntax_tree.hpp"

class DrawContextMenu: public QMenu{
    Q_OBJECT;
public:
    DrawContextMenu(QWidget* t_parent);

public slots:
signals:
    void selectDrawingMaterial(const Rect::Material t_material);
};

#endif