#ifndef __PARSE_TREE_H__
#define __PARSE_TREE_H__

#include "token.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace pt {
enum class NodeKind {
    PROGRAM,
    TERMINAL,
    STATEMENT,
    END_OF_PROGRAM
};

struct Node {
    NodeKind kind {};

    bool isError { false };
    std::string message {};

    std::shared_ptr<Node> parent {};
    std::vector<std::shared_ptr<Node>> child {};

    Node(const NodeKind& t_kind, const std::shared_ptr<Node>& t_parent)
        : kind(t_kind)
        , parent(t_parent) {};
};

struct TerminalNode : public Node {
    TokenKind kind {};
    std::string literal {};

    TerminalNode(const Token& t_token, const std::shared_ptr<Node>& t_parent)
        : Node(NodeKind::TERMINAL, t_parent)
        , kind(t_token.kind)
        , literal(t_token.literal) {};
};

enum class StatementKind {
    REC_CALL,
};

struct StatementNode : public Node {
    StatementKind kind {};

    StatementNode(const StatementKind& t_kind, const std::shared_ptr<Node>& t_parent)
        : Node(NodeKind::STATEMENT, t_parent)
        , kind(t_kind) {};
};
};

#endif