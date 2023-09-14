#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <QWidget>

#include "parser.hpp"

class ViewerWidget : public QWidget {

public:
    ViewerWidget(QWidget* t_parent);

public slots:
    void paintAST();
};

#endif