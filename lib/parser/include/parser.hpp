#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

class Parser {
public:
    std::shared_ptr<Node> pt {};
    std::vector<std::shared_ptr<Rect>> ast {};

    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    void makePT(const std::vector<Token>& t_tokens);
    void makeAST();

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
