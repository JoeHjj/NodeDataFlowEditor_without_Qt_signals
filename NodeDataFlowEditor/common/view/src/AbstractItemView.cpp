#include "common/view/AbstractItemView.hpp"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

namespace nodeeditor::common::view
{

    AbstractItemView::AbstractItemView(QGraphicsItem* parent)
        : QGraphicsItem(parent)
    {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsFocusable, true);

        // Initialize a reasonable default rect if none provided by model yet
        if (rect_.width <= 0.0 || rect_.height <= 0.0)
        {
            m_rect = QRectF(0.0, 0.0, 40.0, 20.0);
        }
        else
        {
            m_rect = toQRectF(rect_);
        }
    }

    void AbstractItemView::setColor(const QColor& c)
    {
        if (color_ == c)
            return;
        color_ = c;
        if (onColorChanged)
            onColorChanged(color_);
        update();
    }

    void AbstractItemView::set_rotation(double r)
    {
        if (rotation_ == r)
            return;
        rotation_ = r;
        setRotation(r);
        rotation_changed.notify(rotation_);
        if (onRotationChanged)
            onRotationChanged(rotation_);
        update();
    }

    void AbstractItemView::set_scale(double s)
    {
        if (scale_ == s)
            return;
        scale_ = s;
        setScale(s);
        scale_changed.notify(scale_);
        if (onScaleChanged)
            onScaleChanged(scale_);
        update();
    }

    void AbstractItemView::set_pos(const utility::SPos& p)
    {
        if (pos_ == p)
            return;
        pos_ = p;
        setPos(p.x, p.y);
        pos_changed.notify(pos_);
        if (onPosChanged)
            onPosChanged(pos_);
        update();
    }

    void AbstractItemView::set_pos(const double& x, const double& y)
    {
        utility::SPos p(x, y);
        set_pos(p);
    }

    void AbstractItemView::set_rect(const utility::SRect& r)
    {
        if (rect_ == r)
        {
            return;
        }
        rect_ = r;
        prepareGeometryChange();
        m_rect = toQRectF(r);
        rect_changed.notify(rect_);
        if (onRectChanged)
            onRectChanged(rect_);
        update();
    }

    void AbstractItemView::set_enable(bool e)
    {
        if (enable_ == e)
            return;
        enable_ = e;
        setEnabled(e);
        enable_changed.notify(enable_);
        if (onEnableChanged)
            onEnableChanged(enable_);
        update();
    }

    void AbstractItemView::set_visible(bool v)
    {
        if (visible_ == v)
            return;
        visible_ = v;
        setVisible(v);
        visible_changed.notify(visible_);
        if (onVisibleChanged)
            onVisibleChanged(visible_);
    }

    void AbstractItemView::set_active(bool a)
    {
        if (active_ == a)
            return;
        active_ = a;
        active_changed.notify(active_);
        if (onActiveChanged)
            onActiveChanged(active_);
        update();
    }

    void AbstractItemView::set_select(bool s)
    {
        if (select_ == s)
            return;
        select_ = s;
        setSelected(s);
        select_changed.notify(select_);
        if (onSelectChanged)
            onSelectChanged(select_);
        update();
    }

    void AbstractItemView::set_hovered(bool s)
    {
        if (hovered_ == s)
            return;
        hovered_ = s;
        hovered_changed.notify(hovered_);

        if (onHoverChanged)
            onHoverChanged(hovered_);
    }

    void AbstractItemView::set_pressed(bool s)
    {
        if (pressed_ == s)
            return;
        pressed_ = s;
        pressed_changed.notify(pressed_);
        if (onPressedChanged)
            onPressedChanged(pressed_);
    }

    void AbstractItemView::set_double_clicked(bool s)
    {
        if (double_clicked_ == s)
            return;
        double_clicked_ = s;
        double_clicked_changed.notify(double_clicked_);
        if (onDoubleClickedChanged)
            onDoubleClickedChanged(double_clicked_);
    }

    void AbstractItemView::set_moving(bool s)
    {
        if (moving_ == s)
            return;
        moving_ = s;
        moving_changed.notify(moving_);
        if (onMovingChanged)
            onMovingChanged(moving_);
    }

    QRectF AbstractItemView::boundingRect() const
    {
        return m_rect;
    }

    void AbstractItemView::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
    {
        // do nothing by default
    }

    QRectF AbstractItemView::toQRectF(const utility::SRect& r)
    {
        return QRectF(r.x, r.y, r.width, r.height);
    }

    utility::SRect AbstractItemView::fromQRectF(const QRectF& r)
    {
        return utility::SRect(r.x(), r.y(), r.width(), r.height());
    }

    bool
    AbstractItemView::sceneEvent(QEvent* event)
    {
        set_double_clicked(false);

        switch (event->type())
        {
            case QEvent::GraphicsSceneHoverEnter:
                set_hovered(true);
                update();
                break;

            case QEvent::GraphicsSceneHoverLeave:
                set_hovered(false);
                update();
                break;

            case QEvent::GraphicsSceneMousePress:
                set_pressed(true);
                set_select(true);
                update();
                break;

            case QEvent::GraphicsSceneMouseRelease:
                set_pressed(false);
                update();
                break;

            case QEvent::GraphicsSceneMouseDoubleClick:
                set_double_clicked(true);
                break;

            default:
                break;
        }

        return QGraphicsItem::sceneEvent(event);
    }

    QVariant
    AbstractItemView::itemChange(GraphicsItemChange change, const QVariant& value)
    {
        if (change == ItemPositionHasChanged)
        {
            utility::SPos pos;
            pos.x = value.toPointF().x();
            pos.y = value.toPointF().y();
            set_pos(pos);
        }
        else if (change == ItemSelectedHasChanged)
        {
            set_select(value.toBool());
        }
        else if (change == ItemRotationHasChanged)
        {
            set_rotation(value.toDouble());
        }
        else if (change == ItemScaleHasChanged)
        {
            set_scale(value.toDouble());
        }
        else if (change == ItemEnabledHasChanged)
        {
            set_enable(value.toBool());
        }
        else if (change == ItemVisibleHasChanged)
        {
            set_visible(value.toBool());
        }

        return QGraphicsItem::itemChange(change, value);
    }

    // -------------------- Callbacks Setters --------------------

    void AbstractItemView::setOnColorChanged(std::function<void(const QColor&)> cb) { onColorChanged = std::move(cb); }
    void AbstractItemView::setOnRotationChanged(std::function<void(const double&)> cb) { onRotationChanged = std::move(cb); }
    void AbstractItemView::setOnScaleChanged(std::function<void(const double&)> cb) { onScaleChanged = std::move(cb); }
    void AbstractItemView::setOnPosChanged(std::function<void(const utility::SPos&)> cb) { onPosChanged = std::move(cb); }
    void AbstractItemView::setOnRectChanged(std::function<void(const utility::SRect&)> cb) { onRectChanged = std::move(cb); }
    void AbstractItemView::setOnEnableChanged(std::function<void(const bool&)> cb) { onEnableChanged = std::move(cb); }
    void AbstractItemView::setOnVisibleChanged(std::function<void(const bool&)> cb) { onVisibleChanged = std::move(cb); }
    void AbstractItemView::setOnActiveChanged(std::function<void(const bool&)> cb) { onActiveChanged = std::move(cb); }
    void AbstractItemView::setOnSelectChanged(std::function<void(const bool&)> cb) { onSelectChanged = std::move(cb); }
    void AbstractItemView::setOnHoverChanged(std::function<void(const bool&)> cb) { onHoverChanged = std::move(cb); }
    void AbstractItemView::setOnPressedChanged(std::function<void(const bool&)> cb) { onPressedChanged = std::move(cb); }
    void AbstractItemView::setOnDoubleClickedChanged(std::function<void(const bool&)> cb) { onDoubleClickedChanged = std::move(cb); }
    void AbstractItemView::setOnMovingChanged(std::function<void(const bool&)> cb) { onMovingChanged = std::move(cb); }

} // namespace nodeeditor::common::view
