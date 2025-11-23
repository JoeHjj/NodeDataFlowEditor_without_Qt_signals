#pragma once

#include "common/model/AbstractItemModel.hpp"
#include "mvp/utility/Signal.hpp"
#include <string>

namespace nodeeditor::core::model
{

    /**
     * @class NodeItemModel
     * @brief Model for a port item, extending common item properties.
     *
     * Inherits generic properties/signals from
     * nodeeditor::common::model::AbstractItemModel and adds a dedicated
     * text property specific to the port item. The setter emits
     * @ref text_changed when the value actually changes.
     */
    struct NodeItemModel : public common::model::AbstractItemModel
    {
        /** @brief Default constructor. */
        explicit NodeItemModel() = default;

        /** @brief Virtual destructor. */
        ~NodeItemModel() override = default;

        /** @brief Emitted when the port text changes. */
        base::mvp::utility::Signal<const std::string&> text_changed; // <new text>
        // ===================== Port Management Signals, not stocked =====================
        // One way call from model to presenter/view
        base::mvp::utility::Signal<const std::string&, const std::string&> add_input;                         // <name, display name>
        base::mvp::utility::Signal<const std::string&, const std::string&> add_output;                        // <name, display name>
        base::mvp::utility::Signal<const std::string&, const std::string&, const std::string&> add_parameter; // <type, name, display name>

        base::mvp::utility::Signal<const std::string&> remove_parameter; // <name>
        base::mvp::utility::Signal<const std::string&> remove_input;     // <name>
        base::mvp::utility::Signal<const std::string&> remove_output;    // <name>
        /**
         * @brief Set the port text.
         * @param t New text value.
         *
         * If the new value differs from the current one, stores it and emits
         * @ref text_changed with the updated value.
         */
        void set_text(const std::string& t);

        /**
         * @brief Get the current port text.
         * @return Const reference to the stored text.
         */
        const std::string& text() const;

    private:
        std::string text_;
    };

} // namespace nodeeditor::core::model
