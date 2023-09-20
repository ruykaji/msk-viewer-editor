#ifndef __CODE_EDITOR_WIDGET_H__
#define __CODE_EDITOR_WIDGET_H__

#include <QPlainTextEdit>

#include "lexer.hpp"
#include "parser.hpp"

class CodeEditorWidget : public QPlainTextEdit {
    Q_OBJECT;

    Lexer* m_lexer {};
    Parser* m_parser {};

    uint16_t m_line {};

public:
    explicit CodeEditorWidget(Lexer* t_lexer, Parser* t_parser, QWidget* t_parent = nullptr);

    void readFile(const QString& t_fileName);
    void writeFile(const QString& t_fileName);

private:
    void deepMakeText(QTextCursor& t_textCursor, QTextCharFormat& t_formater, uint16_t& t_line, const std::shared_ptr<Node>& t_iterator);

public slots:
    void writeText();
    void newRect();
signals:
    void documentRecreated();
};

#endif