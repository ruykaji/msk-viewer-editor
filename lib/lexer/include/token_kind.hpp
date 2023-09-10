#ifndef __TOKEN_KIND_H__
#define __TOKEN_KIND_H__

enum class TokenKind {
    Word,
    Number,

    RoundBracketOpen,
    RoundBracketClose,
    Comma,

    Rec,
};

#endif