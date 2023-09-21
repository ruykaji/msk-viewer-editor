#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <set>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

class Parser {
public:
    std::shared_ptr<Node> pt {};
    std::vector<std::shared_ptr<Rect>> ast {};

    Parser();
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    void makePT(const std::vector<Token>& t_tokens);
    void makeAST();
    void addRECNode(const int16_t& t_left, const int16_t& t_top, const int16_t& t_width, const int16_t& t_height, const Rect::Material& t_material);

private:
    /*Parse tree methods*/
    /*==========================================================================*/

    void makePTREC(const std::shared_ptr<Node>& t_parent, std::vector<Token>::const_iterator& t_tokensIterator);

    /*Abstract syntax tree methods*/
    /*==========================================================================*/

    void makeASTREC(const std::shared_ptr<Node>& t_parent);
};

#endif
