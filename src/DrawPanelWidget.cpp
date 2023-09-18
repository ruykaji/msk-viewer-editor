#include <QVBoxLayout>

#include "DrawPanelWidget.hpp"

OptionWidget::OptionWidget(const Rect::Material t_material, const QString& t_label, QWidget* t_parent)
    : QWidget(t_parent)
    , material(t_material)
    , m_label(t_label)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(25);
}

void OptionWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    auto painter = new QPainter();

    painter->begin(this);

    if (isSelected) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor(0, 0, 255, 50)));
        painter->drawRect(0, 0, 125, 25);
    }

    painter->setPen(QPen(QColor(Qt::black)));
    painter->setFont(QFont("Times", 9));
    painter->drawText(QRect(0, 0, 90, 25), Qt::AlignRight | Qt::AlignVCenter, m_label);

    selectPenAndBrush(material, 1.0, painter);

    painter->drawRect(QRect(100, 0, 24, 24));
    painter->end();
}

void OptionWidget::mousePressEvent(QMouseEvent* t_event)
{
    Q_UNUSED(t_event);

    isSelected = true;

    selected(material);
    update();
}

DrawPanelWidget::DrawPanelWidget(QWidget* t_parent)
    : QWidget(t_parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFixedWidth(125);

    m_options.emplace_back(new OptionWidget(Rect::Material::NW, "N-Well", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::DN, "N+ Diffusion", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::DP, "P+ Diffusion", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::PO, "Polysilicon", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::PO2, "Polysilicon-2", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::ME, "Metal 1", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::M2, "Metal 2", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::M3, "Metal 3", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::M4, "Metal 4", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::M5, "Metal 5", this));
    m_options.emplace_back(new OptionWidget(Rect::Material::M6, "Metal 6", this));

    auto vbox = new QVBoxLayout(this);

    vbox->addStretch();
    vbox->setSpacing(5);

    for (auto option : m_options) {
        vbox->addWidget(option);
        connect(option, &OptionWidget::selected, this, &DrawPanelWidget::selected);
        connect(option, &OptionWidget::selected, this, &DrawPanelWidget::unselect);
    }

    vbox->setContentsMargins(0, 0, 0, 0);

    setLayout(vbox);
}

void DrawPanelWidget::unselect(Rect::Material& t_material)
{
    for (auto option : m_options) {
        if (option->material != t_material) {
            option->isSelected = false;
        }
    }

    update();
}