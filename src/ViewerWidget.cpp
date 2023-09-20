#include <QApplication>
#include <QMenu>
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
    m_contextMenu = new DrawContextMenu(this);

    connect(m_contextMenu, &DrawContextMenu::selectDrawingMaterial, this, &ViewerWidget::selectDrawingMaterial);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(16, 16, 16));
    setAutoFillBackground(true);
    setPalette(pal);
};

void ViewerWidget::drawGrid(QPainter* t_painter)
{
    auto widthCount = width();
    auto heightCount = height();

    t_painter->setPen(QPen(QColor(25, 25, 25), 1.0 / m_currentScale));

    if (m_axesPos.x() - m_moveAxesIn.x() >= 0) {
        for (double i = 0; i < heightCount / m_currentScale + std::abs(m_axesPos.ry() - m_moveAxesIn.ry()); i += 1.0) {
            t_painter->drawLine(QLine(-m_axesPos.rx(), i - m_axesPos.ry(), widthCount / m_currentScale - m_moveAxesIn.rx() + 1, i - m_axesPos.ry()));
        }
    }

    if (m_axesPos.x() - m_moveAxesIn.x() < 0) {
         for (double i = 0; i < heightCount / m_currentScale + std::abs(m_axesPos.ry() - m_moveAxesIn.ry()); i += 1.0) {
            t_painter->drawLine(QLine(-m_moveAxesIn.rx(), i - m_moveAxesIn.ry(), widthCount / m_currentScale - m_axesPos.rx() + 1, i - m_moveAxesIn.ry()));
        }
    }

    if (m_axesPos.y() - m_moveAxesIn.y() >= 0) {
        for (double i = 0; i < widthCount / m_currentScale + std::abs(m_axesPos.rx() - m_moveAxesIn.rx()); i += 1.0) {
            t_painter->drawLine(QLine(i - m_axesPos.rx(), -m_axesPos.ry(), i - m_axesPos.rx(), heightCount / m_currentScale - m_moveAxesIn.ry() + 1));
        }
    }

    if (m_axesPos.y() - m_moveAxesIn.y() < 0) {
        for (double i = 0; i < widthCount / m_currentScale + std::abs(m_axesPos.rx() - m_moveAxesIn.rx()); i += 1.0) {
            t_painter->drawLine(QLine(i - m_moveAxesIn.rx(), -m_moveAxesIn.ry(), i - m_moveAxesIn.rx(), heightCount / m_currentScale - m_axesPos.ry() + 1));
        }
    }
}

void ViewerWidget::setMinMax()
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
}

void ViewerWidget::resetAxisPos()
{
    if (m_isScaleSet) {
        QTransform rotate(1, 0, 0, -1, 0, 0);

        auto borders = rotate.mapRect(QRect(m_min.first, m_min.second, m_max.first - m_min.first, m_max.second - m_min.second));

        m_axesPos = QPoint(-borders.left(), height() / m_currentScale - borders.bottom());
    } else {
        m_axesPos = QPoint(0, height() / m_currentScale);
    }

    m_moveAxesIn = m_axesPos;
}

void ViewerWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate(m_moveAxesIn * m_currentScale);
    painter->scale(m_currentScale, m_currentScale);

    drawGrid(painter);

    QTransform rotate(1, 0, 0, -1, 0, 0);

    for (auto& rect : m_parser->ast) {
        selectPenAndBrush(rect->material, 1.0 / m_currentScale, painter);

        QRect qRect(rect->left, rect->top, rect->width, rect->height);
        qRect = rotate.mapRect(qRect);

        painter->drawRect(qRect);
    }

    if (m_mode == Mode::DRAWING) {
        selectPenAndBrush(m_drawingMaterial, 1.0 / m_currentScale, painter);

        auto rect = QRectF(m_mouseTriggerPos / m_currentScale - m_moveAxesIn, m_mouseCurrentPos / m_currentScale - m_moveAxesIn).toRect();

        if (rect.width() > 0 && rect.height() > 0) {
            painter->drawRect(rect);
        }
    }

    painter->end();
};

void ViewerWidget::resizeEvent(QResizeEvent* t_event)
{
    Q_UNUSED(t_event);

    if (!m_isPositionSet) {
        resetAxisPos();
        m_isPositionSet = true;
    }

    if (m_parser->ast.size() != 0) {
        if (!m_isScaleSet) {
            setMinMax();
            m_isScaleSet = true;
        }

        double newInitial = std::min((width() * __BORDERS__) / (m_max.first - m_min.first), (height() * __BORDERS__) / (m_max.second - m_min.second));

        m_currentScale = m_currentScale / m_initScale * newInitial;
        m_initScale = newInitial;
    }
}

void ViewerWidget::mousePressEvent(QMouseEvent* t_event)
{
    switch (t_event->button()) {
    case Qt::MiddleButton: {
        m_mode = Mode::DRAGING;
        m_mouseTriggerPos = t_event->pos();
        setCursor(QCursor(Qt::ClosedHandCursor));
        break;
    }
    case Qt::LeftButton: {
        m_mode = Mode::DRAWING;
        m_mouseTriggerPos = t_event->pos();
        m_mouseCurrentPos = m_mouseTriggerPos;
        break;
    }
    case Qt::RightButton: {
        m_contextMenu->popup(mapToGlobal(t_event->pos()));
        break;
    }
    default:
        break;
    }

    update();
}

void ViewerWidget::mouseMoveEvent(QMouseEvent* t_event)
{
    switch (m_mode) {
    case Mode::DRAGING: {
        m_moveAxesIn = m_axesPos - (m_mouseTriggerPos - t_event->pos()) / m_currentScale;
        update();
        break;
    }
    case Mode::DRAWING: {
        m_mouseCurrentPos = t_event->pos();
        update();
        break;
    }
    default:
        break;
    }
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent* t_event)
{
    switch (m_mode) {
    case Mode::DRAGING: {
        m_mode = Mode::DEFAULT;
        m_axesPos = m_moveAxesIn;
        setCursor(QCursor(Qt::ArrowCursor));
        update();
        break;
    }
    case Mode::DRAWING: {
        QTransform rotate(1, 0, 0, -1, 0, 0);

        auto rect = rotate.mapRect(QRectF(m_mouseTriggerPos / m_currentScale - m_moveAxesIn, m_mouseCurrentPos / m_currentScale - m_moveAxesIn).toRect());

        if (rect.width() > 0 && rect.height() > 0) {
            m_parser->addRECNode(rect.left(), rect.top(), rect.width(), rect.height(), m_drawingMaterial);
            m_mode = Mode::DEFAULT;

            newRect();
        }

        update();
        break;
    }

    default:
        break;
    }
}

void ViewerWidget::wheelEvent(QWheelEvent* t_event)
{
    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
        m_scroll += 0.1 * (t_event->angleDelta().y() > 0 ? 1 : -1);

        m_prevCurrenScale = m_currentScale;
        m_currentScale = m_initScale / (1.0 / (std::pow(2.718281282846, 1 * m_scroll)));

        m_moveAxesIn = m_axesPos - (m_currentScale / m_prevCurrenScale - 1) * t_event->position() / m_currentScale;
        m_axesPos = m_moveAxesIn;

        update();
    }
}

void ViewerWidget::setNewScaling()
{
    setMinMax();
    resizeEvent(nullptr);
    resetAxisPos();
    update();
}

void ViewerWidget::selectDrawingMaterial(Rect::Material t_material)
{
    m_drawingMaterial = t_material;
}