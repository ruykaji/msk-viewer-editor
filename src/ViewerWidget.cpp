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

void ViewerWidget::selectPenAndBrush(const Rect::Material t_material, QPainter* t_painter)
{
    switch (t_material) {
    case Rect::Material::NW: {
        t_painter->setPen(QPen(QColor(55, 146, 55), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(55, 146, 55), Qt::Dense4Pattern));
        break;
    }
    case Rect::Material::DN: {
        t_painter->setPen(QPen(QColor(0, 128, 0), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(0, 128, 0)));
        break;
    }
    case Rect::Material::DP: {
        t_painter->setPen(QPen(QColor(128, 128, 0), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(128, 128, 0)));
        break;
    }
    case Rect::Material::PO: {
        t_painter->setPen(QPen(QColor(255, 0, 0), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(255, 0, 0)));
        break;
    }
    case Rect::Material::PO2: {
        t_painter->setPen(QPen(QColor(255, 0, 0), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(255, 0, 0)));
        break;
    }
    case Rect::Material::ME: {
        t_painter->setPen(QPen(QColor(0, 0, 255), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255)));
        break;
    }
    case Rect::Material::CO: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    case Rect::Material::M2: {
        t_painter->setPen(QPen(QColor(0, 0, 129), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 129)));
        break;
    }
    case Rect::Material::VI: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    case Rect::Material::M3: {
        t_painter->setPen(QPen(QColor(0, 0, 255), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255), Qt::BDiagPattern));
        break;
    }
    case Rect::Material::V2: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    case Rect::Material::M4: {
        t_painter->setPen(QPen(QColor(52, 252, 52), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(52, 252, 52), Qt::FDiagPattern));
        break;
    }
    case Rect::Material::V3: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    case Rect::Material::M5: {
        t_painter->setPen(QPen(QColor(0, 0, 255), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255), Qt::HorPattern));
        break;
    }
    case Rect::Material::V4: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    case Rect::Material::M6: {
        t_painter->setPen(QPen(QColor(128, 128, 128), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(128, 128, 128), Qt::Dense3Pattern));
        break;
    }
    case Rect::Material::V5: {
        t_painter->setPen(QPen(QColor(247, 247, 247), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247)));
        break;
    }
    default: {
        t_painter->setPen(QPen(QColor(Qt::white), 1.0 / m_currentScale, Qt::SolidLine, Qt::RoundCap));
        t_painter->setBrush(QBrush(QColor(Qt::white)));
        break;
    }
    }
}

void ViewerWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate((m_max.first - m_min.first) / 2.0, (m_max.second - m_min.second) / 2.0);
    painter->scale(m_currentScale, m_currentScale);

    painter->drawLine(QLine(-width() / 2.0, 0, width() / 2.0, 0));
    painter->drawLine(QLine(0, -height() / 2.0, 0, height() / 2.0));

    for (auto& rect : m_parser->ast) {
        selectPenAndBrush(rect->material, painter);
        painter->drawRect(QRect(rect->left - m_min.first, rect->top - m_min.second, rect->width, rect->height));
    }

    painter->end();
};

void ViewerWidget::resizeEvent(QResizeEvent* t_event)
{
    Q_UNUSED(t_event);

    double newInitial = std::min((width() * __BORDERS__) / (m_max.first - m_min.first), (height() * __BORDERS__) / (m_max.second - m_min.second));

    m_currentScale = m_currentScale / m_initScale * newInitial;
    m_initScale = newInitial;
}

void ViewerWidget::setNewScaling()
{
    m_min = { INT16_MAX, INT16_MAX };
    m_max = { 0, 0 };
    m_initScale = 1.0;
    m_currentScale = 1.0;

    for (auto& rect : m_parser->ast) {
        m_min.first = std::min(m_min.first, rect->left);
        m_min.second = std::min(m_min.second, rect->top);
        m_max.first = std::max(m_max.first, static_cast<int16_t>(rect->left + rect->width));
        m_max.second = std::max(m_max.second, static_cast<int16_t>(rect->top + rect->height));
    }

    resizeEvent(nullptr);

    update();
}