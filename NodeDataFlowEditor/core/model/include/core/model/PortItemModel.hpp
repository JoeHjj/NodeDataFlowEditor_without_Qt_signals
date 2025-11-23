#pragma once

#include "common/model/AbstractItemModel.hpp"
#include "common/utility/ConnectionInfo.hpp"
#include "mvp/utility/Signal.hpp"
#include <string>

namespace nodeeditor::core::model
{

    struct PortItemModel : public common::model::AbstractItemModel
    {
        /** @brief Default constructor. */
        explicit PortItemModel() = default;

        /** @brief Virtual destructor. */
        ~PortItemModel() override = default;

        base::mvp::utility::Signal<const std::string&> name_changed;
        void set_name(const std::string& t);
        const std::string& name() const;

        base::mvp::utility::Signal<const std::string&> module_name_changed;
        void set_module_name(const std::string& t);
        const std::string& module_name() const;

        base::mvp::utility::Signal<const std::string&> display_name_changed;
        void set_display_name(const std::string& t);
        const std::string& display_name() const;

        base::mvp::utility::Signal<const common::utility::SPort::Orientation&> orientation_changed;
        void set_orientation(const common::utility::SPort::Orientation& t);
        const common::utility::SPort::Orientation& orientation() const;

    private:
        std::string name_;
        std::string moduleName_;
        std::string displayName_;
        common::utility::SPort::Orientation orientation_;
    };

} // namespace nodeeditor::core::model
