#include <QPainter>

#include "ViewerWidget.hpp"

ViewerWidget::ViewerWidget(Parser* t_parser, QWidget* t_parent): m_parser(t_parser), QWidget(t_parent){
    m_parser = t_parser;

    std::pair<uint16_t, uint16_t> min {};
    std::pair<uint16_t, uint16_t> max {};

    for(auto& child : m_parser->ast->child){
        if(child->kind == ast::NodeKind::REC_NODE){
            auto node = std::static_pointer_cast<ast::RECNode>(child);

            if(node->child.size() == 5){
                if(node->child.at(0)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(0));
                    
                    if(arg->value > min.first){
                        min.first = arg->value;
                    }
                }

                if(node->child.at(1)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(1));
                    
                    if(arg->value > min.second){
                        min.second = arg->value;
                    }
                }

                if(node->child.at(2)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(2));
                    
                    if(arg->value < max.first){
                        max.first = arg->value;
                    }
                }

                if(node->child.at(3)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(3));
                    
                    if(arg->value < max.second){
                        max.second = arg->value;
                    }
                }
            }
        }
    }
}

void ViewerWidget::paintEvent(QPaintEvent* t_event){
    Q_UNUSED(t_event);

    QPainter painter;

    painter.begin(this);

    for(auto& child : m_parser->ast->child){
        if(child->kind == ast::NodeKind::REC_NODE){
            QRect rec;
            auto node = std::static_pointer_cast<ast::RECNode>(child);

            if(node->child.size() == 5){
                if(node->child.at(0)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(0));
                    rec.setLeft(arg->value);
                }

                if(node->child.at(1)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(1));
                    rec.setTop(arg->value);
                }

                if(node->child.at(2)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(2));
                    rec.setRight(rec.left() + arg->value);
                }

                if(node->child.at(3)->kind == ast::NodeKind::NUMBER_NODE){
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(3));
                    rec.setBottom(rec.top() + arg->value);
                }
            }

            painter.drawRect(rec);
        }

    }

    painter.end();
}

void ViewerWidget::redraw(){
    update();
}