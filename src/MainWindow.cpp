#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* t_parent, Qt::WindowFlags t_flags)
    : QMainWindow(t_parent, t_flags)
{
    m_lexer = new Lexer();
    m_parser = new Parser();
    m_codeEditor = new CodeEditorWidget(m_lexer, m_parser, this);
    m_viewerWidget = new ViewerWidget(m_parser, this);

    connect(m_codeEditor, &CodeEditorWidget::textChanged, m_viewerWidget, [this]() { this->m_viewerWidget->update(); });
    connect(m_codeEditor, &CodeEditorWidget::documentRecreated, m_viewerWidget, &ViewerWidget::setNewScaling);
    connect(m_viewerWidget, &ViewerWidget::newRect, m_codeEditor, &CodeEditorWidget::newRect);

    QSplitter* splitter = new QSplitter(this);

    splitter->addWidget(m_codeEditor);
    splitter->addWidget(m_viewerWidget);
    splitter->setSizes(QList<int>() << 300 << 980);

    setCentralWidget(splitter);
    createActions();
    createMenus();

    setWindowTitle("MSK Viewer and Editor");
    setMinimumSize(480, 480);
    resize(1280, 720);
}

void MainWindow::createActions()
{
    newAct = new QAction("&New", this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new file");
    connect(newAct, &QAction::triggered, m_codeEditor, &CodeEditorWidget::clear);
    connect(newAct, &QAction::triggered, m_viewerWidget, &ViewerWidget::clear);

    openAct = new QAction("&Open", this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip("Open a file");
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction("&Save", this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save a file");
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction("&Exit", this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip("Exit from the program");
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open topology", "/home", "Topology (*.msk *.MSK)");

    m_codeEditor->readFile(fileName);

    setWindowTitle("MSK Viewer and Editor - " + fileName);
};

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Open topology", "/home", "Topology (*.msk *.MSK)");

    m_codeEditor->writeFile(fileName);
};