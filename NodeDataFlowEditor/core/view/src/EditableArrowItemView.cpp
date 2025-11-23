#include "core/view/EditableArrowItemView.hpp"

#include <QFont>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace nodeeditor::core::view
{

    EditableArrowItemView::EditableArrowItemView(const QString& text, QGraphicsItem* parent)
        : common::view::AbstractItemView(parent)
    {
        setAcceptedMouseButtons(Qt::LeftButton);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemIsMovable, false);

        labelItem_ = new QGraphicsTextItem(text, this);
        labelItem_->setDefaultTextColor(Qt::white);
        labelItem_->setFont(QFont("Arial", 10, QFont::Bold));

        editWidget_ = new QLineEdit(text);
        editProxy_ = new QGraphicsProxyWidget(this);
        editProxy_->setWidget(editWidget_);
        editProxy_->setVisible(false);

        QObject::connect(editWidget_, &QLineEdit::editingFinished, [this]() {
            finishEditing();
        });

        text_ = text.toStdString();
        repositionElements();

        setOnDoubleClickedChanged([this](const bool& doubleClicked) {
            if (doubleClicked)
            {
                startEditing();
            }
        });
    }

    // ---------------- API -------------------
    void EditableArrowItemView::setArrowBeforeLabel(bool value)
    {
        arrowBeforeLabel_ = value;
        repositionElements();
    }

    void EditableArrowItemView::setEditable(bool value)
    {
        editable_ = value;
    }

    qreal EditableArrowItemView::textWidth() const
    {
        return labelItem_ ? labelItem_->boundingRect().width() : 0.0;
    }

    qreal EditableArrowItemView::textHeight() const
    {
        return labelItem_ ? labelItem_->boundingRect().height() : 0.0;
    }

    // ---------------- QGraphicsItem -------------------
    QRectF EditableArrowItemView::boundingRect() const
    {
        const qreal arrowW = 10.0;
        const qreal spacing = 4.0;
        return QRectF(0, 0, arrowW + spacing + textWidth(), std::max<qreal>(arrowW, textHeight()));
    }

    void EditableArrowItemView::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
    {
        if (!painter)
            return;
        if (!showArrow_)
            return;

        painter->setRenderHint(QPainter::Antialiasing, true);

        const qreal arrowW = 10.0;
        const qreal arrowH = 10.0;
        const qreal spacing = 4.0;
        QPointF arrowPos = arrowBeforeLabel_ ? QPointF(0, (textHeight() - arrowH) / 2.0)
                                             : QPointF(textWidth() + spacing, (textHeight() - arrowH) / 2.0);

        QPolygonF arrow;
        arrow << QPointF(0, 0)
              << QPointF(arrowW, arrowH / 2.0)
              << QPointF(0, arrowH)
              << QPointF(0, 0);

        if (arrowBeforeLabel_)
        {
            QTransform t;
            const QRectF br = arrow.boundingRect();
            t.translate(br.center().x(), br.center().y());
            t.rotate(180);
            t.translate(-br.center().x(), -br.center().y());
            arrow = t.map(arrow);
        }

        arrow.translate(arrowPos);

        painter->setBrush(color_);
        painter->setPen(Qt::NoPen);
        painter->drawPolygon(arrow);
    }

    void EditableArrowItemView::startEditing()
    {
        if (!labelItem_ || !editProxy_ || !editWidget_)
            return;

        labelItem_->setVisible(false);
        editProxy_->setVisible(true);
        editWidget_->setText(QString::fromStdString(text_));
        editWidget_->selectAll();
        editWidget_->setFocus();
    }

    void EditableArrowItemView::finishEditing()
    {
        if (!editProxy_ || !editProxy_->isVisible())
            return;

        editProxy_->setVisible(false);
        if (labelItem_)
            labelItem_->setVisible(true);

        set_text(editWidget_->text().toStdString());
        labelItem_->setPlainText(editWidget_->text());

        if (onTextChanged)
            onTextChanged(editWidget_->text());
        repositionElements();
    }

    void EditableArrowItemView::repositionElements()
    {
        if (!labelItem_ || !editProxy_)
            return;

        prepareGeometryChange();

        labelItem_->adjustSize();
        editProxy_->setGeometry(labelItem_->boundingRect());

        qreal xOffset = 0.;

        if (showArrow_)
            xOffset = arrowBeforeLabel_ ? 10.0 + 4.0 : 0.0;

        labelItem_->setPos(xOffset, 0);
        editProxy_->setPos(xOffset, 0);

        update();
    }

    void EditableArrowItemView::setShowArrow(bool newShowArrow)
    {
        if (showArrow_ == newShowArrow)
            return;
        showArrow_ = newShowArrow;

        prepareGeometryChange();
        update();
    }

    void EditableArrowItemView::set_text(const std::string& t)
    {
        if (text_ == t)
            return;

        text_ = t;

        if (labelItem_)
            labelItem_->setPlainText(QString::fromStdString(t));
        if (editWidget_)
            editWidget_->setText(QString::fromStdString(t));

        text_changed.notify(text_);

        repositionElements();
    }

    void EditableArrowItemView::setOnTextChanged(std::function<void(const QString&)> cb) { onTextChanged = std::move(cb); }

} // namespace nodeeditor::core::view
