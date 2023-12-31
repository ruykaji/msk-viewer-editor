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
        for (std::size_t i = 0; i < heightCount / m_currentScale + std::abs(m_axesPos.ry() - m_moveAxesIn.ry()) + 1.0; i += 1) {
            t_painter->drawLine(QLine(-m_axesPos.rx() - 1.0, i - m_moveAxesIn.ry(), widthCount / m_currentScale - m_moveAxesIn.rx() + 1.0, i - m_moveAxesIn.ry()));
        }
    } else if (m_axesPos.x() - m_moveAxesIn.x() < 0) {
        for (std::size_t i = 0; i < heightCount / m_currentScale + std::abs(m_axesPos.ry() - m_moveAxesIn.ry()) + 1.0; i += 1) {
            t_painter->drawLine(QLine(-m_moveAxesIn.rx() - 1.0, i - m_moveAxesIn.ry(), widthCount / m_currentScale - m_axesPos.rx() + 1.0, i - m_moveAxesIn.ry()));
        }
    }

    if (m_axesPos.y() - m_moveAxesIn.y() >= 0) {
        for (std::size_t i = 0; i < widthCount / m_currentScale + std::abs(m_axesPos.rx() - m_moveAxesIn.rx()) + 1.0; i += 1) {
            t_painter->drawLine(QLine(i - m_moveAxesIn.rx(), -m_axesPos.ry() - 1.0, i - m_moveAxesIn.rx(), heightCount / m_currentScale - m_moveAxesIn.ry() + 1.0));
        }
    } else if (m_axesPos.y() - m_moveAxesIn.y() < 0) {
        for (std::size_t i = 0; i < widthCount / m_currentScale + std::abs(m_axesPos.rx() - m_moveAxesIn.rx()) + 1.0; i += 1) {
            t_painter->drawLine(QLine(i - m_moveAxesIn.rx(), -m_moveAxesIn.ry() - 1.0, i - m_moveAxesIn.rx(), heightCount / m_currentScale - m_axesPos.ry() + 1.0));
        }
    }
}

