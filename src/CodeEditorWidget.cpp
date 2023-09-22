#include <QFile>
#include <fstream>

#include "CodeEditorWidget.hpp"

CodeEditorWidget::CodeEditorWidget(Lexer* t_lexer, Parser* t_parser, QWidget* t_parent)
    : m_lexer(t_lexer)
    , m_parser(t_parser)
    , QPlainTextEdit(t_parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumWidth(300);

    auto palette = QPalette();

    palette.setColor(QPalette::Base, QColor(32, 37, 49));
    palette.setColor(QPalette::Text, Qt::white);

    setPalette(palette);

    connect(this, &QPlainTextEdit::textChanged, this, &CodeEditorWidget::writeText);
};

void CodeEditorWidget::deepMakeText(QTextCursor& t_textCursor, QTextCharFormat& t_formater, uint16_t& t_line, const std::shared_ptr<Node>& t_iterator)
{
    if (t_iterator->kind != NodeKind::END_OF_PROGRAM) {
        if (t_iterator->kind == NodeKind::STATEMENT) {
            auto statement = std::static_pointer_cast<StatementNode>(t_iterator);

            switch (statement->stmKind) {
            case StatementKind::VERSION:
            case StatementKind::FIG: {
                for (auto& child : t_iterator->child) {
                    auto node = std::static_pointer_cast<TerminalNode>(child);

                    switch (node->kind) {
                    case TokenKind::VERSION:
                    case TokenKind::FIG: {
                        t_formater.setForeground(QBrush(QColor(50, 172, 237)));
                        break;
                    }
                    default:
                        t_formater.setForeground(QBrush(Qt::white));
                        break;
                    }

                    t_textCursor.insertText(QString::fromStdString(node->literal), t_formater);
                }
                break;
            }
            case StatementKind::REC_CALL: {
                for (auto& child : t_iterator->child) {
                    auto node = std::static_pointer_cast<TerminalNode>(child);

                    if (child->isError) {
                        t_formater.setUnderlineStyle(QTextCharFormat::WaveUnderline);
                        t_formater.setUnderlineColor(Qt::red);
                        t_formater.setToolTip(QString::fromStdString(child->message));
                    }

                    switch (node->kind) {
                    case TokenKind::REC:
                        t_formater.setForeground(QBrush(QColor(237, 172, 50)));
                        break;
                    case TokenKind::LEFT_BRACE:
                    case TokenKind::RIGHT_BRACE:
                        t_formater.setForeground(QBrush(QColor(50, 144, 237)));
                        break;
                    case TokenKind::NUMBER: {
                        t_formater.setForeground(QBrush(QColor(50, 244, 137)));
                        break;
                    }
                    case TokenKind::STRING: {
                        t_formater.setForeground(QBrush(QColor(250, 144, 137)));
                        break;
                    }
                    default:
                        t_formater.setForeground(QBrush(Qt::white));
                        break;
                    }

                    t_textCursor.insertText(QString::fromStdString(node->literal), t_formater);

                    if (child->isError) {
                        t_formater.setUnderlineStyle(QTextCharFormat::NoUnderline);
                        t_formater.setUnderlineColor(Qt::transparent);
                        t_formater.setToolTip(QString(""));
                    }
                }
                break;
            }
            case StatementKind::HELP: {
                for (auto& child : t_iterator->child) {
                    auto node = std::static_pointer_cast<TerminalNode>(child);
                    t_formater.setForeground(QBrush(QColor(50, 255, 50)));
                    t_formater.setFontItalic(true);
                    t_textCursor.insertText(QString::fromStdString(node->literal), t_formater);
                }

                t_formater.setFontItalic(false);

                break;
            }
            default:
                break;
            }
        } else {
            for (auto& child : t_iterator->child) {
                if (child->kind == NodeKind::STATEMENT) {
                    deepMakeText(t_textCursor, t_formater, t_line, child);
                } else if (child->kind != NodeKind::END_OF_PROGRAM) {
                    auto node = std::static_pointer_cast<TerminalNode>(child);
                    t_formater.setForeground(QBrush(Qt::white));
                    t_textCursor.insertText(QString::fromStdString(node->literal), t_formater);
                }
            }
        }
    }
}

void CodeEditorWidget::readFile(const QString& t_fileName)
{
    QFile file(t_fileName);
    QString text;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        text = in.readAll();

        auto __document = document();
        auto __textCursor = textCursor();

        __document->blockSignals(true);
        __document->clear();

        m_parser->makePT(m_lexer->tokenize(text.toStdString()));
        m_parser->makeAST();

        auto formater = QTextCharFormat();

        formater.setFontPointSize(14);

        uint16_t line {};

        deepMakeText(__textCursor, formater, line, m_parser->pt);

        __document->blockSignals(false);

        documentRecreated();

        file.close();
    }
}

void CodeEditorWidget::writeFile(const QString& t_fileName)
{
    QFile file(t_fileName);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << toPlainText();
        file.close();
    }
}

void CodeEditorWidget::writeText()
{
    auto __document = document();
    auto file = __document->toPlainText().toStdString();

    if (!file.empty()) {
        auto __textCursor = textCursor();
        auto cursorPosition = __textCursor.position();

        __document->blockSignals(true);
        __document->clear();
        __textCursor.setPosition(0);

        m_parser->makePT(m_lexer->tokenize(file));
        m_parser->makeAST();

        auto formater = QTextCharFormat();

        formater.setFontPointSize(12);

        m_line = 0;

        deepMakeText(__textCursor, formater, m_line, m_parser->pt);

        __textCursor.setPosition(cursorPosition > __textCursor.position() ? __textCursor.position() : cursorPosition);
        setTextCursor(__textCursor);

        __document->blockSignals(false);
    } else {
        m_parser->clear();
    }
}

void CodeEditorWidget::newRect()
{
    auto __document = document();
    auto __textCursor = textCursor();

    __document->blockSignals(true);

    auto formater = QTextCharFormat();

    formater.setFontPointSize(12);

    deepMakeText(__textCursor, formater, m_line, *m_parser->pt->child.rbegin());

    __document->blockSignals(false);
};