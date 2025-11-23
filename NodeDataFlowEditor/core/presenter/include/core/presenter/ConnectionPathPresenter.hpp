#pragma once

#include "common/presenter/AbstractPathPresenter.hpp"

namespace nodeeditor::core::model
{
    class ConnectionPathModel;
}
namespace nodeeditor::core::view
{
    class ConnectionPathView;
}
namespace nodeeditor::core::presenter
{

    /**
     * @class ConnectionPathPresenter
     * @brief Connects an ConnectionPathModel with an ConnectionPathView.
     *
     * This class acts as the Presenter in the Model-View-Presenter (MVP) architecture,
     * handling synchronization between the item model and its visual representation.
     *
     * It provides a base implementation that can be extended for specific graphical items.
     */
    class ConnectionPathPresenter : public common::presenter::AbstractPathPresenter
    {
    public:
        /**
         * @brief Constructs an ConnectionPathPresenter.
         *
         * Initializes the presenter by binding the given model and view together.
         *
         * @param model Shared pointer to the ConnectionPathModel instance.
         * @param view Shared pointer to the ConnectionPathView instance.
         */
        ConnectionPathPresenter(std::shared_ptr<model::ConnectionPathModel> model,
                                std::shared_ptr<view::ConnectionPathView> view);

        /**
         * @brief Virtual destructor.
         *
         * Ensures proper cleanup in derived classes.
         */
        ~ConnectionPathPresenter() override;
    };

} // namespace nodeeditor::core::presenter
