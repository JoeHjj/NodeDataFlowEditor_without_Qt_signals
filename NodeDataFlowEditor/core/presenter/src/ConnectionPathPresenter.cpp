#include "core/presenter/ConnectionPathPresenter.hpp"
#include "core/model/ConnectionPathModel.hpp"
#include "core/view/ConnectionPathView.hpp"

#include <memory>

namespace nodeeditor::core::presenter
{

    ConnectionPathPresenter::ConnectionPathPresenter(std::shared_ptr<model::ConnectionPathModel> model,
                                                     std::shared_ptr<view::ConnectionPathView> view)
        : common::presenter::AbstractPathPresenter(model, view)
    {
        std::weak_ptr<model::ConnectionPathModel> wm = model;
        std::weak_ptr<view::ConnectionPathView> wv = view;

        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Model → View ----------------
        m->active_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_active(b); });
        m->input_changed.connect([wv](common::utility::SPort b) { if (auto pv = wv.lock()) pv->set_inputPort(b); });
        m->output_changed.connect([wv](common::utility::SPort b) { if (auto pv = wv.lock()) pv->set_outputPort(b); });
        m->endPoint_changed.connect([wv](common::utility::SPoint b) { if (auto pv = wv.lock()) pv->set_endPoint(b); });
        m->compatible_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_compatible(b); });

        v->active_changed.connect([wm](bool b) { if (auto pv = wm.lock()) pv->set_active(b); });
        v->input_changed.connect([wm](const common::utility::SPort& b) { if (auto pv = wm.lock()) pv->set_input(b); });
        v->output_changed.connect([wm](const common::utility::SPort& b) { if (auto pv = wm.lock()) pv->set_output(b); });
        v->endPoint_changed.connect([wm](const common::utility::SPoint& b) { if (auto pv = wm.lock()) pv->set_endPoint(b); });
        v->compatible_changed.connect([wm](bool b) { if (auto pv = wm.lock()) pv->set_compatible(b); });
    }

    ConnectionPathPresenter::~ConnectionPathPresenter() = default;

} // namespace nodeeditor::core::presenter
