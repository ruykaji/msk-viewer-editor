#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QMenu>

class MainWindow : public QMainWindow {
    Q_OBJECT;

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