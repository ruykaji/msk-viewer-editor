#include <fstream>
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

inline bool isSpaceOnly(const std::string& t_str)
{
    for (auto& ch : t_str) {
        if (ch != ' ' || ch != '\t') {
            return false;
        }
    }

    return true;
}

std::vector<std::pair<TokenKind, std::string>> Lexer::tokenize(const std::string& t_fileName)
{
    std::vector<std::pair<TokenKind, std::string>> tokens {};
    std::ifstream fin { { t_fileName.begin(), t_fileName.end() } };

    if (fin.is_open()) {
        std::string token {};
        TokenKind tokenKind {};
        char ch;

        while (fin >> std::noskipws >> ch) {
            tokenKind = isSeparator(ch);

            if (tokenKind != TokenKind::Word) {
                if (tokenKind != TokenKind::WhiteSpace) {
                    tokens.push_back(std::pair<TokenKind, std::string>(tokenKind, std::to_string(ch)));

                    if (!token.empty()) {
                        tokenKind = isKeyword(token);

                        tokens.push_back(std::pair<TokenKind, std::string>(tokenKind, token));
                        token.clear();
                    }
                } else {
                    token += ch;
                }
            } else {
                if (isSpaceOnly(token)) {
                    tokens.push_back(std::pair<TokenKind, std::string>(TokenKind::WhiteSpace, token));
                    token.clear();
                }

                token += ch;
            }
        }
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
    case ' ':
    case '\t':
        return TokenKind::WhiteSpace;
    case '\n':
        return TokenKind::NewLine;
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