#pragma once

#include "common/presenter/AbstractItemPresenter.hpp"

namespace nodeeditor::core::model
{
    class PortItemModel;
}

namespace nodeeditor::core::view
{
    class PortItemView;
}

namespace nodeeditor::core::presenter
{
    class PortItemPresenter : public common::presenter::AbstractItemPresenter
    {
    public:
        PortItemPresenter(std::shared_ptr<model::PortItemModel> model,
                          std::shared_ptr<view::PortItemView> view);
        ~PortItemPresenter() override;
    };

} // namespace nodeeditor::core::presenter
