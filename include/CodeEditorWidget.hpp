#ifndef __CODE_EDITOR_WIDGET_H__
#define __CODE_EDITOR_WIDGET_H__

#include <QPlainTextEdit>

class CodeEditorWidget : public QPlainTextEdit {

    Q_OBJECT;

public:
    CodeEditorWidget(QWidget* t_parent = nullptr);
};

#endif