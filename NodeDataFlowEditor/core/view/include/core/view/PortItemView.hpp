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

#pragma once

#include "common/taggable/Taggable.hpp"
#include "common/utility/ConnectionInfo.hpp"
#include "common/view/AbstractItemView.hpp"

namespace nodeeditor::core::view
{
    class EditableArrowItemView;

    /**
     * @brief Represents a node port in a graph editor.
     *
     * PortItemView manages both the visual representation (label + optional arrow)
     * and user interactions such as hovering, clicking, and editing. Ports
     * can also have multiple connections and carry metadata tags via Taggable.
     */
    class PortItemView : public common::view::AbstractItemView, public common::taggable::Taggable
    {
    public:
        /**
         * @brief Orientation or type of the port.
         */

        /** @name Constructors & Destructor */
        ///@{
        /**
         * @brief Construct a new PortItemView with separate internal and displayed names.
         * @param name Internal port name.
         * @param displayName Text shown to the user.
         * @param moduleName Name of the parent module.
         * @param orientation Orientation of the port (Input/Parameter/Output).
         * @param parent Optional parent QGraphicsItem.
         */
        explicit PortItemView(QString name,
                              QString displayName,
                              QString moduleName,
                              common::utility::SPort::Orientation orientation = common::utility::SPort::Orientation::Output,
                              QGraphicsItem* parent = nullptr);

        /**
         * @brief Construct a new PortItemView with same internal and displayed name.
         * @param name Port internal and displayed name.
         * @param moduleName Name of the parent module.
         * @param orientation Orientation of the port.
         * @param parent Optional parent QGraphicsItem.
         */
        explicit PortItemView(QString name,
                              QString moduleName,
                              common::utility::SPort::Orientation = common::utility::SPort::Orientation::Output,
                              QGraphicsItem* parent = nullptr);

        /**
         * @brief Destructor.
         */
        ~PortItemView() override;
        ///@}

        /** @name QGraphicsItem Overrides */
        ///@{
        /**
         * @brief Returns the bounding rectangle for painting and interaction.
         */
        QRectF boundingRect() const override;

        /** @name Orientation & Identity */
        ///@{
        /**
         * @brief Set the orientation of the port.
         * @param o New orientation.
         */
        void setOrientation(common::utility::SPort::Orientation o);

        /**
         * @brief Get the orientation of the port.
         * @return Orientation
         */
        common::utility::SPort::Orientation getOrientation() const;

        /**
         * @brief Check if this is any kind of input port (Input or Parameter).
         * @return true if input or parameter
         */
        bool isAnyInputPort() const;

        /**
         * @brief Check if this is a parameter port.
         * @return true if Parameter
         */
        bool isParameterPort() const;

        /**
         * @brief Check if this is an input port.
         * @return true if Input
         */
        bool isInputPort() const;

        /**
         * @brief Check if this is an output port.
         * @return true if Output
         */
        bool isOutputPort() const;

        /**
         * @brief Set the displayed label of the port.
         * @param text New displayed text.
         */
        void setDisplayName(const QString& text);

        /**
         * @brief Get the displayed label.
         * @return QString Displayed text
         */
        QString displayName() const;

        /**
         * @brief Set the internal name of the port.
         * @param text Internal name
         */
        void setName(const QString& text);

        /**
         * @brief Get the internal name.
         * @return QString Internal name
         */
        QString name() const;

        /**
         * @brief Set the parent module name.
         * @param moduleName Name of module
         */
        void setModuleName(const QString& moduleName);

        /**
         * @brief Get the parent module name.
         * @return QString Module name
         */
        QString moduleName() const;
        ///@}

        /** @name Connections */
        ///@{
        /**
         * @brief Returns a representation of the connection port.
         * @return ConnectionPort
         */
        // ConnectionPort getConnectionPortData() const;
        ///@}

        /**
         * @brief Set the color of the port.
         * @param color QColor to use.
         */
        void setColor(const QColor& color);
        ///@}
        ///
        std::function<void(const QString&)> onNameChanged;

        // Signals / slots for each property
        base::mvp::utility::Signal<const std::string&> name_changed;
        void set_name(const std::string& t);

        base::mvp::utility::Signal<const std::string&> module_name_changed;
        void set_module_name(const std::string& t);

        base::mvp::utility::Signal<const std::string&> display_name_changed;
        void set_display_name(const std::string& t);

        base::mvp::utility::Signal<const common::utility::SPort::Orientation&> orientation_changed;
        void set_orientation(const common::utility::SPort::Orientation& t);

    protected:
        /**
         * @brief Paints the port label and arrow.
         * @param painter Painter object for drawing.
         * @param option Style options.
         * @param widget Optional widget context.
         */
        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget* widget = nullptr) override;
        ///@}
    private:
        /**
         * @brief Repositions the label and arrow according to the port orientation.
         */
        void repositionLabel();

    private:
        EditableArrowItemView* m_editableArrow = nullptr;  ///< Visual component showing label + arrow
        QString m_portName;                                ///< Internal port name
        QString m_portDisplayName;                         ///< Displayed label
        QString m_moduleName;                              ///< Parent module name
        common::utility::SPort::Orientation m_orientation; ///< Port orientation

        QColor m_hoveredColor = QColor(0, 255, 0, 100);  ///< Highlight when hovered
        QColor m_clickedColor = QColor(80, 255, 0, 120); ///< Highlight when clicked
        QColor m_portColor = QColor(110, 110, 110);      ///< Base color
    };
} // namespace nodeeditor::core::view
