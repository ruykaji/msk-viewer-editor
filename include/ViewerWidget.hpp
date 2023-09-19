#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QWidget>

#include "DrawContextMenu.hpp"
#include "parser.hpp"

class ViewerWidget : public QWidget {
    Q_OBJECT

    enum class Mode {
        DEFAULT,
        DRAWING,
        DRAGING,
    };

    Mode m_mode  {};
    
    Parser* m_parser {};
    DrawContextMenu* m_contextMenu {};

    bool m_isScaleSet { false };
    bool m_isPositionSet { false };
    std::pair<int16_t, int16_t> m_min { INT16_MAX, INT16_MAX };
    std::pair<int16_t, int16_t> m_max { 0, 0 };
    double m_initScale { 1.0 };
    double m_currentScale { 1.0 };
    double m_scroll { 0.0 };

    Rect::Material m_drawingMaterial { Rect::Material::ME };

    QPoint m_mouseTriggerPos {};
    QPoint m_mouseCurrentPos {};
    QPoint m_moveAxesIn {};
    QPoint m_axesPos {};

public:
    explicit ViewerWidget(Parser* t_parser, QWidget* t_parent);

private:
    void drawGrid(QPainter* t_painter);
    void setMinMax();
    void resetAxisPos();

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