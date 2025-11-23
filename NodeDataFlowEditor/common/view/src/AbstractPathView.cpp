/*
    MIT License

    Copyright (c) 2025 Joseph Al Hajjar

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "common/view/AbstractPathView.hpp"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStyleOption>
#include <QtMath>
#include <qcoreevent.h>
namespace nodeeditor::common::view
{
    AbstractPathView::AbstractPathView(QGraphicsItem* parent)
        : QGraphicsPathItem(parent)
    {
        setFlag(ItemIsSelectable, true);
        setFlag(ItemIsMovable, false);
        QPen pen(Qt::red, 2);
        setPen(pen);
        setZValue(1);
    }

    AbstractPathView::~AbstractPathView() = default;

    QPainterPath
    AbstractPathView::shape() const
    {
        QPainterPathStroker stroker;
        stroker.setWidth(10); // thickness for selection hit area
        return stroker.createStroke(m_currentPath);
    }

    void AbstractPathView::set_pos(const utility::SPos& p)
    {
        if (pos_.x == p.x && pos_.y == p.y)
            return;
        pos_ = p;
        setPos(p.x, p.y);
        pos_changed.notify(pos_);
        if (onPosChanged)
            onPosChanged(pos_);
        update();
    }

    void AbstractPathView::set_rect(const utility::SRect& r)
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

    QRectF AbstractPathView::toQRectF(const utility::SRect& r)
    {
        return QRectF(r.x, r.y, r.width, r.height);
    }

    void AbstractPathView::set_visible(bool v)
    {
        if (visible_ == v)
            return;
        visible_ = v;
        setVisible(v);
        visible_changed.notify(visible_);
        if (onVisibleChanged)
            onVisibleChanged(visible_);
    }

    void AbstractPathView::set_select(bool s)
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

    void AbstractPathView::set_pressed(bool s)
    {
        if (pressed_ == s)
            return;
        pressed_ = s;
        pressed_changed.notify(pressed_);
        if (onPressedChanged)
            onPressedChanged(pressed_);
    }

    void
    AbstractPathView::paint(QPainter* /*painter*/,
                            const QStyleOptionGraphicsItem* /*option*/,
                            QWidget*)
    {
    }

    bool
    AbstractPathView::sceneEvent(QEvent* event)
    {
        switch (event->type())
        {
            case QEvent::GraphicsSceneMousePress:
                set_pressed(true);
                update();
                return true;

            case QEvent::GraphicsSceneMouseRelease:
                set_pressed(false);
                update();
                return true;

            default:
                break;
        }

        return QGraphicsItem::sceneEvent(event);
    }

    QVariant
    AbstractPathView::itemChange(GraphicsItemChange change, const QVariant& value)
    {
        if (change == ItemPositionHasChanged)
        {
            pos_.x = value.toPointF().x();
            pos_.y = value.toPointF().y();
            set_pos(pos_);
        }
        else if (change == ItemSelectedHasChanged)
        {
            set_select(value.toBool());
        }
        else if (change == ItemVisibleHasChanged)
        {
            set_visible(value.toBool());
        }

        return QGraphicsItem::itemChange(change, value);
    }

    // -------------------- Callbacks Setters --------------------

    void AbstractPathView::setOnPosChanged(std::function<void(const utility::SPos&)> cb) { onPosChanged = std::move(cb); }
    void AbstractPathView::setOnRectChanged(std::function<void(const utility::SRect&)> cb) { onRectChanged = std::move(cb); }
    void AbstractPathView::setOnVisibleChanged(std::function<void(const bool&)> cb) { onVisibleChanged = std::move(cb); }
    void AbstractPathView::setOnSelectChanged(std::function<void(const bool&)> cb) { onSelectChanged = std::move(cb); }
    void AbstractPathView::setOnPressedChanged(std::function<void(const bool&)> cb) { onPressedChanged = std::move(cb); }

} // namespace nodeeditor::common::view
