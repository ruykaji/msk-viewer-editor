#include "lexer/include/lexer.hpp"
#include "parser/include/parser.hpp"
#include "unit_test.hpp"

#include <fstream>
#include <iostream>

TEST_CASE("Parser should produce correct parse tree.")
{
    std::string file { "REC(-2,120,66,30,NW)  \n\n\n  REC ( 49, 134, 1, 4, DP )\n" };

    Lexer lexer;
    Parser parser;

    auto tokens = lexer.tokenize(file);
    auto tree = parser.makePT(tokens);

    ASSERT(tree->kind == pt::NodeKind::PROGRAM);
    ASSERT(tree->child.size() == 5);
    ASSERT(tree->child.at(0)->kind == pt::NodeKind::STATEMENT);
    ASSERT(tree->child.at(1)->kind == pt::NodeKind::TERMINAL);

    auto statementOne = std::static_pointer_cast<pt::StatementNode>(tree->child.at(0));

    ASSERT(statementOne->child.size() == 12);

    for (auto& child : statementOne->child) {
        auto node = std::static_pointer_cast<pt::TerminalNode>(child);

        ASSERT(!node->isError);
    }
}

void deep(uint16_t& t_line, std::string& t_str, std::shared_ptr<pt::Node>& t_iterator)
{
    for (auto& child : t_iterator->child) {
        if (child->kind == pt::NodeKind::TERMINAL) {
            auto node = std::static_pointer_cast<pt::TerminalNode>(child);

            t_str += node->literal;
        } else if (child->kind == pt::NodeKind::STATEMENT) {
            deep(t_line, t_str, child);
        }
    }
}

TEST_CASE("Parser tree should contain enough information to recreate original text.")
{
    std::string file { "REC(-2,120,66,30,NW)  \n\n\n  REC ( 49, 134, 1, 4, DP " };
    std::string newFile {};

    Lexer lexer;
    Parser parser;

    auto tokens = lexer.tokenize(file);
    auto tree = parser.makePT(tokens);

    uint16_t line {};

    deep(line, newFile, tree);

    ASSERT(file == newFile);
}