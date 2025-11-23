#pragma once

#include "common/presenter/AbstractItemPresenter.hpp"

namespace nodeeditor::core::model
{
    class EditableArrowItemModel;
}

namespace nodeeditor::core::view
{
    class EditableArrowItemView;
}

namespace nodeeditor::core::presenter
{
    class EditableArrowItemPresenter : public common::presenter::AbstractItemPresenter
    {
    public:
        EditableArrowItemPresenter(std::shared_ptr<model::EditableArrowItemModel> model,
                                   std::shared_ptr<view::EditableArrowItemView> view);
        ~EditableArrowItemPresenter() override;
    };

} // namespace nodeeditor::core::presenter
