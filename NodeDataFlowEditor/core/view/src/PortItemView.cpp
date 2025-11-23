/*
    MIT License

    Copyright (c) 2025 Joseph Al Hajjar

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "core/view/PortItemView.hpp"
#include "core/view/EditableArrowItemView.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace nodeeditor::core::view
{
    PortItemView::PortItemView(QString name,
                               QString displayName,
                               QString moduleName,
                               common::utility::SPort::Orientation orientation,
                               QGraphicsItem* parent)
        : nodeeditor::common::view::AbstractItemView(parent)
    {
        setAcceptedMouseButtons(Qt::LeftButton);
        setAcceptHoverEvents(true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemIsMovable, false);
        m_editableArrow = new EditableArrowItemView(m_portDisplayName, this);
        m_editableArrow->setArrowBeforeLabel(isAnyInputPort());
        m_editableArrow->setEditable(true);
        m_editableArrow->setColor(m_portColor);

        m_editableArrow->setOnTextChanged([this](const QString& newText) {
            set_display_name(newText.toStdString());
        });

        setName(name);
        setDisplayName(displayName);
        setOrientation(orientation);
        setModuleName(moduleName);

        repositionLabel();
    }

    PortItemView::PortItemView(QString name,
                               QString moduleName,
                               common::utility::SPort::Orientation orientation,
                               QGraphicsItem* parent)
        : PortItemView(name, name, moduleName, orientation, parent)
    {
    }

    PortItemView::~PortItemView() = default;

    QRectF
    PortItemView::boundingRect() const
    {
        if (!m_editableArrow)
            return QRectF();

        qreal arrowWidth = 10;
        qreal spacing = 4;
        qreal labelWidth = m_editableArrow->textWidth();
        qreal width = arrowWidth + spacing + labelWidth;
        qreal height = m_editableArrow->textHeight();

        return QRectF(0, 0, width, height);
    }

    void
    PortItemView::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
    {
        painter->setRenderHint(QPainter::Antialiasing);

        QRectF rect = boundingRect();

        // Draw hover highlight
        if (hovered_)
        {
            painter->setBrush(m_hoveredColor);
            painter->setPen(Qt::NoPen);
            painter->drawRect(rect);
        }

        // Draw clicked highlight
        if (pressed_)
        {
            painter->setBrush(m_clickedColor);
            painter->setPen(Qt::NoPen);
            painter->drawRect(rect);
        }
    }

    void
    PortItemView::setOrientation(common::utility::SPort::Orientation o)
    {
        if (m_orientation == o)
            return;

        prepareGeometryChange();
        m_orientation = o;
        if (m_editableArrow)
            m_editableArrow->setArrowBeforeLabel(isAnyInputPort());

        repositionLabel();
        set_orientation(o);
        update();
    }

    common::utility::SPort::Orientation
    PortItemView::getOrientation() const
    {
        return m_orientation;
    }

    bool
    PortItemView::isAnyInputPort() const
    {
        return m_orientation == common::utility::SPort::Orientation::Input || m_orientation == common::utility::SPort::Orientation::Parameter;
    }

    bool
    PortItemView::isParameterPort() const
    {
        return m_orientation == common::utility::SPort::Orientation::Parameter;
    }

    bool
    PortItemView::isInputPort() const
    {
        return m_orientation == common::utility::SPort::Orientation::Input;
    }

    bool
    PortItemView::isOutputPort() const
    {
        return m_orientation == common::utility::SPort::Orientation::Output;
    }

    void
    PortItemView::setDisplayName(const QString& text)
    {
        if (m_portDisplayName == text)
            return;

        m_portDisplayName = text;
        if (m_editableArrow)
            m_editableArrow->set_text(m_portDisplayName.toStdString());

        set_display_name(text.toStdString());

        prepareGeometryChange();
        update();
    }

    void PortItemView::set_name(const std::string& t)
    {
        if (m_portName == t)
            return;
        setName(QString::fromStdString(t));

        name_changed.notify(t);
    }

    void PortItemView::set_module_name(const std::string& t)
    {
        if (m_moduleName == t)
            return;
        setModuleName(QString::fromStdString(t));

        module_name_changed.notify(t);
    }

    void PortItemView::set_display_name(const std::string& t)
    {
        if (m_portDisplayName == t)
            return;
        setDisplayName(QString::fromStdString(t));

        display_name_changed.notify(t);
    }

    void PortItemView::set_orientation(const common::utility::SPort::Orientation& t)
    {
        if (m_orientation == t)
            return;
        setOrientation(t);

        orientation_changed.notify(t);
    }

    QString
    PortItemView::displayName() const
    {
        return m_portDisplayName;
    }

    void
    PortItemView::setName(const QString& text)
    {
        m_portName = text;
        set_name(text.toStdString());
    }

    QString
    PortItemView::name() const
    {
        return m_portName;
    }

    void
    PortItemView::setModuleName(const QString& moduleName)
    {
        m_moduleName = moduleName;
        set_module_name(moduleName.toStdString());
    }

    QString
    PortItemView::moduleName() const
    {
        return m_moduleName;
    }

    void
    PortItemView::setColor(const QColor& color)
    {
        m_portColor = color;
        if (m_editableArrow)
            m_editableArrow->setColor(color);
        update();
    }

    void
    PortItemView::repositionLabel()
    {
        if (!m_editableArrow)
            return;

        // Align arrow and label horizontally
        if (isAnyInputPort())
        {
            m_editableArrow->setPos(0, 0);
        }
        else
        {
            // Output: shift arrow to the right of label
            m_editableArrow->setPos(0, 0);
            m_editableArrow->setArrowBeforeLabel(false);
        }
    }
} // namespace nodeeditor::core::view
