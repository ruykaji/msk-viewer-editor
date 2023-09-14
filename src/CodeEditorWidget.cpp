#include <fstream>

#include "CodeEditorWidget.hpp"

CodeEditorWidget::CodeEditorWidget(Lexer* t_lexer, Parser* t_parser, QWidget* t_parent)
    : m_lexer(t_lexer), m_parser(t_parser), QPlainTextEdit(t_parent)
{
    auto palette = QPalette();

    palette.setColor(QPalette::Base, QColor(42, 47, 59));
    palette.setColor(QPalette::Text, Qt::white);

    setPalette(palette);

    std::ifstream fin { "/home/alaie/projects/inv.msk" };
    std::string file {};
    char ch;

    while (fin >> std::noskipws >> ch) {
        file += ch;
    }

    auto tokens = m_lexer->tokenize(file);
    auto tree = m_parser->makePT(tokens);

    m_parser->makeAST();

    auto formater = QTextCharFormat();
    auto __textCursor = textCursor();

    formater.setFontPointSize(12);

    uint16_t line {};

    deepMakeText(__textCursor, formater, line, tree);

    connect(this, &QPlainTextEdit::textChanged, this, &CodeEditorWidget::writeText);
};

void CodeEditorWidget::deepMakeText(QTextCursor& t_textCursor, QTextCharFormat& t_formater, uint16_t& t_line, std::shared_ptr<pt::Node>& t_iterator)
{
    if (t_iterator->kind != pt::NodeKind::END_OF_PROGRAM) {
        if (t_iterator->isError) {
            t_formater.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            t_formater.setUnderlineColor(Qt::red);
            t_formater.setToolTip(QString::fromStdString(t_iterator->message));
        } else {
            t_formater.setUnderlineStyle(QTextCharFormat::NoUnderline);
            t_formater.setUnderlineColor(Qt::transparent);
            t_formater.setToolTip(QString(""));
        }

        for (auto& child : t_iterator->child) {
            if (child->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(child);

                switch (node->kind) {
                case TokenKind::REC:
                    t_formater.setForeground(QBrush(QColor(237, 172, 50)));
                    break;
                case TokenKind::LEFT_BRACE:
                case TokenKind::RIGHT_BRACE:
                    t_formater.setForeground(QBrush(QColor(50, 144, 237)));
                    break;
                default:
                    t_formater.setForeground(QBrush(Qt::white));
                    break;
                }

                t_textCursor.insertText(QString::fromStdString(node->literal), t_formater);

            } else if (child->kind == pt::NodeKind::STATEMENT) {
                deepMakeText(t_textCursor, t_formater, t_line, child);
            }
        }
    }
}

void CodeEditorWidget::writeText()
{
    auto __document = document();
    auto __textCursor = textCursor();
    auto file = __document->toPlainText().toStdString();
    auto cursorPosition = __textCursor.position();

    __document->blockSignals(true);
    __document->clear();
    __textCursor.setPosition(0);

    auto tokens = m_lexer->tokenize(file);
    auto tree = m_parser->makePT(tokens);

    m_parser->makeAST();

    auto formater = QTextCharFormat();

    formater.setFontPointSize(12);

    uint16_t line {};

    deepMakeText(__textCursor, formater, line, tree);

    __textCursor.setPosition(cursorPosition > __textCursor.position() ? __textCursor.position() : cursorPosition);
    setTextCursor(__textCursor);

    __document->blockSignals(false);
}