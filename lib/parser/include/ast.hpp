#ifndef __AST_H__
#define __AST_H__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "token_kind.hpp"

struct Node {
    TokenKind tokenKind {};

    Node(const TokenKind& t_tokenKind)
        : tokenKind(t_tokenKind) {};
};

struct LiteralNode : public Node {
    std::string value {};

    LiteralNode(const TokenKind& t_tokenKind, const std::string t_value)
        : Node(t_tokenKind)
        , value(t_value) {};
};

struct StatementNode : public Node {
    bool isWarning { false };
    bool isError { false };
    std::string message {};
    std::vector<std::unique_ptr<Node>> children {};

    StatementNode(const TokenKind& t_tokenKind)
        : Node(t_tokenKind) {};
};

#endif