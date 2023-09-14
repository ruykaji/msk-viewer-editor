#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

class Parser {
    std::shared_ptr<pt::Node> m_pt {};
    std::shared_ptr<ast::Node> m_ast {};

public:
    Parser();
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
