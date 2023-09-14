#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

class Parser {
public:
    std::shared_ptr<pt::Node> pt {};
    std::shared_ptr<ast::Node> ast {};

    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::shared_ptr<pt::Node> makePT(const std::vector<Token>& t_tokens);
    std::shared_ptr<ast::Node> makeAST();

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
