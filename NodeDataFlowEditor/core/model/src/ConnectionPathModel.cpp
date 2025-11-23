#include "core/model/ConnectionPathModel.hpp"

namespace nodeeditor::core::model
{

    void ConnectionPathModel::set_compatible(bool b)
    {
        if (compatible_ == b)
            return;
        compatible_ = b;
        compatible_changed.notify(compatible_);
    }

    bool ConnectionPathModel::compatible() const
    {
        return compatible_;
    }

    void ConnectionPathModel::set_active(bool b)
    {
        if (active_ == b)
            return;
        active_ = b;
        active_changed.notify(active_);
    }

    bool ConnectionPathModel::active() const
    {
        return active_;
    }
    void ConnectionPathModel::set_input(common::utility::SPort b)
    {
        if (inputPort_ == b)
            return;
        inputPort_ = b;
        input_changed.notify(inputPort_);
    }

    common::utility::SPort ConnectionPathModel::input() const
    {
        return inputPort_;
    }

    void ConnectionPathModel::set_output(common::utility::SPort b)
    {
        if (outputPort_ == b)
            return;
        outputPort_ = b;
        output_changed.notify(outputPort_);
    }

    common::utility::SPort ConnectionPathModel::output() const
    {
        return outputPort_;
    }

    void ConnectionPathModel::set_endPoint(common::utility::SPoint b)
    {
        if (endPoint_ == b)
            return;
        endPoint_ = b;
        endPoint_changed.notify(endPoint_);
    }

    common::utility::SPoint ConnectionPathModel::endPoint() const
    {
        return endPoint_;
    }

} // namespace nodeeditor::core::model
