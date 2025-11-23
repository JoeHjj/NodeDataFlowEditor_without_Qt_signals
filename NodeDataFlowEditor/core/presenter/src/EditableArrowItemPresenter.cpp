#include "core/presenter/EditableArrowItemPresenter.hpp"

#include "core/model/EditableArrowItemModel.hpp"
#include "core/view/EditableArrowItemView.hpp"
#include <memory>

namespace nodeeditor::core::presenter
{
    EditableArrowItemPresenter::EditableArrowItemPresenter(std::shared_ptr<model::EditableArrowItemModel> model,
                                                           std::shared_ptr<view::EditableArrowItemView> view)
        : common::presenter::AbstractItemPresenter(model, view)
    {
        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Connect Model -> View ----------------
        m->text_changed.connect([v](const std::string& t) { v->set_text(t); });

        // ---------------- Connect View -> Model ----------------
        v->text_changed.connect([m](const std::string& t) { m->set_text(t); });
    }

    EditableArrowItemPresenter::~EditableArrowItemPresenter() = default;
} // namespace nodeeditor::core::presenter
