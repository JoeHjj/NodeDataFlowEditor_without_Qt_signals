#pragma once

#include "common/utility/GraphicsProperties.hpp"
#include "mvp/model/IModel.hpp"
#include "mvp/utility/Signal.hpp"

namespace nodeeditor::common::model
{

    /**
     * @class AbstractItemModel
     * @brief Base model class representing common graphical item properties.
     *
     * This class provides property management for visual and interactive state
     * such as rotation, scale, position, geometry, enable/visibility flags,
     * selection, and interaction states like pressed, double-clicked, and moving.
     *
     * Each property emits a signal when its value changes.
     */
    struct AbstractItemModel : public ::base::mvp::model::IModel
    {
        /** @brief Default constructor. */
        explicit AbstractItemModel() = default;

        /** @brief Virtual destructor. */
        ~AbstractItemModel() override = default;

        // ===================== Rotation =====================
        /** @brief Signal emitted when rotation changes. */
        base::mvp::utility::Signal<const double&> rotation_changed;
        /** @brief Sets the rotation value. */
        void set_rotation(double r);
        /** @brief Returns the current rotation. */
        double rotation() const;

        // ===================== Scale =====================
        /** @brief Signal emitted when scale changes. */
        base::mvp::utility::Signal<const double&> scale_changed;
        /** @brief Sets the scale factor. */
        void set_scale(double s);
        /** @brief Returns the current scale. */
        double scale() const;

        // ===================== Position =====================
        /** @brief Signal emitted when position changes. */
        base::mvp::utility::Signal<const utility::SPos&> pos_changed;
        /** @brief Sets the position. */
        void set_pos(const utility::SPos& p);
        /** @brief Returns the current position. */
        const utility::SPos& pos() const;

        // ===================== Rectangle =====================
        /** @brief Signal emitted when rectangle bounds change. */
        base::mvp::utility::Signal<const utility::SRect&> rect_changed;
        /** @brief Sets the rectangle bounds. */
        void set_rect(const utility::SRect& r);
        /** @brief Returns the current rectangle bounds. */
        const utility::SRect& rect() const;

        // ===================== Enable =====================
        /** @brief Signal emitted when enable state changes. */
        base::mvp::utility::Signal<const bool&> enable_changed;
        /** @brief Sets whether the item is enabled. */
        void set_enable(bool b);
        /** @brief Returns the enable state. */
        bool enable() const;

        // ===================== Visibility =====================
        /** @brief Signal emitted when visibility changes. */
        base::mvp::utility::Signal<const bool&> visible_changed;
        /** @brief Sets whether the item is visible. */
        void set_visible(bool b);
        /** @brief Returns the visibility state. */
        bool visible() const;

        // ===================== Active =====================
        /** @brief Signal emitted when active state changes. */
        base::mvp::utility::Signal<const bool&> active_changed;
        /** @brief Sets whether the item is active. */
        void set_active(bool b);
        /** @brief Returns the active state. */
        bool active() const;

        // ===================== Selection =====================
        /** @brief Signal emitted when selection state changes. */
        base::mvp::utility::Signal<const bool&> select_changed;
        /** @brief Sets whether the item is selected. */
        void set_select(bool b);
        /** @brief Returns the selection state. */
        bool select() const;

        // ===================== Pressed =====================
        /** @brief Signal emitted when pressed state changes. */
        base::mvp::utility::Signal<const bool&> pressed_changed;
        /** @brief Sets whether the item is pressed. */
        void set_pressed(bool b);
        /** @brief Returns the pressed state. */
        bool pressed() const;

        // ===================== Double-clicked =====================
        /** @brief Signal emitted when double-clicked state changes. */
        base::mvp::utility::Signal<const bool&> double_clicked_changed;
        /** @brief Sets whether the item is double-clicked. */
        void set_double_clicked(bool b);
        /** @brief Returns the double-clicked state. */
        bool double_clicked() const;

        // ===================== Moving =====================
        /** @brief Signal emitted when moving state changes. */
        base::mvp::utility::Signal<const bool&> moving_changed;
        /** @brief Sets whether the item is currently moving. */
        void set_moving(bool b);
        /** @brief Returns the moving state. */
        bool moving() const;

    private:
        double rotation_{0.0};
        double scale_{1.0};
        utility::SPos pos_;
        utility::SRect rect_;
        bool enable_{true};
        bool visible_{true};
        bool active_{false};
        bool select_{false};
        bool hovered_{false};
        bool pressed_{false};
        bool double_clicked_{false};
        bool moving_{false};
    };

} // namespace nodeeditor::common::model
