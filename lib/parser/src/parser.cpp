#include "parser.hpp"

std::shared_ptr<Node> Parser::makeTree(const std::vector<Token>& t_tokens)
{
    auto treeIterator = std::make_shared<Node>(Node(NodeKind::PROGRAM));
    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        makeStep(treeIterator, tokensIterator);
    }

    treeIterator->child.emplace_back(std::make_shared<TerminalNode>(TerminalNode(*tokensIterator++)));

    return treeIterator;
}

void Parser::makeStep(std::shared_ptr<Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator)
{
    switch (t_tokensIterator->kind) {
    case TokenKind::REC: {
        auto node = std::make_shared<StatementNode>(StatementNode(StatementKind::REC_CALL));
        node->parent = t_treeIterator;

        t_treeIterator->child.emplace_back(node);
        t_treeIterator = node;

        t_treeIterator->child.emplace_back(std::make_shared<TerminalNode>(TerminalNode(*t_tokensIterator++)));

        makeREC(t_treeIterator, t_tokensIterator);

        t_treeIterator = t_treeIterator->parent;
        break;
    }
    default: {
        t_treeIterator->child.emplace_back(std::make_shared<TerminalNode>(TerminalNode(*t_tokensIterator++)));
        break;
    }
    }
}

void Parser::makeREC(std::shared_ptr<Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator)
{
    if (!t_treeIterator->isError && t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        t_treeIterator->isError = true;
        t_treeIterator->message = "There is no declaration for REC statement!";
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::NUMBER: {
            if (t_treeIterator->child.size() > 0 && t_treeIterator->child.back()->kind == NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<TerminalNode>(t_treeIterator->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }
            break;
        }
        case TokenKind::STRING: {
            if (t_treeIterator->child.size() > 0 && t_treeIterator->child.back()->kind == NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<TerminalNode>(t_treeIterator->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }

            break;
        }
        case TokenKind::RIGHT_BRACE: {
            makeStep(t_treeIterator, t_tokensIterator);
            return;
        }
        default:
            break;
        }

        makeStep(t_treeIterator, t_tokensIterator);
    }

    if (!t_treeIterator->isError) {
        t_treeIterator->isError = true;
        t_treeIterator->message = "Statement is not closed!";
    }
}