#include "common/presenter/AbstractPathPresenter.hpp"
#include "common/model/AbstractPathModel.hpp"
#include "common/view/AbstractPathView.hpp"
#include "mvp/presenter/Presenter.hpp"
#include <QDebug>
#include <memory>

namespace nodeeditor::common::presenter
{

    AbstractPathPresenter::AbstractPathPresenter(std::shared_ptr<model::AbstractPathModel> model,
                                                 std::shared_ptr<view::AbstractPathView> view)
        : base::mvp::presenter::Presenter(model, view)
    {
        std::weak_ptr<model::AbstractPathModel> wm = model;
        std::weak_ptr<view::AbstractPathView> wv = view;

        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Model → View ----------------
        m->pos_changed.connect([wv](const utility::SPos& p) { if (auto pv = wv.lock()) pv->set_pos(p); });
        m->rect_changed.connect([wv](const utility::SRect& r) { if (auto pv = wv.lock()) pv->set_rect(r); });
        m->visible_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_visible(b); });
        m->select_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_select(b); });
        m->pressed_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_pressed(b); });

        v->pos_changed.connect([wm](const utility::SPos& p) { if (auto pv = wm.lock()) pv->set_pos(p); });
        v->rect_changed.connect([wm](const utility::SRect& r) { if (auto pv = wm.lock()) pv->set_rect(r); });
        v->visible_changed.connect([wm](bool b) { if (auto pv = wm.lock()) pv->set_visible(b); });
        v->select_changed.connect([wm](bool b) { if (auto pv = wm.lock()) pv->set_select(b); });
        v->pressed_changed.connect([wm](bool b) { if (auto pv = wm.lock()) pv->set_pressed(b); });
    }

    AbstractPathPresenter::~AbstractPathPresenter()
    {
        // Gracefully disconnect all signals on both sides
        auto msp = m_model;
        auto vsp = m_view;

        if (msp)
        {
            auto* m = static_cast<model::AbstractPathModel*>(msp.get());
            m->pos_changed.disconnectAll();
            m->rect_changed.disconnectAll();
            m->visible_changed.disconnectAll();
            m->select_changed.disconnectAll();
            m->pressed_changed.disconnectAll();
        }
    }

} // namespace nodeeditor::common::presenter
