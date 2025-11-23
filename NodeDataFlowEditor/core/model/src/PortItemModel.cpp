#include "core/model/PortItemModel.hpp"

namespace nodeeditor::core::model
{

    void PortItemModel::set_name(const std::string& t)
    {
        if (name_ == t)
            return;
        name_ = t;
        name_changed.notify(name_);
    }

    const std::string& PortItemModel::name() const
    {
        return name_;
    }

    void PortItemModel::set_module_name(const std::string& t)
    {
        if (moduleName_ == t)
            return;
        moduleName_ = t;
        module_name_changed.notify(moduleName_);
    }

    const std::string& PortItemModel::module_name() const
    {
        return moduleName_;
    }

    void PortItemModel::set_display_name(const std::string& t)
    {
        if (displayName_ == t)
            return;
        displayName_ = t;
        display_name_changed.notify(displayName_);
    }

    const std::string& PortItemModel::display_name() const
    {
        return displayName_;
    }

    void PortItemModel::set_orientation(const common::utility::SPort::Orientation& t)
    {
        if (orientation_ == t)
            return;
        orientation_ = t;
        orientation_changed.notify(orientation_);
    }

    const common::utility::SPort::Orientation& PortItemModel::orientation() const
    {
        return orientation_;
    }

} // namespace nodeeditor::core::model
