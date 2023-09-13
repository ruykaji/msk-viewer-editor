#include "lexer/include/lexer.hpp"
#include "parser/include/parser.hpp"
#include "unit_test.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

TEST_CASE("Lexer should produce right tokens.")
{
    std::string file { "REC(-2,120,66,30,NW)\nREC(-2,120,66,30,NW)" };

    Lexer lexer;
    Parser parser;

    auto tokens = lexer.tokenize(file);
    auto tree = parser.makeTree(tokens);
}