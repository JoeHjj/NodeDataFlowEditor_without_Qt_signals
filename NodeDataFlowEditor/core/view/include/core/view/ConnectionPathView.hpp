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

#include "common/utility/ConnectionInfo.hpp"
#include "common/view/AbstractPathView.hpp"
#include "mvp/utility/Signal.hpp"
#include <QTimer>

namespace nodeeditor::core::view
{
    /**
     * @brief Represents a visual link (edge) between two ports in a node-based graph.
     *
     * ConnectionPathView manages its geometry, path drawing, active animation, and state flags.
     * It can dynamically update its endpoints as ports move or as the user drags a pending connection.
     */
    class ConnectionPathView : public common::view::AbstractPathView
    {
        Q_INTERFACES(QGraphicsItem)

    public:
        /**
         * @brief Construct a connection from a port.
         * @param startPoint Information about the port position and rect.
         * @param parent Optional parent graphics item.
         *
         * The connection will visually follow the cursor until its endpoint is set.
         */
        explicit ConnectionPathView(const ConnectionPortData& port, QGraphicsItem* parent = nullptr);
        explicit ConnectionPathView(const ConnectionPortData& port1, const ConnectionPortData& port2, QGraphicsItem* parent = nullptr);

        /**
         * @brief Destructor. Cleans up timers and visual resources.
         */
        ~ConnectionPathView();

        /**
         * @brief add an input or output endpoint.
         * @param isInput True if setting the input side; false for output.
         * @param point New position in scene coordinates.
         * @param portRect Bounding rectangle of the port.
         */
        void addPort(const ConnectionPortData& port);

        /**
         * @brief Update the connection when a connected node moves.
         * @param isInput True if the input port moved; false for output port.
         * @param newPos New position of the moved port in scene coordinates.
         * @param rect Bounding rectangle of the moved port.when forwarded port
         * use the group node rect
         */
        void onNodeMoved(bool isInput, const QPointF& newPos, const QRectF& rect);

        /**
         * @brief Update the endpoint position while the connection is being drawn interactively.
         * @param point New cursor or temporary endpoint position.
         */
        void updateEndPoint(const QPointF& point);

        /**
         * @brief Retrieve the data of the input and output ports.
         */
        ConnectionPortData inputPort() const;
        ConnectionPortData outputPort() const;

        /**
         * @brief Check or set the active state of the connection.
         *
         * Active connections can be drawn differently (e.g. with glow or animation)
         * to indicate data flow or selection.
         */
        bool isActivated() const;
        void setIsActive(bool newIsActive);

        /**
         * @brief Recompute the connection curve between the two endpoints.
         *
         * Typically called when a port moves or endpoint positions change.
         */
        void updatePath();

        /**
         * @brief Mark the connection as compatible or incompatible with its target.
         *
         * Used during drag-to-connect operations to provide user feedback.
         */
        void setIsCompatible(bool newIsCompatible);

        /**
         * @brief Check whether the connection is currently being destroyed.
         *
         * This is used to prevent updates during teardown animations or cleanup.
         */
        bool isDestroying() const;
        QPainterPath shape() const override;
        // -------------------- Signals & Setters --------------------
        base::mvp::utility::Signal<const common::utility::SPort&> inputPort_changed;
        void set_inputPort(const common::utility::SPort& p);

        void set_inputPos(const common::utility::SPos& p);
        void set_outputPos(const common::utility::SPos& p);

        base::mvp::utility::Signal<const common::utility::SPort&> outputPort_changed;
        void set_outputPort(const common::utility::SPort& p);

        base::mvp::utility::Signal<const common::utility::SPoint&> endPoint_changed;
        void set_endPoint(const common::utility::SPoint& p);

        /** @brief Emitted when active state changes. */
        base::mvp::utility::Signal<const bool&> active_changed;
        /** @brief Set active state and emit @ref active_changed. */
        void set_active(bool a);

        /** @brief Emitted when mouse pressed changes. */
        base::mvp::utility::Signal<const bool&> compatible_changed;
        base::mvp::utility::Signal<const common::utility::SPort&> input_changed;
        base::mvp::utility::Signal<const common::utility::SPort&> output_changed;

        void set_compatible(bool s);

        // -------------------- Optional Callbacks --------------------
        void setOnActiveChanged(std::function<void(const bool&)> cb);
        void setOnCompatibleChanged(std::function<void(const bool&)> cb);

    private:
        /**
         * @brief Paint the connection curve and optional animated elements.
         */
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

        /**
         * @brief Compute and store a smooth curved path between the start and end points.
         */
        void drawPath(const QPointF& startPoint, const QPointF& endPoint);

        /**
         * @brief Update internal animation state for active connection visuals.
         */
        void updateAnimationStatus();

        /**
         * @brief Compute the exact point on the input port for connection attachment.
         */
        QPointF computeInputPoint(const ConnectionPortData& port) const;

        /**
         * @brief Compute the exact point on the output port for connection attachment.
         */
        QPointF computeOutputPoint(const ConnectionPortData& port) const;

    private:
        ConnectionPortData m_inputPort;  ///< Data for input connection port.
        ConnectionPortData m_outputPort; ///< Data for output connection port.

        QPointF m_endPoint; ///< Current end point of the connection.

        QTimer m_animationTimer;           ///< Timer used for animating active connections.
        QVector<double> m_circlePositions; ///< Animation positions for decorative elements (e.g. flowing dots).

        bool m_isDestroying = false; ///< Flag set during cleanup to prevent further updates.

        bool active_{false};
        bool compatible_{false};

    private:
        /** @brief Called after active state changes in @ref set_active. */
        std::function<void(const bool&)> onActiveChanged;
        std::function<void(const bool&)> onCompatibleChanged;
        std::function<void(const common::utility::SPort&)> onInputPortChanged;
        std::function<void(const common::utility::SPort&)> onOutputPortChanged;
        std::function<void(const common::utility::SPoint&)> onEndPointChanged;
    };
} // namespace nodeeditor::core::view
