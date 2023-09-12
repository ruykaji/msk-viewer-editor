#include <sstream>

#include "lexer.hpp"

std::vector<Token> Lexer::tokenize(const std::string& t_file)
{
    Token tokenFromSymbol {};
    Token tokenFromLexeme {};

    for (char ch : t_file) {
        tokenFromSymbol = scanSymbol(ch);

        if (tokenFromSymbol.kind != TokenKind::NONE) {
            tokenFromLexeme = scanLexeme();

            if (tokenFromLexeme.kind != TokenKind::NONE) {
                m_tokens.push_back(tokenFromLexeme);
                m_lexeme.clear();
            }

            if (tokenFromSymbol.kind != TokenKind::UNDEFINED) {
                m_tokens.push_back(tokenFromSymbol);
            } else if (ch == '\t') {
                m_cursor += 2;
            } else if (ch == '\n') {
                ++m_line;
            }
        }

        ++m_cursor;
    }

    m_tokens.push_back(Token(TokenKind::END_OF_FILE, m_cursor, m_line, ""));

    return m_tokens;
}

Token Lexer::scanSymbol(const char& t_ch)
{
    switch (t_ch) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
    case '\v':
        return Token(TokenKind::UNDEFINED, 0, 0, "");
    case '(':
        return Token(TokenKind::LEFT_BRACE, m_cursor, m_line, "(");
    case ')':
        return Token(TokenKind::RIGHT_BRACE, m_cursor, m_line, ")");
    case ',':
        return Token(TokenKind::COMMA, m_cursor, m_line, ",");
    default:
        m_lexeme += t_ch;
        return Token(TokenKind::NONE, 0, 0, "");
    }
}

Token Lexer::scanLexeme()
{
    if (!m_lexeme.empty()) {
        if (m_lexeme == "REC") {
            return Token(TokenKind::REC, m_cursor - m_lexeme.length(), m_line, m_lexeme);
        }

        if (isNumber(m_lexeme)) {
            return Token(TokenKind::NUMBER, m_cursor - m_lexeme.length(), m_line, m_lexeme);
        }

        return Token(TokenKind::STRING, m_cursor - m_lexeme.length(), m_line, m_lexeme);
    }

    return Token(TokenKind::NONE, 0, 0, "");
}

bool Lexer::isNumber(const std::string& t_str)
{
    if (t_str.empty()) {
        return false;
    }

    char ch;

    for (std::size_t i {}; i < t_str.length(); ++i) {
        ch = t_str.at(i);

        if (i == 0 && (ch == '+' || ch == '-')) {
            continue;
        }

        if (!std::isdigit(ch)) {
            return false;
        }
    }

    return true;
}