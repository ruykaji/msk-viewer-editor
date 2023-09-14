#include "parser.hpp"

/*Parse tree methods*/
/*==========================================================================*/

std::shared_ptr<pt::Node> Parser::makePT(const std::vector<Token>& t_tokens)
{
    pt = std::make_shared<pt::Node>(pt::NodeKind::PROGRAM, nullptr);

    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        makePTStep(tokensIterator);
    }

    pt->child.emplace_back(std::make_shared<pt::Node>(pt::Node(pt::NodeKind::END_OF_PROGRAM, pt)));

    return pt;
}

void Parser::makePTStep(std::vector<Token>::const_iterator& t_tokensIterator)
{
    switch (t_tokensIterator->kind) {
    case TokenKind::REC: {
        auto node = std::make_shared<pt::StatementNode>(pt::StatementNode(pt::StatementKind::REC_CALL, pt));
        node->parent = pt;

        pt->child.emplace_back(node);
        pt = node;

        pt->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++, pt)));

        makePTREC(t_tokensIterator);

        pt = pt->parent;
        break;
    }
    default: {
        pt->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++, pt)));
        break;
    }
    }
}

void Parser::makePTREC(std::vector<Token>::const_iterator& t_tokensIterator)
{
    while (t_tokensIterator->kind == TokenKind::UNDEFINED) {
        makePTStep(t_tokensIterator);
    }

    if (!pt->isError && t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        pt->isError = true;
        pt->message = "There is no declaration for REC statement!";
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::NUMBER: {
            if (pt->child.size() > 0 && pt->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(pt->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }
            break;
        }
        case TokenKind::STRING: {
            if (pt->child.size() > 0 && pt->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(pt->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
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

        makePTStep(t_tokensIterator);
    }

    if (!pt->isError) {
        pt->isError = true;
        pt->message = "Statement is not closed!";
    }
}

/*Abstract syntax tree methods*/
/*==========================================================================*/

std::shared_ptr<ast::Node> Parser::makeAST()
{
    ast = std::make_shared<ast::Node>(ast::NodeKind::PROGRAM, nullptr);

    if (pt->kind != pt::NodeKind::END_OF_PROGRAM) {
        for (auto& child : pt->child) {
            if (child->kind == pt::NodeKind::STATEMENT) {
                auto node = std::static_pointer_cast<pt::StatementNode>(child);

                switch (node->kind) {
                case pt::StatementKind::REC_CALL: {
                    ast->child.emplace_back(std::make_shared<ast::RECNode>(ast));
                    ast = ast->child.back();
                    pt = node;

                    makeASTREC();

                    ast = ast->parent;
                    pt = node->parent;
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    return ast;
}

void Parser::makeASTREC()
{
    for (auto& child : pt->child) {
        if (child->kind == pt::NodeKind::TERMINAL) {
            auto node = std::static_pointer_cast<pt::TerminalNode>(child);

            switch (node->kind) {
            case TokenKind::NUMBER: {
                ast->child.emplace_back(std::make_shared<ast::NumberNode>(std::stoi(node->literal), ast));
                break;
            }
            case TokenKind::STRING: {
                ast->child.emplace_back(std::make_shared<ast::StringNode>(node->literal, ast));
                break;
            }
            default:
                break;
            }
        }
    }
}