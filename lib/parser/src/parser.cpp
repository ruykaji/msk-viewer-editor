#include "parser.hpp"

/*Parse tree methods*/
/*==========================================================================*/

std::shared_ptr<pt::Node> Parser::makePT(const std::vector<Token>& t_tokens)
{
    auto treeIterator = std::make_shared<pt::Node>(pt::Node(pt::NodeKind::PROGRAM));
    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        makePTStep(treeIterator, tokensIterator);
    }

    treeIterator->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*tokensIterator++)));

    return treeIterator;
}

void Parser::makePTStep(std::shared_ptr<pt::Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator)
{
    switch (t_tokensIterator->kind) {
    case TokenKind::REC: {
        auto node = std::make_shared<pt::StatementNode>(pt::StatementNode(pt::StatementKind::REC_CALL));
        node->parent = t_treeIterator;

        t_treeIterator->child.emplace_back(node);
        t_treeIterator = node;

        t_treeIterator->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++)));

        makePTREC(t_treeIterator, t_tokensIterator);

        t_treeIterator = t_treeIterator->parent;
        break;
    }
    default: {
        t_treeIterator->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++)));
        break;
    }
    }
}

void Parser::makePTREC(std::shared_ptr<pt::Node>& t_treeIterator, std::vector<Token>::const_iterator& t_tokensIterator)
{
    while (t_tokensIterator->kind == TokenKind::UNDEFINED) {
        makePTStep(t_treeIterator, t_tokensIterator);
    }

    if (!t_treeIterator->isError && t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        t_treeIterator->isError = true;
        t_treeIterator->message = "There is no declaration for REC statement!";
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::NUMBER: {
            if (t_treeIterator->child.size() > 0 && t_treeIterator->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(t_treeIterator->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }
            break;
        }
        case TokenKind::STRING: {
            if (t_treeIterator->child.size() > 0 && t_treeIterator->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(t_treeIterator->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }

            break;
        }
        case TokenKind::RIGHT_BRACE: {
            makePTStep(t_treeIterator, t_tokensIterator);
            return;
        }
        default:
            break;
        }

        makePTStep(t_treeIterator, t_tokensIterator);
    }

    if (!t_treeIterator->isError) {
        t_treeIterator->isError = true;
        t_treeIterator->message = "Statement is not closed!";
    }
}

/*Abstract syntax tree methods*/
/*==========================================================================*/

std::shared_ptr<ast::Node> Parser::makeAST(const std::shared_ptr<pt::Node>& t_parseTree){
    auto treeIterator= std::make_shared<ast::Node>(ast::Node(ast::NodeKind::PROGRAM));

    
}