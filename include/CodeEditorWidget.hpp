#ifndef __CODE_EDITOR_WIDGET_H__
#define __CODE_EDITOR_WIDGET_H__

#include <QPlainTextEdit>

#include "lexer.hpp"
#include "parser.hpp"

class CodeEditorWidget : public QPlainTextEdit {
    Q_OBJECT;

    Lexer* m_lexer {};
    Parser* m_parser {};

public:
    CodeEditorWidget(Lexer* t_lexer, Parser* t_parser, QWidget* t_parent = nullptr);

private:
    void deepMakeText(QTextCursor& t_textCursor, QTextCharFormat& t_formater, uint16_t& t_line, std::shared_ptr<pt::Node>& t_iterator);

public slots:
    void writeText();
};

#endif