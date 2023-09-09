#include "parser.hpp"

inline std::unique_ptr<Node> makeNode(std::pair<TokenKind, std::string>& t_token)
{
    switch (t_token.first) {
    case TokenKind::Word:
    case TokenKind::Number:
    case TokenKind::RoundBracketOpen:
    case TokenKind::RoundBracketClose:
    case TokenKind::Comma:
    case TokenKind::WhiteSpace:
    case TokenKind::Tab:
    case TokenKind::NewLine: {
        return std::make_unique<Node>(LiteralNode(t_token.first, t_token.second));
    }
    case TokenKind::Rec: {
        return std::make_unique<Node>(StatementNode(t_token.first));
    }
    }
}

std::vector<std::unique_ptr<Node>> Parser::parse(const std::vector<std::pair<TokenKind, std::string>>& t_tokens)
{
    std::vector<std::unique_ptr<Node>> nodes {};

    for (size_t i = 0; i < t_tokens.size(); ++i) {
        switch (t_tokens[i].first) {
        case TokenKind::Rec:{
                  
            break;
        }
        }
    }

    return nodes;
};

TokenKind Parser::isCorrectSyntaxPattern(std::vector<TokenKind> t_tokenOrder){

}