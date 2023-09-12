#ifndef __LEXER_H__
#define __LEXER_H__

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "token.hpp"
class Lexer {

    std::vector<Token> m_tokens {};
    std::string m_lexeme {};
    uint16_t m_cursor {};
    uint16_t m_line {};

public:
    Lexer() = default;
    ~Lexer() = default;

    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    std::vector<Token> tokenize(const std::string& t_fileName);

private:
    Token scanSymbol(const char& t_ch);
    Token scanLexeme();
    bool isNumber(const std::string& t_str);
};

#endif