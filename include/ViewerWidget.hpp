#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QWidget>

#include "parser.hpp"
#include "DrawContextMenu.hpp"

class ViewerWidget : public QWidget {
    Q_OBJECT

    Parser* m_parser{};
    DrawContextMenu* m_contextMenu{};

    std::pair<int16_t, int16_t> m_min { INT16_MAX, INT16_MAX };
    std::pair<int16_t, int16_t> m_max { 0, 0 };
    double m_initScale { 1.0 };
    double m_currentScale { 1.0 };
    double m_scroll { 0.0 };

    bool m_isDrawing { false };
    Rect::Material m_drawingMaterial { Rect::Material::None };

    QPointF m_mouseTriggerPos {};
    QPointF m_mouseCurrentPos {};
    QPointF m_moveAxesIn {};
    QPointF m_axesPos {};

public:
    explicit ViewerWidget(Parser* t_parser, QWidget* t_parent);

private:
    void drawGrid(QPainter* t_painter);

protected:
    void paintEvent(QPaintEvent* t_event);
    void resizeEvent(QResizeEvent* t_event);
    void mousePressEvent(QMouseEvent* t_event);
    void mouseMoveEvent(QMouseEvent* t_event);
    void mouseReleaseEvent(QMouseEvent* t_event);
    void wheelEvent(QWheelEvent* t_event);

public slots:
    void setNewScaling();
    void selectDrawingMaterial(Rect::Material t_material);

signals:
    void newRect();
};

#endif