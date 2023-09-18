#ifndef __DRAW_PANEL_WIDGET_H__
#define __DRAW_PANEL_WIDGET_H__

#include <QMouseEvent>
#include <QWidget>

#include "DrawData.hpp"

class OptionWidget : public QWidget {
    Q_OBJECT

    QString m_label {};

public:
    bool isSelected {};
    Rect::Material material { Rect::Material::None };

    OptionWidget(const Rect::Material t_material, const QString& t_label, QWidget* t_parent);

protected:
    void paintEvent(QPaintEvent* t_event);
    void mousePressEvent(QMouseEvent* t_event);

public slots:
signals:
    void selected(Rect::Material& t_material);
};

class DrawPanelWidget : public QWidget {
    Q_OBJECT

    std::vector<OptionWidget*> m_options {};

public:
    DrawPanelWidget(QWidget* t_parent);

public slots:
    void unselect(Rect::Material& t_material);
signals:
    void selected(Rect::Material& t_material);
};

#endif