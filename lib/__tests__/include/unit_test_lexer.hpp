#include "lexer/include/lexer.hpp"
#include "unit_test.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

static std::vector<std::string> tokensName = {
    "Word",
    "Number",
    "RoundBracketOpen",
    "RoundBracketClose",
    "Comma",
    "Rec",
};

TEST_CASE("Lexer manually checking.")
{
    std::ifstream fin { "F:/projects/LayoutReader/Libs/LayoutReader/Test/MskFiles/inv.msk" };
    std::string file {};
    char ch;

    while (fin >> std::noskipws >> ch) {
        file += ch;
    }

    Lexer lexer;
    std::vector<std::pair<TokenKind, TokenValue>> tokens = lexer.tokenize(file);

    for (auto& [first, second] : tokens) {
        std::cout << std::setw(20) << tokensName[static_cast<int>(first)] << " |" << second.value << "|\n";
    }

    std::cout << '\n'
              << std::flush;

    ASSERT(true);
}