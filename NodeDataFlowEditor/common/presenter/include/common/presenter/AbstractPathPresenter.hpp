#pragma once

#include "mvp/presenter/Presenter.hpp"

namespace nodeeditor::common::view
{
    class AbstractPathView;
}

namespace nodeeditor::common::model
{
    class AbstractPathModel;
}

namespace nodeeditor::common::presenter
{

    /**
     * @class AbstractPathPresenter
     * @brief Connects an AbstractPathModel with an AbstractPathView.
     *
     * This class acts as the Presenter in the Model-View-Presenter (MVP) architecture,
     * handling synchronization between the item model and its visual representation.
     *
     * It provides a base implementation that can be extended for specific graphical items.
     */
    class AbstractPathPresenter : public base::mvp::presenter::Presenter
    {
    public:
        /**
         * @brief Constructs an AbstractPathPresenter.
         *
         * Initializes the presenter by binding the given model and view together.
         *
         * @param model Shared pointer to the AbstractPathModel instance.
         * @param view Shared pointer to the AbstractPathView instance.
         */
        AbstractPathPresenter(std::shared_ptr<model::AbstractPathModel> model,
                              std::shared_ptr<view::AbstractPathView> view);

        /**
         * @brief Virtual destructor.
         *
         * Ensures proper cleanup in derived classes.
         */
        ~AbstractPathPresenter() override;
    };

} // namespace nodeeditor::common::presenter
