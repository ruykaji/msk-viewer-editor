#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include "CodeEditorWidget.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* t_parent, Qt::WindowFlags t_flags)
    : QMainWindow(t_parent, t_flags)
{
    CodeEditorWidget* codeEditor = new CodeEditorWidget(this);

    setCentralWidget(codeEditor);
    createActions();
    createMenus();

    setWindowTitle(tr("MSK Viewer and Editor"));
    setMinimumSize(480, 480);
    resize(1280, 720);
}

void MainWindow::newFile() {};
void MainWindow::open() {};
void MainWindow::save() {};

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    newAct = new QAction(tr("&Open"), this);
    newAct->setShortcuts(QKeySequence::Open);
    newAct->setStatusTip(tr("Open a file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::open);

    newAct = new QAction(tr("&Save"), this);
    newAct->setShortcuts(QKeySequence::Save);
    newAct->setStatusTip(tr("Save a file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}