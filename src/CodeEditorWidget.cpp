#include <fstream>

#include "CodeEditorWidget.hpp"

CodeEditorWidget::CodeEditorWidget(QWidget* t_parent)
    : QPlainTextEdit(t_parent)
{
    std::ifstream fin { "F:/projects/LayoutReader/Libs/LayoutReader/Test/MskFiles/inv.msk" };
    std::string file {};
    char ch;

    while (fin >> std::noskipws >> ch) {
        file += ch;
    }

    setPlainText(QString::fromStdString(file));
};