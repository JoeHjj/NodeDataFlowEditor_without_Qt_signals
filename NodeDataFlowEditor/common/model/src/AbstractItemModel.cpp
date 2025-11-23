#include "common/model/AbstractItemModel.hpp"

namespace nodeeditor::common::model
{

    void AbstractItemModel::set_rotation(double r)
    {
        if (rotation_ == r)
            return;
        rotation_ = r;
        rotation_changed.notify(rotation_);
    }

    double AbstractItemModel::rotation() const
    {
        return rotation_;
    }

    void AbstractItemModel::set_scale(double s)
    {
        if (scale_ == s)
            return;
        scale_ = s;
        scale_changed.notify(scale_);
    }

    double AbstractItemModel::scale() const
    {
        return scale_;
    }

    void AbstractItemModel::set_pos(const utility::SPos& p)
    {
        if (pos_ == p)
            return;
        pos_ = p;
        pos_changed.notify(pos_);
    }

    const utility::SPos& AbstractItemModel::pos() const
    {
        return pos_;
    }

    void AbstractItemModel::set_rect(const utility::SRect& r)
    {
        if (rect_ == r)
            return;
        rect_ = r;
        rect_changed.notify(rect_);
    }

    const utility::SRect& AbstractItemModel::rect() const
    {
        return rect_;
    }

    void AbstractItemModel::set_enable(bool b)
    {
        if (enable_ == b)
            return;
        enable_ = b;
        enable_changed.notify(enable_);
    }

    bool AbstractItemModel::enable() const
    {
        return enable_;
    }

    void AbstractItemModel::set_visible(bool b)
    {
        if (visible_ == b)
            return;
        visible_ = b;
        visible_changed.notify(visible_);
    }

    bool AbstractItemModel::visible() const
    {
        return visible_;
    }

    void AbstractItemModel::set_active(bool b)
    {
        if (active_ == b)
            return;
        active_ = b;
        active_changed.notify(active_);
    }

    bool AbstractItemModel::active() const
    {
        return active_;
    }

    void AbstractItemModel::set_select(bool b)
    {
        if (select_ == b)
            return;
        select_ = b;
        select_changed.notify(select_);
    }

    bool AbstractItemModel::select() const
    {
        return select_;
    }

    void AbstractItemModel::set_pressed(bool b)
    {
        if (pressed_ == b)
            return;
        pressed_ = b;
        pressed_changed.notify(pressed_);
    }

    bool AbstractItemModel::pressed() const
    {
        return pressed_;
    }

    void AbstractItemModel::set_double_clicked(bool b)
    {
        if (double_clicked_ == b)
            return;
        double_clicked_ = b;
        double_clicked_changed.notify(double_clicked_);
    }

    bool AbstractItemModel::double_clicked() const
    {
        return double_clicked_;
    }

    void AbstractItemModel::set_moving(bool b)
    {
        if (moving_ == b)
            return;
        moving_ = b;
        moving_changed.notify(moving_);
    }

    bool AbstractItemModel::moving() const
    {
        return moving_;
    }

} // namespace nodeeditor::common::model
