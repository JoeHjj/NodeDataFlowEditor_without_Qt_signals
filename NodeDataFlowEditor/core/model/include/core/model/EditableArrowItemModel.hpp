#pragma once

#include "common/model/AbstractItemModel.hpp"
#include "mvp/utility/Signal.hpp"
#include <string>

namespace nodeeditor::core::model
{

    /**
     * @class EditableArrowItemModel
     * @brief Model for a port item, extending common item properties.
     *
     * Inherits generic properties/signals from
     * nodeeditor::common::model::AbstractItemModel and adds a dedicated
     * text property specific to the port item. The setter emits
     * @ref text_changed when the value actually changes.
     */
    struct EditableArrowItemModel : public common::model::AbstractItemModel
    {
        /** @brief Default constructor. */
        explicit EditableArrowItemModel() = default;

        /** @brief Virtual destructor. */
        ~EditableArrowItemModel() override = default;

        /** @brief Emitted when the port text changes. */
        base::mvp::utility::Signal<const std::string&> text_changed;

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
