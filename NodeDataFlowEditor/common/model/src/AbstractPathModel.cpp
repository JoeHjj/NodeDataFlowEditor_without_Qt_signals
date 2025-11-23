#include "common/model/AbstractPathModel.hpp"

namespace nodeeditor::common::model
{

    void AbstractPathModel::set_pos(const utility::SPos& p)
    {
        if (scenePos_ == p)
            return;
        scenePos_ = p;
        pos_changed.notify(scenePos_);
    }

    const utility::SPos& AbstractPathModel::pos() const
    {
        return scenePos_;
    }

    void AbstractPathModel::set_rect(const utility::SRect& r)
    {
        if (rect_ == r)
            return;
        rect_ = r;
        rect_changed.notify(rect_);
    }

    const utility::SRect& AbstractPathModel::rect() const
    {
        return rect_;
    }

    void AbstractPathModel::set_visible(bool b)
    {
        if (visible_ == b)
            return;
        visible_ = b;
        visible_changed.notify(visible_);
    }

    bool AbstractPathModel::visible() const
    {
        return visible_;
    }

    void AbstractPathModel::set_select(bool b)
    {
        if (select_ == b)
            return;
        select_ = b;
        select_changed.notify(select_);
    }

    bool AbstractPathModel::select() const
    {
        return select_;
    }

    void AbstractPathModel::set_pressed(bool b)
    {
        if (pressed_ == b)
            return;
        pressed_ = b;
        pressed_changed.notify(pressed_);
    }

    bool AbstractPathModel::pressed() const
    {
        return pressed_;
    }

} // namespace nodeeditor::common::model
