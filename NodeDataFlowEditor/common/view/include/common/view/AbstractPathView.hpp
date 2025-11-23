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

#include "common/utility/ConnectionPortData.hpp"
#include "common/utility/GraphicsProperties.hpp"
#include "mvp/utility/Signal.hpp"
#include "mvp/view/IViewItem.hpp"
#include <QGraphicsItem>

namespace nodeeditor::common::view
{
    /**
     * @brief Represents a visual link (edge) between two ports in a node-based graph.
     *
     * AbstractPathView manages its geometry, path drawing, active animation, and state flags.
     * It can dynamically update its endpoints as ports move or as the user drags a pending connection.
     */
    class AbstractPathView : public base::mvp::view::IViewItem, public QGraphicsPathItem
    {
        Q_INTERFACES(QGraphicsItem)

    public:
        /**
         * @brief Construct a connection.
         * @param parent Optional parent graphics item.
         *
         */
        explicit AbstractPathView(QGraphicsItem* parent = nullptr);

        /**
         * @brief Destructor. Cleans up timers and visual resources.
         */
        ~AbstractPathView() override;

        QPainterPath shape() const override;
        // -------------------- Signals & Setters --------------------
        /** @brief Emitted when position changes. */
        base::mvp::utility::Signal<const utility::SPos&> pos_changed;
        /** @brief Set position and emit @ref pos_changed. */
        void set_pos(const utility::SPos& p);

        /** @brief Emitted when rectangle changes. */
        base::mvp::utility::Signal<const utility::SRect&> rect_changed;
        /** @brief Set rectangle and emit @ref rect_changed. */
        void set_rect(const utility::SRect& r);

        /** @brief Emitted when visibility changes. */
        base::mvp::utility::Signal<const bool&> visible_changed;
        /** @brief Set visibility and emit @ref visible_changed. */
        void set_visible(bool v);

        /** @brief Emitted when selection changes. */
        base::mvp::utility::Signal<const bool&> select_changed;
        /** @brief Set selection state and emit @ref select_changed. */
        void set_select(bool s);

        /** @brief Emitted when mouse pressed changes. */
        base::mvp::utility::Signal<const bool&> pressed_changed;
        /** @brief Set mouse pressed state and emit @ref pressed_changed. */
        void set_pressed(bool s);

        // -------------------- Optional Callbacks --------------------
        void setOnPosChanged(std::function<void(const utility::SPos&)> cb);
        void setOnRectChanged(std::function<void(const utility::SRect&)> cb);
        void setOnVisibleChanged(std::function<void(const bool&)> cb);
        void setOnSelectChanged(std::function<void(const bool&)> cb);
        void setOnPressedChanged(std::function<void(const bool&)> cb);

    private:
        /**
         * @brief Paint the connection curve and optional animated elements.
         */
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        bool sceneEvent(QEvent* event) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    protected:
        static QRectF toQRectF(const utility::SRect& r);

        QPainterPath m_currentPath; ///< Cached connection curve.

        utility::SPos pos_;
        utility::SRect rect_;
        QRectF m_rect;
        bool visible_{true};
        bool select_{false};
        bool pressed_{false};

    private:
        /** @brief Called after position changes in @ref set_pos. */
        std::function<void(const utility::SPos&)> onPosChanged;

        /** @brief Called after rectangle changes in @ref set_rect. */
        std::function<void(const utility::SRect&)> onRectChanged;

        /** @brief Called after visibility changes in @ref set_visible. */
        std::function<void(const bool&)> onVisibleChanged;

        /** @brief Called after selection changes in @ref set_select. */
        std::function<void(const bool&)> onSelectChanged;

        /** @brief Called after pressed changes in @ref set_pressed. */
        std::function<void(const bool&)> onPressedChanged;
    };
} // namespace nodeeditor::common::view
