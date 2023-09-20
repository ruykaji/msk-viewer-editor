#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <set>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

static auto __compare = [](auto& el1, auto el2) { return static_cast<uint16_t>(el1->material) <= static_cast<uint16_t>(el2->material); };

class Parser {
public:
    std::shared_ptr<Node> pt {};
    std::set<std::shared_ptr<Rect>, decltype(__compare)> ast {};

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

    void makePTStep(std::vector<Token>::const_iterator& t_tokensIterator);
    void makePTREC(std::vector<Token>::const_iterator& t_tokensIterator);

    /*Abstract syntax tree methods*/
    /*==========================================================================*/

    void makeASTStep();
    void makeASTREC();
};

#endif
