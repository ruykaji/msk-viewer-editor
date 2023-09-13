#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "parse_tree.hpp"

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::shared_ptr<StatementNode> makeTree(const std::vector<Token>& t_tokens);

private:
    void makeStep(std::shared_ptr<Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator);
    void makeREC(std::shared_ptr<Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator);
};

#endif
