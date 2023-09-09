#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "ast.hpp"
#include "token_kind.hpp"

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::vector<std::unique_ptr<Node>> parse(const std::vector<std::pair<TokenKind, std::string>>& t_tokens);

private:
    TokenKind isCorrectSyntaxPattern(std::vector<TokenKind> t_tokenOrder);
};

#endif
