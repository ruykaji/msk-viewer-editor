#ifndef __PARSE_TREE_H__
#define __PARSE_TREE_H__

#include <cstdint>

struct Node {
    uint16_t start {};
    uint16_t end {};
};

enum class ExprKind {
    NUMBER_EXPR,
    LITERAL_EXPR,
};

struct ExprNode : public Node {
    ExprKind kind;
};

enum class StmtKind {
    CALL_EXPR,
};

struct StmtNode : public Node {
    StmtKind kind;
};

#endif