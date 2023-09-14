#include "parser.hpp"

Parser::Parser()
{
    m_pt = std::make_shared<pt::Node>(pt::NodeKind::PROGRAM, nullptr);
    m_ast = std::make_shared<ast::Node>(ast::NodeKind::PROGRAM, nullptr);
}

/*Parse tree methods*/
/*==========================================================================*/

std::shared_ptr<pt::Node> Parser::makePT(const std::vector<Token>& t_tokens)
{
    auto tokensIterator = t_tokens.begin();

    while (tokensIterator->kind != TokenKind::END_OF_FILE) {
        makePTStep(tokensIterator);
    }

    m_pt->child.emplace_back(std::make_shared<pt::Node>(pt::Node(pt::NodeKind::END_OF_PROGRAM, m_pt)));

    return m_pt;
}

void Parser::makePTStep(std::vector<Token>::const_iterator& t_tokensIterator)
{
    switch (t_tokensIterator->kind) {
    case TokenKind::REC: {
        auto node = std::make_shared<pt::StatementNode>(pt::StatementNode(pt::StatementKind::REC_CALL, m_pt));
        node->parent = m_pt;

        m_pt->child.emplace_back(node);
        m_pt = node;

        m_pt->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++, m_pt)));

        makePTREC(t_tokensIterator);

        m_pt = m_pt->parent;
        break;
    }
    default: {
        m_pt->child.emplace_back(std::make_shared<pt::TerminalNode>(pt::TerminalNode(*t_tokensIterator++, m_pt)));
        break;
    }
    }
}

void Parser::makePTREC(std::vector<Token>::const_iterator& t_tokensIterator)
{
    while (t_tokensIterator->kind == TokenKind::UNDEFINED) {
        makePTStep(t_tokensIterator);
    }

    if (!m_pt->isError && t_tokensIterator->kind != TokenKind::LEFT_BRACE) {
        m_pt->isError = true;
        m_pt->message = "There is no declaration for REC statement!";
    }

    while (t_tokensIterator->kind != TokenKind::END_OF_FILE) {
        switch (t_tokensIterator->kind) {
        case TokenKind::NUMBER: {
            if (m_pt->child.size() > 0 && m_pt->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(m_pt->child.back());

                if (node->kind == TokenKind::NUMBER || node->kind == TokenKind::STRING) {
                    node->isError = true;
                    node->message = "Separator is needed!";
                }
            }
            break;
        }
        case TokenKind::STRING: {
            if (m_pt->child.size() > 0 && m_pt->child.back()->kind == pt::NodeKind::TERMINAL) {
                auto node = std::static_pointer_cast<pt::TerminalNode>(m_pt->child.back());

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

    if (!m_pt->isError) {
        m_pt->isError = true;
        m_pt->message = "Statement is not closed!";
    }
}

/*Abstract syntax tree methods*/
/*==========================================================================*/

std::shared_ptr<ast::Node> Parser::makeAST()
{
    if (m_pt->kind != pt::NodeKind::END_OF_PROGRAM) {
        for (auto& child : m_pt->child) {
            if (child->kind == pt::NodeKind::STATEMENT) {
                auto node = std::static_pointer_cast<pt::StatementNode>(child);

                switch (node->kind) {
                case pt::StatementKind::REC_CALL: {
                    m_ast->child.emplace_back(std::make_shared<ast::RECNode>(m_ast));
                    m_ast = m_ast->child.back();

                    makeASTREC();

                    m_ast = m_ast->parent;
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    return m_ast;
}

void Parser::makeASTREC()
{
    for (auto& child : m_pt->child) {
        if (child->kind == pt::NodeKind::TERMINAL) {
            auto node = std::static_pointer_cast<pt::TerminalNode>(child);

            switch (node->kind) {
            case TokenKind::NUMBER: {
                m_ast->child.emplace_back(std::make_shared<ast::NumberNode>(std::stoi(node->literal), m_ast));
                break;
            }
            case TokenKind::STRING: {
                m_ast->child.emplace_back(std::make_shared<ast::StringNode>(node->literal, m_ast));
                break;
            }
            default:
                break;
            }
        }
    }
}