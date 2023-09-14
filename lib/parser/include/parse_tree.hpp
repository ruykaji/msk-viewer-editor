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
    STATEMENT
};

struct Node {
    NodeKind kind {};

    bool isError { false };
    std::string message {};

    std::shared_ptr<Node> parent {};
    std::vector<std::shared_ptr<Node>> child {};

    Node(const NodeKind t_kind)
        : kind(t_kind) {};
};

struct TerminalNode : public Node {
    TokenKind kind {};
    std::string literal {};

    TerminalNode(const Token& t_token)
        : Node(NodeKind::TERMINAL)
        , kind(t_token.kind)
        , literal(t_token.literal) {};
};

enum class StatementKind {
    REC_CALL,
};

struct StatementNode : public Node {
    StatementKind kind {};

    StatementNode(const StatementKind& t_kind)
        : Node(NodeKind::STATEMENT)
        , kind(t_kind) {};
};
};

#endif