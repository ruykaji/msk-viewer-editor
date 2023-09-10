#include <sstream>

#include "lexer.hpp"

inline bool isNumber(const std::string& t_str)
{
    for (auto& ch : t_str) {
        if (std::isalpha(ch)) {
            return false;
        }
    }

    return true;
}

std::vector<std::pair<TokenKind, TokenValue>> Lexer::tokenize(const std::string& t_file)
{
    std::vector<std::pair<TokenKind, TokenValue>> tokens {};

    std::string token {};
    TokenKind tokenKind {};
    uint16_t position {};

    for (auto& ch : t_file) {
        tokenKind = isSeparator(ch);

        if (ch == ' ' || ch == '\t' || ch == '\n' || tokenKind != TokenKind::Word) {
            if (!token.empty()) {
                tokens.push_back(std::pair<TokenKind, TokenValue>(isKeyword(token), TokenValue(position - token.length(), position, token)));
                token.clear();
            }

            if (tokenKind != TokenKind::Word) {
                tokens.push_back(std::pair<TokenKind, TokenValue>(tokenKind, TokenValue(position + 1, position + 1, std::string(1, ch))));
            }
        } else {
            token += ch;
        }

        ++position;
    }

    return tokens;
}

TokenKind Lexer::isSeparator(const char t_ch)
{
    switch (t_ch) {
    case '(':
        return TokenKind::RoundBracketOpen;
    case ')':
        return TokenKind::RoundBracketOpen;
    case ',':
        return TokenKind::Comma;
    default:
        return TokenKind::Word;
    }
}

TokenKind Lexer::isKeyword(const std::string& t_str)
{
    if (t_str == "REC") {
        return TokenKind::Rec;
    }
    if (isNumber(t_str)) {
        return TokenKind::Number;
    }

    return TokenKind::Word;
}