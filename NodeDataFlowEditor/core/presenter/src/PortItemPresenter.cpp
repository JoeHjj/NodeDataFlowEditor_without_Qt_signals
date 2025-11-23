#include "core/presenter/PortItemPresenter.hpp"

#include "core/model/PortItemModel.hpp"
#include "core/view/PortItemView.hpp"
#include <memory>

namespace nodeeditor::core::presenter
{
    PortItemPresenter::PortItemPresenter(std::shared_ptr<model::PortItemModel> model,
                                         std::shared_ptr<view::PortItemView> view)
        : common::presenter::AbstractItemPresenter(model, view)
    {
        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Connect Model -> View ----------------
        m->name_changed.connect([v](const std::string& t) { v->set_name(t); });
        m->module_name_changed.connect([v](const std::string& t) { v->set_module_name(t); });
        m->display_name_changed.connect([v](const std::string& t) { v->set_display_name(t); });
        m->orientation_changed.connect([v](const common::utility::SPort::Orientation& t) { v->set_orientation(t); });

        // ---------------- Connect View -> Model ----------------
        v->name_changed.connect([m](const std::string& t) { m->set_name(t); });
        v->module_name_changed.connect([m](const std::string& t) { m->set_module_name(t); });
        v->display_name_changed.connect([m](const std::string& t) { m->set_display_name(t); });
        v->orientation_changed.connect([m](const common::utility::SPort::Orientation& t) { m->set_orientation(t); });
    }

    PortItemPresenter::~PortItemPresenter() = default;
} // namespace nodeeditor::core::presenter
