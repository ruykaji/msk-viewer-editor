#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <QWidget>

#include "parser.hpp"

class ViewerWidget : public QWidget {

Parser* m_parser;

public:
    ViewerWidget(Parser* t_parser, QWidget* t_parent);

protected:
    void paintEvent(QPaintEvent* t_event);

public slots:
    void redraw();
};

#endif