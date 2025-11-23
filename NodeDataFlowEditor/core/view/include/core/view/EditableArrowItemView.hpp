#pragma once

#include "common/view/AbstractItemView.hpp"

class QLineEdit;
class QGraphicsProxyWidget;
class QGraphicsTextItem;

namespace nodeeditor::core::view
{

    class EditableArrowItemView : public common::view::AbstractItemView
    {
    public:
        explicit EditableArrowItemView(const QString& text, QGraphicsItem* parent = nullptr);
        ~EditableArrowItemView() override = default;
        // API
        void setArrowBeforeLabel(bool value);
        void setEditable(bool value);

        qreal textWidth() const;
        qreal textHeight() const;

        // Signals / slots for each property
        base::mvp::utility::Signal<const std::string&> text_changed;
        void set_text(const std::string& t);
        void setOnTextChanged(std::function<void(const QString&)> cb);

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

        // Editing
        void startEditing();
        void finishEditing();

        void setShowArrow(bool newShowArrow);

    private:
        std::function<void(const QString&)> onTextChanged;

        void repositionElements();

    private:
        bool arrowBeforeLabel_{true};
        bool editable_{true};
        bool showArrow_{true};

        QGraphicsTextItem* labelItem_;
        QLineEdit* editWidget_;
        QGraphicsProxyWidget* editProxy_;

        // Internal storage for properties
        std::string text_;
    };

} // namespace nodeeditor::core::view
