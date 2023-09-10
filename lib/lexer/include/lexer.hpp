#ifndef __LEXER_H__
#define __LEXER_H__

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "token_kind.hpp"

struct TokenValue {
    uint16_t start {};
    uint16_t end {};
    std::string value {};

    TokenValue(const uint16_t& t_start, const uint16_t& t_end, const std::string& t_value)
        : start(t_start)
        , end(t_end)
        , value(t_value) {};
};

class Lexer {
public:
    Lexer() = default;
    ~Lexer() = default;

    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    std::vector<std::pair<TokenKind, TokenValue>> tokenize(const std::string& t_fileName);

private:
    TokenKind isKeyword(const std::string& t_str);
    TokenKind isSeparator(const char ch);
};

#endif