#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <string_view>
#include <vector>

#include "token_kind.hpp"

class Lexer {
public:
    Lexer() = default;
    ~Lexer() = default;

    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    std::vector<std::pair<TokenKind, std::string>> tokenize(const std::string& t_fileName);

private:
    TokenKind isKeyword(const std::string& t_str);
    TokenKind isSeparator(const char ch);
};

#endif