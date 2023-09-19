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
    splitter->setSizes(QList<int>() << 200 << 1080);

    setCentralWidget(splitter);
    createActions();
    createMenus();

    setWindowTitle(tr("MSK Viewer and Editor"));
    setMinimumSize(480, 480);
    resize(1280, 720);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit from the program"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open topology"), "/home", tr("Topology (*.msk *.MSK)"));

    m_codeEditor->readFile(fileName);
};

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open topology"), "/home", tr("Topology (*.msk *.MSK)"));

    m_codeEditor->writeFile(fileName);
};

void MainWindow::exit()
{
    close();
};