#ifndef __TOKEN_KIND_H__
#define __TOKEN_KIND_H__

enum class TokenKind {
    NONE,
    UNDEFINED,

    // Single one tokens.
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,

    // Literals.
    NUMBER,
    STRING,

    // Keywords.
    REC,

    END_OF_FILE
};

struct Token {
    TokenKind kind {};
    uint16_t start {};
    uint16_t line {};
    std::string literal {};

    Token() = default;
    ~Token() = default;

    Token(const TokenKind& t_kind, const uint16_t& t_start, const uint16_t& t_line, const std::string& t_literal)
        : kind(t_kind)
        , start(t_start)
        , line(t_line)
        , literal(t_literal) {};
};

#endif