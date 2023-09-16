#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <QPainter>
#include <QWidget>

#include "parser.hpp"

class ViewerWidget : public QWidget {

    Parser* m_parser;

    std::pair<int16_t, int16_t> m_min { INT16_MAX, INT16_MAX };
    std::pair<int16_t, int16_t> m_max { 0, 0 };
    double m_initScale { 1.0 };
    double m_currentScale { 1.0 };

public:
    explicit ViewerWidget(Parser* t_parser, QWidget* t_parent);

private:
    void selectPenAndBrush(const Rect::Material t_material, QPainter* t_painter);

protected:
    void paintEvent(QPaintEvent* t_event);
    void resizeEvent(QResizeEvent* t_event);

public slots:
    void setNewScaling();
};

#endif