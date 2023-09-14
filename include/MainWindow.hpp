#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QMenu>

#include "lexer.hpp"
#include "parser.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT;

    Lexer lexer {};
    Parser parser {};

    QMenu* fileMenu;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* exitAct;

public:
    MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private:
    void createActions();
    void createMenus();

private slots:
    void newFile();
    void open();
    void save();
};

#endif