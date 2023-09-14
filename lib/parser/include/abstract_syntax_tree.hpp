#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace ast {
enum class NodeKind {
    PROGRAM,
    NUMBER_NODE,
    STRING_NODE,
    REC_NODE
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

struct NumberNode : public Node {
    uint16_t value;
    
    NumberNode(const uint16_t& t_value, const std::shared_ptr<Node>& t_parent)
        : Node(NodeKind::NUMBER_NODE, t_parent)
        , value(t_value) {};
};

struct StringNode : public Node {
    std::string value;

    StringNode(const std::string& t_value, const std::shared_ptr<Node>& t_parent)
        : Node(NodeKind::STRING_NODE, t_parent)
        , value(t_value) {};
};

struct RECNode : public Node {

    RECNode(const std::shared_ptr<Node>& t_parent)
        : Node(NodeKind::REC_NODE, t_parent) {};
};

};

#endif