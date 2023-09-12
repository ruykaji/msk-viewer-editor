#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow* widget = new MainWindow();
    widget->show();

    return app.exec();
}