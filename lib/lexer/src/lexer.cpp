#include <sstream>

#include "lexer.hpp"

std::vector<Token> Lexer::tokenize(const std::string& t_file)
{
    reset();

    Token tokenFromSymbol {};
    Token tokenFromLexeme {};

    for (char ch : t_file) {
        tokenFromSymbol = scanSymbol(ch);

        if (tokenFromSymbol.kind != TokenKind::NONE) {
            tokenFromLexeme = scanLexeme();

            if (tokenFromLexeme.kind != TokenKind::NONE) {
                m_tokens.emplace_back(tokenFromLexeme);
                m_lexeme.clear();
            }

            if (tokenFromSymbol.kind != TokenKind::UNDEFINED && tokenFromSymbol.kind != TokenKind::NEW_LINE) {
                m_tokens.emplace_back(tokenFromSymbol);
            } else if (tokenFromSymbol.kind == TokenKind::UNDEFINED && m_tokens.back().kind == TokenKind::UNDEFINED) {
                m_tokens.rbegin()->literal += tokenFromSymbol.literal;
            } else if (tokenFromSymbol.kind == TokenKind::NEW_LINE && m_tokens.back().kind == TokenKind::NEW_LINE) {
                m_tokens.rbegin()->literal += tokenFromSymbol.literal;
            } else {
                m_tokens.emplace_back(tokenFromSymbol);
            }
        }
    }

    if (!m_lexeme.empty()) {
        tokenFromLexeme = scanLexeme();
        m_tokens.emplace_back(tokenFromLexeme);
        m_lexeme.clear();
    }

    m_tokens.emplace_back(Token(TokenKind::END_OF_FILE, ""));

    return m_tokens;
}

Token Lexer::scanSymbol(const char& t_ch)
{
    switch (t_ch) {
    case ' ':
    case '\t':
    case '\f':
    case '\v':
        return Token(TokenKind::UNDEFINED, std::string(1, t_ch));
    case '\r':
    case '\n':
        return Token(TokenKind::NEW_LINE, std::string(1, t_ch));
    case '(':
        return Token(TokenKind::LEFT_BRACE, "(");
    case ')':
        return Token(TokenKind::RIGHT_BRACE, ")");
    case ',':
        return Token(TokenKind::COMMA, ",");
    default:
        m_lexeme += t_ch;
        return Token(TokenKind::NONE, "");
    }
}

Token Lexer::scanLexeme()
{
    if (!m_lexeme.empty()) {
        if (m_lexeme == "REC") {
            return Token(TokenKind::REC, m_lexeme);
        }
        if (m_lexeme == "VERSION") {
            return Token(TokenKind::VERSION, m_lexeme);
        }
        if (m_lexeme == "FIG" || m_lexeme == "FFIG") {
            return Token(TokenKind::FIG, m_lexeme);
        }
        if (m_lexeme == "HELP") {
            return Token(TokenKind::HELP, m_lexeme);
        }
        if (isNumber(m_lexeme)) {
            return Token(TokenKind::NUMBER, m_lexeme);
        }

        return Token(TokenKind::STRING, m_lexeme);
    }

    return Token(TokenKind::NONE, "");
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

void Lexer::reset()
{
    m_tokens.clear();
    m_lexeme.clear();
}