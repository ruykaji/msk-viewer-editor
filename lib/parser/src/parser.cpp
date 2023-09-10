#include "parser.hpp"

std::vector<std::unique_ptr<Node>> Parser::parse(const std::vector<std::pair<TokenKind, std::string>>& t_tokens)
{
    std::vector<std::unique_ptr<Node>> nodes {};

    for (size_t i = 0; i < t_tokens.size(); ++i) {
        switch (t_tokens[i].first) {
        case TokenKind::Rec: {
            break;
        }
        }
    }

    return nodes;
};

TokenKind Parser::isCorrectSyntaxPattern(std::vector<TokenKind> t_tokenOrder)
{
}