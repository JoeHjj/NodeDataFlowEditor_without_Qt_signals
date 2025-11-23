#pragma once

#include "common/model/AbstractPathModel.hpp"
#include "common/utility/ConnectionInfo.hpp"
#include "common/utility/GraphicsProperties.hpp"
#include "mvp/utility/Signal.hpp"

namespace nodeeditor::core::model
{

    /**
     * @class ConnectionPathModel
     * @brief Base model class representing common graphical item properties.
     *
     * This class provides property management for visual and interactive state
     * such as rotation, scale, position, geometry, enable/visibility flags,
     * selection, and interaction states like pressed, double-clicked, and moving.
     *
     * Each property emits a signal when its value changes.
     */
    struct ConnectionPathModel : public common::model::AbstractPathModel
    {
        /** @brief Default constructor. */
        explicit ConnectionPathModel() = default;

        /** @brief Virtual destructor. */
        ~ConnectionPathModel() override = default;

        base::mvp::utility::Signal<const bool&> compatible_changed;
        void set_compatible(bool b);
        bool compatible() const;

        // ===================== Active =====================
        /** @brief Signal emitted when active state changes. */
        base::mvp::utility::Signal<const bool&> active_changed;
        /** @brief Sets whether the item is active. */
        void set_active(bool b);
        /** @brief Returns the active state. */
        bool active() const;

        // ===================== input =====================
        base::mvp::utility::Signal<const common::utility::SPort&> input_changed;
        void set_input(common::utility::SPort b);
        common::utility::SPort input() const;

        // ===================== output =====================
        base::mvp::utility::Signal<const common::utility::SPort&> output_changed;
        void set_output(common::utility::SPort b);
        common::utility::SPort output() const;

        // ===================== end point =====================
        base::mvp::utility::Signal<const common::utility::SPoint&> endPoint_changed;
        void set_endPoint(common::utility::SPoint b);
        common::utility::SPoint endPoint() const;

    private:
        common::utility::SPoint endPoint_;
        common::utility::SPort inputPort_;
        common::utility::SPort outputPort_;

        bool active_{false};
        bool compatible_{false};
    };

} // namespace nodeeditor::core::model
