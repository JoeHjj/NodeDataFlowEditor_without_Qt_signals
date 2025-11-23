#include "common/presenter/AbstractItemPresenter.hpp"
#include "common/model/AbstractItemModel.hpp"
#include "common/view/AbstractItemView.hpp"
#include "mvp/presenter/Presenter.hpp"
#include <QDebug>
#include <memory>

namespace nodeeditor::common::presenter
{

    AbstractItemPresenter::AbstractItemPresenter(std::shared_ptr<model::AbstractItemModel> model,
                                                 std::shared_ptr<view::AbstractItemView> view)
        : base::mvp::presenter::Presenter(model, view)
    {
        std::weak_ptr<model::AbstractItemModel> wm = model;
        std::weak_ptr<view::AbstractItemView> wv = view;

        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Model → View ----------------
        m->rotation_changed.connect([wv](double r) { if (auto pv = wv.lock()) pv->set_rotation(r); });
        m->scale_changed.connect([wv](double s) { if (auto pv = wv.lock()) pv->set_scale(s); });
        m->pos_changed.connect([wv](const utility::SPos& p) { if (auto pv = wv.lock()) pv->set_pos(p); });
        m->rect_changed.connect([wv](const utility::SRect& r) { if (auto pv = wv.lock()) pv->set_rect(r); });
        m->enable_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_enable(b); });
        m->visible_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_visible(b); });
        m->active_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_active(b); });
        m->select_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_select(b); });
        m->pressed_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_pressed(b); });
        m->double_clicked_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_double_clicked(b); });
        m->moving_changed.connect([wv](bool b) { if (auto pv = wv.lock()) pv->set_moving(b); });

        // ---------------- View → Model ----------------
        v->rotation_changed.connect([wm](double r) { if (auto pm = wm.lock()) pm->set_rotation(r); });
        v->scale_changed.connect([wm](double s) { if (auto pm = wm.lock()) pm->set_scale(s); });
        v->pos_changed.connect([wm](const utility::SPos& p) { if (auto pm = wm.lock()) pm->set_pos(p); });
        v->rect_changed.connect([wm](const utility::SRect& r) { if (auto pm = wm.lock()) pm->set_rect(r); });
        v->enable_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_enable(b); });
        v->visible_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_visible(b); });
        v->active_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_active(b); });
        v->select_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_select(b); });
        v->pressed_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_pressed(b); });
        v->double_clicked_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_double_clicked(b); });
        v->moving_changed.connect([wm](bool b) { if (auto pm = wm.lock()) pm->set_moving(b); });
    }

    AbstractItemPresenter::~AbstractItemPresenter()
    {
        // Gracefully disconnect all signals on both sides
        auto msp = m_model;
        auto vsp = m_view;

        if (msp)
        {
            auto* m = static_cast<model::AbstractItemModel*>(msp.get());
            m->rotation_changed.disconnectAll();
            m->scale_changed.disconnectAll();
            m->pos_changed.disconnectAll();
            m->rect_changed.disconnectAll();
            m->enable_changed.disconnectAll();
            m->visible_changed.disconnectAll();
            m->active_changed.disconnectAll();
            m->select_changed.disconnectAll();
            m->pressed_changed.disconnectAll();
            m->double_clicked_changed.disconnectAll();
            m->moving_changed.disconnectAll();
        }

        if (vsp)
        {
            auto* v = static_cast<view::AbstractItemView*>(vsp.get());
            v->rotation_changed.disconnectAll();
            v->scale_changed.disconnectAll();
            v->pos_changed.disconnectAll();
            v->rect_changed.disconnectAll();
            v->enable_changed.disconnectAll();
            v->visible_changed.disconnectAll();
            v->active_changed.disconnectAll();
            v->select_changed.disconnectAll();
            v->pressed_changed.disconnectAll();
            v->double_clicked_changed.disconnectAll();
            v->moving_changed.disconnectAll();
        }
    }

} // namespace nodeeditor::common::presenter
