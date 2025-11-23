#pragma once

#include "common/utility/GraphicsProperties.hpp"
#include "mvp/model/IModel.hpp"
#include "mvp/utility/Signal.hpp"

namespace nodeeditor::common::model
{

    /**
     * @class AbstractPathModel
     * @brief Base model class representing common graphical item properties.
     *
     * This class provides property management for visual and interactive state
     * such as rotation, scale, position, geometry, enable/visibility flags,
     * selection, and interaction states like pressed, double-clicked, and moving.
     *
     * Each property emits a signal when its value changes.
     */
    struct AbstractPathModel : public ::base::mvp::model::IModel
    {
        /** @brief Default constructor. */
        explicit AbstractPathModel() = default;

        /** @brief Virtual destructor. */
        ~AbstractPathModel() override = default;

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

        // ===================== Visibility =====================
        /** @brief Signal emitted when visibility changes. */
        base::mvp::utility::Signal<const bool&> visible_changed;
        /** @brief Sets whether the item is visible. */
        void set_visible(bool b);
        /** @brief Returns the visibility state. */
        bool visible() const;

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

    private:
        utility::SPos scenePos_;
        utility::SRect rect_;

        bool visible_{true};
        bool select_{false};
        bool pressed_{false};
    };

} // namespace nodeeditor::common::model
