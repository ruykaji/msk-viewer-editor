#include "lexer/include/lexer.hpp"
#include "unit_test.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

TEST_CASE("Lexer should produce right tokens.")
{
    std::string file { "SIMU #5.00\nREC ( -2, 120, 66, 30, NW )\nREC ( 49, 134, 1, 4, DP )" };

    static std::vector<TokenKind> tokensKind = {
        TokenKind::STRING,
        TokenKind::STRING,

        TokenKind::REC,
        TokenKind::LEFT_BRACE,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::STRING,
        TokenKind::RIGHT_BRACE,

        TokenKind::REC,
        TokenKind::LEFT_BRACE,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::NUMBER,
        TokenKind::COMMA,
        TokenKind::STRING,
        TokenKind::RIGHT_BRACE,

        TokenKind::END_OF_FILE
    };

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(file);

    ASSERT(tokens.size() == tokensKind.size());

    for (std::size_t i {}; i < tokens.size(); ++i) {
        ASSERT(tokensKind[i] == tokens[i].kind);
    }
}

TEST_CASE("Lexer token's should contain enough information to recreate original text.")
{
    std::string file { "SIMU #5.00\nREC ( -2, 120, 66, 30, NW )\nREC ( 49, 134, 1, 4, DP )" };
    std::string newFile {};

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(file);

    uint16_t cursor {};
    uint16_t line {};

    for (auto& token : tokens) {
        if (line < token.line) {
            ++line;
            newFile += '\n';
        }

        if (newFile.length() < token.start) {
            for (std::size_t i {}; i < token.start - newFile.length(); ++i) {
                newFile += ' ';
            }
        }

        newFile += token.literal;
    }

    ASSERT(file == newFile);
}