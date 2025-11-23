#pragma once
#include "mvp/model/IModel.hpp"
#include "mvp/presenter/IPresenter.hpp"
#include "mvp/view/IViewItem.hpp"
#include <memory>

namespace base::mvp::presenter
{

    class Presenter : public IPresenter
    {
    public:
        Presenter(std::shared_ptr<model::IModel> model, std::shared_ptr<view::IViewItem> view)
            : m_model(model)
            , m_view(view)
        {
        }

        std::shared_ptr<model::IModel> model()
        {
            return m_model;
        }

        std::shared_ptr<view::IViewItem> view()
        {
            return m_view;
        }

        ~Presenter() override = default;

    protected:
        std::shared_ptr<model::IModel> m_model;
        std::shared_ptr<view::IViewItem> m_view;
    };

} // namespace base::mvp::presenter
