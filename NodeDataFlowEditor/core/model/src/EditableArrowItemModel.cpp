#include "core/model/EditableArrowItemModel.hpp"

namespace nodeeditor::core::model
{

    void EditableArrowItemModel::set_text(const std::string& t)
    {
        if (text_ == t)
            return;
        text_ = t;
        text_changed.notify(text_);
    }

    const std::string& EditableArrowItemModel::text() const
    {
        return text_;
    }

} // namespace nodeeditor::core::model
