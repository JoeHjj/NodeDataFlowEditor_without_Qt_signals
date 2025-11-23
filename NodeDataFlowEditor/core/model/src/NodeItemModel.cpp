#include "core/model/NodeItemModel.hpp"

namespace nodeeditor::core::model
{

    void NodeItemModel::set_text(const std::string& t)
    {
        if (text_ == t)
            return;
        text_ = t;
        text_changed.notify(text_);
    }

    const std::string& NodeItemModel::text() const
    {
        return text_;
    }

} // namespace nodeeditor::core::model
