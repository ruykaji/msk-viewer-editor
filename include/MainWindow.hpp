#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QMenu>

#include "CodeEditorWidget.hpp"
#include "ViewerWidget.hpp"
#include "lexer.hpp"
#include "parser.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT;

    Lexer* m_lexer {};
    Parser* m_parser {};

    QMenu* fileMenu;
    QAction* openAct;
    QAction* saveAct;
    QAction* exitAct;

    CodeEditorWidget* m_codeEditor {};
    ViewerWidget* m_viewerWidget {};

public:
    explicit MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private:
    void createActions();
    void createMenus();

private slots:
    void open();
    void save();
    void exit();
};

#endif