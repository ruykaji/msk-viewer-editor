#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include "CodeEditorWidget.hpp"
#include "MainWindow.hpp"
#include "ViewerWidget.hpp"

MainWindow::MainWindow(QWidget* t_parent, Qt::WindowFlags t_flags)
    : QMainWindow(t_parent, t_flags)
{
    m_lexer = new Lexer();
    m_parser = new Parser();

    auto codeEditor = new CodeEditorWidget(m_lexer, m_parser, this);
    auto viewerWidget = new ViewerWidget(m_parser, this);

    connect(codeEditor, &CodeEditorWidget::textChanged, viewerWidget, &ViewerWidget::redraw);

    codeEditor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    codeEditor->setMaximumWidth(300);

    viewerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto __centralWidget = new QWidget(this);
    auto hbox = new QHBoxLayout(__centralWidget);

    hbox->addWidget(codeEditor);
    hbox->addWidget(viewerWidget);
    hbox->setContentsMargins(0, 0, 0, 0);

    __centralWidget->setLayout(hbox);

    setCentralWidget(__centralWidget);
    createActions();
    createMenus();

    setWindowTitle(tr("MSK Viewer and Editor"));
    setMinimumSize(480, 480);
    resize(1280, 720);
}

void MainWindow::newFile() {};
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open topology"), "/home", tr("Topology (*.msk *.MSK)"));
};
void MainWindow::save() {};

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    // fileMenu->addAction(exitAct);
}