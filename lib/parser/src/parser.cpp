#include <algorithm>

#include "parser.hpp"

inline bool isNumber(const std::string& t_str)
{
    for (std::size_t i {}; i < t_str.length(); ++i) {
        if (i == 0 && t_str.length() > 1 && (t_str[0] == '-' || t_str[0] == '+')) {
            continue;
        }

        if (!std::isdigit(t_str[i])) {
            return false;
        }
    }

    return true;
}

/*Parse tree methods*/
/*==========================================================================*/

void Parser::makePT(const std::vector<Token>& t_tokens)
{
    pt = std::make_shared<Node>(NodeKind::PROGRAM, nullptr);

    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        makePTStep(tokensIterator);
    }

    pt->child.emplace_back(std::make_shared<Node>(Node(NodeKind::END_OF_PROGRAM, pt)));
}

void Parser::makePTStep(std::vector<Token>::const_iterator& t_tokensIterator)
{
    switch (t_tokensIterator->kind) {
    case TokenKind::REC: {
        auto node = std::make_shared<StatementNode>(StatementNode(StatementKind::REC_CALL, pt));
        node->parent = pt;

        pt->child.emplace_back(node);
        pt = node;

        pt->child.emplace_back(std::make_shared<TerminalNode>(TerminalNode(*t_tokensIterator++, pt)));

        makePTREC(t_tokensIterator);

        pt = pt->parent;
        break;
    }
    default: {
        pt->child.emplace_back(std::make_shared<TerminalNode>(TerminalNode(*t_tokensIterator++, pt)));
        break;
    }
    }
}

void Parser::makePTREC(std::vector<Token>::const_iterator& t_tokensIterator)
{
    while (t_tokensIterator->kind == TokenKind::UNDEFINED) {
        makePTStep(t_tokensIterator);
    }

    if (t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        pt->isError = true;
        pt->child.at(0)->isError = true;
        pt->child.at(0)->message = "There is no declaration for REC statement!";

        return;
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::LEFT_BRACE: {
            break;
        }
        case TokenKind::COMMA: {
            break;
        }
        case TokenKind::NUMBER: {
            if (pt->child.size() > 0) {
                for (auto i = pt->child.rbegin(); i != pt->child.rend(); ++i) {
                    auto node = std::static_pointer_cast<TerminalNode>((*i));

                    if (node->kind == TokenKind::COMMA) {
                        break;
                    }

                    if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                        pt->isError = true;
                        node->isError = true;
                        node->message = "Separator is needed!";
                        break;
                    }
                }
            }
            break;
        }
        case TokenKind::STRING: {
            if (pt->child.size() > 0 && pt->child.back()->kind == NodeKind::TERMINAL) {
                for (auto i = pt->child.rbegin(); i != pt->child.rend(); ++i) {
                    auto node = std::static_pointer_cast<TerminalNode>((*i));

                    if (node->kind == TokenKind::COMMA) {
                        break;
                    }

                    if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                        pt->isError = true;
                        node->isError = true;
                        node->message = "Separator is needed!";
                        break;
                    }
                }
            }

            break;
        }
        case TokenKind::RIGHT_BRACE: {
            makePTStep(t_tokensIterator);
            return;
        }
        default:
            break;
        }

        if (t_tokensIterator->kind != TokenKind::REC) {
            makePTStep(t_tokensIterator);
        } else {
            break;
        }
    }

    if (!pt->isError) {
        pt->isError = true;
        pt->child.at(0)->isError = true;
        pt->child.at(0)->message = "Statement is not closed!";
    }
}

/*Abstract syntax tree methods*/
/*==========================================================================*/

void Parser::makeAST()
{
    ast.clear();

    if (pt->kind != NodeKind::END_OF_PROGRAM) {
        for (auto& child : pt->child) {
            if (child->kind == NodeKind::STATEMENT && !child->isError) {
                auto node = std::static_pointer_cast<StatementNode>(child);
                pt = node;

                makeASTREC();

                pt = node->parent;
            }
        }
    }

    std::sort(ast.begin(), ast.end(), [](auto& el1, auto el2) {
        return static_cast<uint16_t>(el1->material) < static_cast<uint16_t>(el2->material);
    });
}

void Parser::makeASTREC()
{
    std::vector<int16_t> numericArgs {};
    std::string literalArg {};

    for (auto& child : pt->child) {
        if (child->kind == NodeKind::TERMINAL) {
            auto node = std::static_pointer_cast<TerminalNode>(child);

            switch (node->kind) {
            case TokenKind::NUMBER: {
                if (isNumber(node->literal)) {
                    numericArgs.emplace_back(std::stoi(node->literal));
                }
                break;
            }
            case TokenKind::STRING: {
                literalArg = node->literal;
                break;
            }
            default:
                break;
            }
        }
    }

    if (numericArgs.size() == 4 && literalArg != "") {
        Rect::Material material {};

        if (literalArg == "NW") {
            material = Rect::Material::NW;
        } else if (literalArg == "DN") {
            material = Rect::Material::DN;
        } else if (literalArg == "DP") {
            material = Rect::Material::DP;
        } else if (literalArg == "PO") {
            material = Rect::Material::PO;
        } else if (literalArg == "PO2") {
            material = Rect::Material::PO2;
        } else if (literalArg == "ME") {
            material = Rect::Material::ME;
        } else if (literalArg == "CO") {
            material = Rect::Material::CO;
        } else if (literalArg == "M2") {
            material = Rect::Material::M2;
        } else if (literalArg == "VI") {
            material = Rect::Material::VI;
        } else if (literalArg == "M3") {
            material = Rect::Material::M3;
        } else if (literalArg == "V2") {
            material = Rect::Material::V2;
        } else if (literalArg == "M4") {
            material = Rect::Material::NW;
        } else if (literalArg == "V3") {
            material = Rect::Material::V3;
        } else if (literalArg == "M5") {
            material = Rect::Material::NW;
        } else if (literalArg == "V4") {
            material = Rect::Material::V4;
        } else if (literalArg == "M6") {
            material = Rect::Material::M6;
        } else if (literalArg == "V5") {
            material = Rect::Material::V5;
        }

        auto rect = std::make_shared<Rect>(numericArgs[0], numericArgs[1], numericArgs[2], numericArgs[3], material);
        rect->source = pt;

        ast.emplace_back(rect);
    }
}