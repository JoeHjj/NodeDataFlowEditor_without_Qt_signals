#pragma once

#include "mvp/presenter/Presenter.hpp"

namespace nodeeditor::common::view
{
    class AbstractItemView;
}

namespace nodeeditor::common::model
{
    class AbstractItemModel;
}

namespace nodeeditor::common::presenter
{

    /**
     * @class AbstractItemPresenter
     * @brief Connects an AbstractItemModel with an AbstractItemView.
     *
     * This class acts as the Presenter in the Model-View-Presenter (MVP) architecture,
     * handling synchronization between the item model and its visual representation.
     *
     * It provides a base implementation that can be extended for specific graphical items.
     */
    class AbstractItemPresenter : public base::mvp::presenter::Presenter
    {
    public:
        /**
         * @brief Constructs an AbstractItemPresenter.
         *
         * Initializes the presenter by binding the given model and view together.
         *
         * @param model Shared pointer to the AbstractItemModel instance.
         * @param view Shared pointer to the AbstractItemView instance.
         */
        AbstractItemPresenter(std::shared_ptr<model::AbstractItemModel> model,
                              std::shared_ptr<view::AbstractItemView> view);

        /**
         * @brief Virtual destructor.
         *
         * Ensures proper cleanup in derived classes.
         */
        ~AbstractItemPresenter() override;
    };

} // namespace nodeeditor::common::presenter
