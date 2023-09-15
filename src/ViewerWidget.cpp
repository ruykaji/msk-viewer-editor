#include <QPainter>

#include "ViewerWidget.hpp"

#define __BORDERS__ 0.8

ViewerWidget::ViewerWidget(Parser* t_parser, QWidget* t_parent)
    : m_parser(t_parser)
    , QWidget(t_parent)
{
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(16, 16, 16));
    setAutoFillBackground(true);
    setPalette(pal);
};

void ViewerWidget::updateData()
{
    m_min = { INT16_MAX, INT16_MAX };
    m_max = { 0, 0 };

    for (auto& child : m_parser->ast->child) {
        if (child->kind == ast::NodeKind::REC_NODE) {
            auto node = std::static_pointer_cast<ast::RECNode>(child);

            if (node->child.size() == 5) {
                std::vector<int16_t> value { 0, 0, 0, 0 };

                for (std::size_t i {}; i < 4; ++i) {
                    if (node->child.at(i)->kind == ast::NodeKind::NUMBER_NODE) {
                        auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(i));
                        value[i] = arg->value;
                    }
                }

                m_min.first = std::min(m_min.first, value[0]);
                m_min.second = std::min(m_min.second, value[1]);
                m_max.first = std::max(m_max.first, static_cast<int16_t>(value[0] + value[2]));
                m_max.second = std::max(m_max.second, static_cast<int16_t>(value[1] + value[3]));
            }
        }
    }

    double newInitial = std::min((width() * __BORDERS__) / (m_max.first - m_min.first), (height() * __BORDERS__) / (m_max.second - m_min.second));

    m_currentScale = m_currentScale / m_initScale * newInitial;
    m_initScale = newInitial;
}

void ViewerWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate((m_max.first - m_min.first) / 2.0, (m_max.second - m_min.second) / 2.0);
    painter->scale(m_currentScale, m_currentScale);

    painter->setPen(QPen(QColor(Qt::white), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));

    painter->drawLine(QLine(-width() / 2.0, 0, width() / 2.0, 0));
    painter->drawLine(QLine(0, -height() / 2.0, 0, height() / 2.0));

    for (auto& child : m_parser->ast->child) {
        if (child->kind == ast::NodeKind::REC_NODE) {
            std::vector<double> value { 0, 0, 0, 0 };
            auto node = std::static_pointer_cast<ast::RECNode>(child);

            if (node->child.size() == 5) {
                if (node->child.at(0)->kind == ast::NodeKind::NUMBER_NODE) {
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(0));
                    value[0] = arg->value;
                }

                if (node->child.at(1)->kind == ast::NodeKind::NUMBER_NODE) {
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(1));
                    value[1] = arg->value;
                }

                if (node->child.at(2)->kind == ast::NodeKind::NUMBER_NODE) {
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(2));
                    value[2] = arg->value;
                }

                if (node->child.at(3)->kind == ast::NodeKind::NUMBER_NODE) {
                    auto arg = std::static_pointer_cast<ast::NumberNode>(node->child.at(3));
                    value[3] = arg->value;
                }
            }

            painter->drawRect(QRect(value[0], value[1], value[2], value[3]));
        }
    }

    painter->end();
};

void ViewerWidget::redraw()
{
    updateData();
    update();
};