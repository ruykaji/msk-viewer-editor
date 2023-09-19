#include <QApplication>
#include <QTransform>

#include "DrawData.hpp"
#include "ViewerWidget.hpp"

constexpr double __BORDERS__ = 0.8;
constexpr double __LEFT_SIDE_SHIFT__ = 0.85;
constexpr double __BOTTOM_SIDE_SHIFT__ = 1.25;
constexpr double __MOVE_FACTOR__ = 0.85;

ViewerWidget::ViewerWidget(Parser* t_parser, QWidget* t_parent)
    : m_parser(t_parser)
    , QWidget(t_parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(16, 16, 16));
    setAutoFillBackground(true);
    setPalette(pal);
};

void ViewerWidget::drawGrid(QPainter* t_painter)
{
    auto widthCount = width() / 1;
    auto heightCount = height() / 1;

    for (std::size_t i = 0; i < widthCount; ++i) {
    }
}

void ViewerWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate(m_moveAxesIn * m_currentScale);
    painter->scale(m_currentScale, m_currentScale);

    QTransform rotate(1, 0, 0, -1, 0, 0);

    for (auto& rect : m_parser->ast) {
        selectPenAndBrush(rect->material, 1.0 / m_currentScale, painter);

        QRect qRect(rect->left, rect->top, rect->width, rect->height);
        qRect = rotate.mapRect(qRect);

        painter->drawRect(qRect);
    }

    if (m_isDrawing) {
        selectPenAndBrush(m_drawingMaterial, 1.0 / m_currentScale, painter);
        painter->drawRect(QRect(m_mouseTriggerPos / m_currentScale - m_moveAxesIn, m_mouseCurrentPos / m_currentScale - m_moveAxesIn));
    }

    painter->end();
};

void ViewerWidget::resizeEvent(QResizeEvent* t_event)
{
    Q_UNUSED(t_event);

    if (m_parser->ast.size() != 0) {
        double newInitial = std::min((width() * __BORDERS__) / (m_max.first - m_min.first), (height() * __BORDERS__) / (m_max.second - m_min.second));

        m_currentScale = m_currentScale / m_initScale * newInitial;
        m_initScale = newInitial;
    }
}

void ViewerWidget::mousePressEvent(QMouseEvent* t_event)
{
    if (t_event->button() == Qt::LeftButton) {
        m_mouseTriggerPos = t_event->pos();

        if (!m_isDrawing) {
            QCursor changedCursor = cursor();
            changedCursor.setShape(Qt::ClosedHandCursor);
            setCursor(changedCursor);
        }
    }
}

void ViewerWidget::mouseMoveEvent(QMouseEvent* t_event)
{
    if (t_event->buttons() & Qt::LeftButton) {

        if (!m_isDrawing) {
            m_moveAxesIn = m_axesPos - (m_mouseTriggerPos - t_event->pos()) / m_currentScale;
        } else {
            m_mouseCurrentPos = t_event->pos();
        }

        update();
    }
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_axesPos = m_moveAxesIn;

    if (!m_isDrawing) {
        QCursor changedCursor = cursor();
        changedCursor.setShape(Qt::ArrowCursor);
        setCursor(changedCursor);
    } else {
        QTransform rotate(1, 0, 0, -1, 0, 0);

        auto rect = rotate.mapRect(QRect(m_mouseTriggerPos / m_currentScale - m_moveAxesIn, m_mouseCurrentPos / m_currentScale - m_moveAxesIn));

        m_parser->addRECNode(rect.left(), rect.top(), rect.width(), rect.height(), m_drawingMaterial);
        m_drawingMaterial = Rect::Material::None;
        m_isDrawing = false;

        newRect();
    }
}

void ViewerWidget::wheelEvent(QWheelEvent* t_event)
{
    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
        double sigmoid = 1.0 / (1 + std::pow(2.718281282846, -1 * m_scroll));

        if ((sigmoid > 0.5 || t_event->angleDelta().y() > 0) && (sigmoid < 0.995 || t_event->angleDelta().y() < 0)) {
            m_scroll += 0.1 * (t_event->angleDelta().y() > 0 ? 1 : -1);
            auto newCurrentScale = m_initScale / (1.0 / (std::pow(2.718281282846, 1 * m_scroll)));
            auto scaleDiff = newCurrentScale / m_currentScale;
            m_currentScale = newCurrentScale;

            auto mousePos = t_event->position();
            auto diff = (mousePos * scaleDiff - mousePos);
            m_moveAxesIn = m_axesPos - QPoint(diff.x(), diff.y()) / m_currentScale;
            m_axesPos = m_moveAxesIn;
        }

        update();
    }
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

    QTransform rotate(1, 0, 0, -1, 0, 0);
    auto borders = rotate.mapRect(QRect(m_min.first, m_min.second, m_max.first - m_min.first, m_max.second - m_min.second));
    m_axesPos = QPoint(-borders.left() * __LEFT_SIDE_SHIFT__, height() / m_currentScale - borders.bottom() * __BOTTOM_SIDE_SHIFT__);
    m_moveAxesIn = m_axesPos;

    update();
}

void ViewerWidget::selectDrawingMaterial(Rect::Material& t_material)
{
    m_isDrawing = true;
    m_drawingMaterial = t_material;
}