void ViewerWidget::setMinMax()
{
    if (!m_parser->ast.empty()) {
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
}

void ViewerWidget::resetAxisPos()
{
    if (m_isScaleSet && !m_parser->ast.empty()) {
        QTransform rotate(1, 0, 0, -1, 0, 0);

        auto borders = rotate.mapRect(QRect(m_min.first, m_min.second, m_max.first - m_min.first, m_max.second - m_min.second));

        m_axesPos = QPoint(-borders.left(), height() / m_currentScale - borders.bottom());
    } else {
        m_axesPos = QPoint(0, height() / m_currentScale);
    }

    m_moveAxesIn = m_axesPos;
}

void ViewerWidget::drawCustomRect(QPainter* t_painter)
{
    selectPenAndBrush(m_drawingMaterial, 1.0 / m_currentScale, t_painter, 255, false);
    t_painter->setBrush(QBrush(QColor(Qt::transparent)));

    auto rect = QRectF(m_mouseTriggerPos / m_currentScale - m_moveAxesIn, m_mouseCurrentPos / m_currentScale - m_moveAxesIn).toRect();

    if (rect.width() != 0 && rect.height() != 0) {
        t_painter->drawRect(rect);

        t_painter->setPen(QPen(QColor(Qt::white), 1.0 / m_currentScale));

        auto textWidth = QString::number(abs(rect.width())) + "-LW";
        auto textHeight = QString::number(abs(rect.height())) + "-LH";

        if (rect.width() > 0 && rect.height() > 0) {
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(0, -1), QPointF(rect.right() + 1, rect.top() - 1)).toLine());
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(-1, 0), QPointF(rect.left() - 1, rect.bottom() + 1)).toLine());

            t_painter->drawText(((rect.topLeft() + QPointF(-textWidth.length(), -2)) + QPointF(rect.right() + 1, rect.top() - 1)) / 2.0, textWidth);
            t_painter->drawText(((rect.topLeft() + QPointF(-3 - textHeight.length(), 0)) + QPointF(rect.left() - 1, rect.bottom() + 1)) / 2.0, textHeight);
        } else if (rect.width() < 0 && rect.height() > 0) {
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(0, -1), QPointF(rect.right() + 1, rect.top() - 1)).toLine());
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(1, 0), QPointF(rect.left() + 1, rect.bottom() + 1)).toLine());

            t_painter->drawText(((rect.topLeft() + QPointF(-textWidth.length(), -2)) + QPointF(rect.right() + 1, rect.top() - 1)) / 2.0, textWidth);
            t_painter->drawText(((rect.topLeft() + QPointF(2, 0)) + QPointF(rect.left() + 1, rect.bottom() + 1)) / 2.0, textHeight);
        } else if (rect.width() > 0 && rect.height() < 0) {
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(0, 1), QPointF(rect.right() + 1, rect.top() + 1)).toLine());
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(-1, 0), QPointF(rect.left() - 1, rect.bottom() + 1)).toLine());

            t_painter->drawText(((rect.topLeft() + QPointF(-textWidth.length(), 4)) + QPointF(rect.right() + 1, rect.top() + 1)) / 2.0, textWidth);
            t_painter->drawText(((rect.topLeft() + QPointF(-3 - textHeight.length(), 0)) + QPointF(rect.left() - 1, rect.bottom() + 1)) / 2.0, textHeight);
        } else if (rect.width() < 0 && rect.height() < 0) {
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(0, 1), QPointF(rect.right() + 1, rect.top() + 1)).toLine());
            t_painter->drawLine(QLineF(rect.topLeft() + QPointF(1, 0), QPointF(rect.left() + 1, rect.bottom() + 1)).toLine());

            t_painter->drawText(((rect.topLeft() + QPointF(-textWidth.length(), 4)) + QPointF(rect.right() + 1, rect.top() + 1)) / 2.0, textWidth);
            t_painter->drawText(((rect.topLeft() + QPointF(2, 0)) + QPointF(rect.left() + 1, rect.bottom() + 1)) / 2.0, textHeight);
        }
    }
}

void ViewerWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate(m_moveAxesIn * m_currentScale);
    painter->scale(m_currentScale, m_currentScale);
    painter->setFont(QFont("Times", 1));

    drawGrid(painter);

    QTransform rotate(1, 0, 0, -1, 0, 0);

    bool isPenOpacity = m_mode == Mode::DRAWING;
    uint8_t opacity = isPenOpacity ? 25 : 145;

    for (auto& rect : m_parser->ast) {
        selectPenAndBrush(rect->material, 1.0 / m_currentScale, painter, opacity, isPenOpacity);

        QRect qRect(rect->left, rect->top, rect->width, rect->height);
        qRect = rotate.mapRect(qRect);

        painter->drawRect(qRect);
    }

    if (m_mode == Mode::DRAWING) {
        drawCustomRect(painter);
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

            newRect();
        }

        m_mode = Mode::DEFAULT;

        update();
        break;
    }

    default:
        break;
    }
}

void ViewerWidget::wheelEvent(QWheelEvent* t_event)
{
    if (m_mode == Mode::DEFAULT && QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
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
    m_isPositionSet = true;
    m_isScaleSet = true;

    setMinMax();
    resizeEvent(nullptr);
    resetAxisPos();
    update();
}

void ViewerWidget::selectDrawingMaterial(Rect::Material t_material)
{
    m_drawingMaterial = t_material;
}

void ViewerWidget::clear()
{
    m_isScaleSet = false;
    m_isPositionSet = false;
    m_min = { INT16_MAX, INT16_MAX };
    m_max = { 0, 0 };
    m_initScale = 10.0;
    m_currentScale = 10.0;
    m_prevCurrenScale = 10.0;
    m_scroll = 0.0;
    m_mode = Mode::DEFAULT;
    m_mouseTriggerPos = { 0, 0 };
    m_mouseCurrentPos = { 0, 0 };
    m_moveAxesIn = { 0, 0 };
    m_axesPos = { 0, 0 };
}