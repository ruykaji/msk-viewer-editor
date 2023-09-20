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

Parser::Parser()
{
    pt = std::make_shared<Node>(NodeKind::PROGRAM);
}

/*Parse tree methods*/
/*==========================================================================*/

void Parser::makePT(const std::vector<Token>& t_tokens)
{
    pt = std::make_shared<Node>(NodeKind::PROGRAM);

    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (tokensIterator->kind) {
        case TokenKind::REC: {
            auto node = std::make_shared<StatementNode>(StatementKind::REC_CALL);
            pt->child.emplace_back(node);

            makePTREC(node, tokensIterator);
            break;
        }
        default: {
            pt->child.emplace_back(std::make_shared<TerminalNode>(*tokensIterator++));
            break;
        }
        }
    }

    pt->child.emplace_back(std::make_shared<Node>(Node(NodeKind::END_OF_PROGRAM)));
}

void Parser::makePTREC(const std::shared_ptr<Node>& t_parent, std::vector<Token>::const_iterator& t_tokensIterator)
{
    uint8_t totalElements { 1 };

    t_parent->child.emplace_back(std::make_shared<TerminalNode>(*t_tokensIterator++));

    while (t_tokensIterator->kind == TokenKind::UNDEFINED && t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        t_parent->child.emplace_back(std::make_shared<TerminalNode>(*t_tokensIterator++));
    }

    if (t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        t_parent->isError = true;
        t_parent->child.at(0)->isError = true;
        t_parent->child.at(0)->message = "There is no declaration for REC statement!";

        return;
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::LEFT_BRACE: {
            ++totalElements;
            break;
        }
        case TokenKind::COMMA: {
            ++totalElements;

            if (t_parent->child.size() > 1) {
                for (auto i = t_parent->child.rbegin(); i != t_parent->child.rend(); ++i) {
                    auto node = std::static_pointer_cast<TerminalNode>((*i));

                    if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                        break;
                    }

                    if (node->kind == TokenKind::COMMA) {
                        t_parent->isError = true;
                        node->isError = true;
                        node->message = "Value is needed!";
                        break;
                    }
                }
            }
            break;
        }
        case TokenKind::NUMBER: {
            ++totalElements;

            if (t_parent->child.size() > 1) {
                for (auto i = t_parent->child.rbegin(); i != t_parent->child.rend(); ++i) {
                    auto node = std::static_pointer_cast<TerminalNode>((*i));

                    if (node->kind == TokenKind::COMMA) {
                        break;
                    }

                    if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                        t_parent->isError = true;
                        node->isError = true;
                        node->message = "Separator is needed!";
                        break;
                    }
                }
            }
            break;
        }
        case TokenKind::STRING: {
            ++totalElements;

            if (t_parent->child.size() > 1) {
                for (auto i = t_parent->child.rbegin(); i != t_parent->child.rend(); ++i) {
                    auto node = std::static_pointer_cast<TerminalNode>((*i));

                    if (node->kind == TokenKind::COMMA) {
                        break;
                    }

                    if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                        t_parent->isError = true;
                        node->isError = true;
                        node->message = "Separator is needed!";
                        break;
                    }
                }
            }
            break;
        }
        case TokenKind::RIGHT_BRACE: {
            ++totalElements;

            if (!t_parent->isError && totalElements != 12) {
                t_parent->isError = true;
                t_parent->child.at(0)->isError = true;
                t_parent->child.at(0)->message = "Wrong statement declaration!";
            }

            t_parent->child.emplace_back(std::make_shared<TerminalNode>(*t_tokensIterator++));
            return;
        }
        default:
            break;
        }

        if (t_tokensIterator->kind == TokenKind::REC) {
            break;
        }

        t_parent->child.emplace_back(std::make_shared<TerminalNode>(*t_tokensIterator++));
    }

    if (!t_parent->isError) {
        t_parent->isError = true;
        t_parent->child.at(0)->isError = true;
        t_parent->child.at(0)->message = "Statement is not closed!";
    }
}

void Parser::addRECNode(const int16_t& t_left, const int16_t& t_top, const int16_t& t_width, const int16_t& t_height, const Rect::Material& t_material)
{
    auto node = std::make_shared<StatementNode>(StatementKind::REC_CALL);

    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::REC, "REC")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::LEFT_BRACE, "(")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::NUMBER, std::to_string(t_left))));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::COMMA, ",")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::NUMBER, std::to_string(t_top))));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::COMMA, ",")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::NUMBER, std::to_string(t_width))));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::COMMA, ",")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::NUMBER, std::to_string(t_height))));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::COMMA, ",")));

    switch (t_material) {
    case Rect::Material::NW:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "NW")));
        break;
    case Rect::Material::DN:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "DN")));
        break;
    case Rect::Material::DP:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "DP")));
        break;
    case Rect::Material::PO:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "PO")));
        break;
    case Rect::Material::PO2:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "PO2")));
        break;
    case Rect::Material::ME:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "ME")));
        break;
    case Rect::Material::CO:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "CO")));
        break;
    case Rect::Material::M2:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "M2")));
        break;
    case Rect::Material::VI:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "VI")));
        break;
    case Rect::Material::M3:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "M3")));
        break;
    case Rect::Material::V2:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "V2")));
        break;
    case Rect::Material::M4:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "M4")));
        break;
    case Rect::Material::V3:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "V3")));
        break;
    case Rect::Material::M5:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "M5")));
        break;
    case Rect::Material::V4:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "V4")));
        break;
    case Rect::Material::M6:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "M6")));
        break;
    case Rect::Material::V5:
        node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::STRING, "V5")));
        break;
    default:
        break;
    }

    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::RIGHT_BRACE, ")")));
    node->child.emplace_back(std::make_shared<TerminalNode>(Token(TokenKind::UNDEFINED, "\n")));

    pt->child.emplace_back(node);

    makeASTREC(node);
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

                makeASTREC(node);
            }
        }
    }
}

void Parser::makeASTREC(const std::shared_ptr<Node>& t_parent)
{
    std::vector<int16_t> numericArgs {};
    std::string literalArg {};

    for (auto& child : t_parent->child) {
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
            material = Rect::Material::M4;
        } else if (literalArg == "V3") {
            material = Rect::Material::V3;
        } else if (literalArg == "M5") {
            material = Rect::Material::M5;
        } else if (literalArg == "V4") {
            material = Rect::Material::V4;
        } else if (literalArg == "M6") {
            material = Rect::Material::M6;
        } else if (literalArg == "V5") {
            material = Rect::Material::V5;
        }

        auto rect = std::make_shared<Rect>(numericArgs[0], numericArgs[1], numericArgs[2], numericArgs[3], material);
        rect->source = t_parent;

        ast.insert(rect);
    }
}