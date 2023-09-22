#ifndef __TOKEN_KIND_H__
#define __TOKEN_KIND_H__

enum class TokenKind {
    NONE,
    UNDEFINED,
    NEW_LINE,

    // Single one tokens.
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,

    // Literals.
    NUMBER,
    STRING,

    // Keywords.
    HELP,
    VERSION,
    FIG,
    REC,

    END_OF_FILE
};

struct Token {
    TokenKind kind {};
    std::string literal {};

    Token() = default;
    ~Token() = default;

    Token(const TokenKind& t_kind, const std::string& t_literal)
        : kind(t_kind)
        , literal(t_literal) {};
};

#endif