#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <string>

#include "abstract_syntax_tree.hpp"
#include "parse_tree.hpp"

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::shared_ptr<pt::Node> makePT(const std::vector<Token>& t_tokens);
    std::shared_ptr<ast::Node> makeAST(const std::shared_ptr<pt::Node>& t_parseTree);

private:
    /*Parse tree methods*/
    /*==========================================================================*/

    void makePTStep(std::shared_ptr<pt::Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator);
    void makePTREC(std::shared_ptr<pt::Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator);

    /*Abstract syntax tree methods*/
    /*==========================================================================*/
    
    void makeASTStep(std::shared_ptr<pt::Node>& t_treeIterator);
    void makeASTREC(std::shared_ptr<pt::Node>& t_treeIterator);
};

#endif
