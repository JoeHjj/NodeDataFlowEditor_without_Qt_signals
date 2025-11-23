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

#include "core/view/ConnectionPathView.hpp"
#include "common/utility/ConnectionInfo.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStyleOption>
#include <QtMath>
#include <qcoreevent.h>

namespace nodeeditor::core::view
{
    ConnectionPathView::ConnectionPathView(const ConnectionPortData& port, QGraphicsItem* parent)
        : nodeeditor::common::view::AbstractPathView(parent)
    {
        setFlag(ItemIsSelectable, true);
        QPen pen(Qt::red, 2);
        setPen(pen);
        setZValue(1);

        addPort(port);

        m_circlePositions = {0.0, 0.2, 0.4, 0.6, 0.8};

        QObject::connect(&m_animationTimer, &QTimer::timeout, [this]() {
            for (auto& position : m_circlePositions)
            {
                if (!m_inputPort.isInput)
                {
                    // forward
                    position = std::fmod(position + 0.01, 1.0);
                }
                else
                {
                    // backward
                    position -= 0.01;
                    if (position < 0.0)
                        position += 1.0;
                }
            }
            update();
        });
    }

    ConnectionPathView::ConnectionPathView(const ConnectionPortData& port1, const ConnectionPortData& port2, QGraphicsItem* parent)
        : ConnectionPathView(port1, parent)
    {
        addPort(port2);
    }

    void
    ConnectionPathView::addPort(const ConnectionPortData& port)
    {
        if (port.isInput)
        {
            m_inputPort = port;
        }
        else
        {
            m_outputPort = port;
        }
        updatePath();
    }

    ConnectionPathView::~ConnectionPathView()
    {
        m_isDestroying = true;
    }

    void
    ConnectionPathView::onNodeMoved(bool isInput, const QPointF& newPos, const QRectF& rect)
    {
        if (isInput)
        {
            m_inputPort.scenePos = newPos;
        }
        else
        {
            m_outputPort.scenePos = newPos;
            if (rect.isValid() && !rect.isNull())
                m_outputPort.rect = rect;
        }
        updatePath();
    }

    void
    ConnectionPathView::updateEndPoint(const QPointF& point)
    {
        m_endPoint = point;
        updatePath();
    }

    ConnectionPortData
    ConnectionPathView::inputPort() const
    {
        return m_inputPort;
    }

    ConnectionPortData
    ConnectionPathView::outputPort() const
    {
        return m_outputPort;
    }

    void
    ConnectionPathView::updateAnimationStatus()
    {
        if (active_)
            m_animationTimer.start(30);
        else
            m_animationTimer.stop();
    }

    QPointF
    ConnectionPathView::computeInputPoint(const ConnectionPortData& port) const
    {
        return {port.scenePos.x() + port.rect.width(), port.scenePos.y() + (port.rect.height() / 2) - 3};
    }

    QPointF
    ConnectionPathView::computeOutputPoint(const ConnectionPortData& port) const
    {
        return {port.scenePos.x(), port.scenePos.y() + (port.rect.height() / 2) - 3};
    }

    void
    ConnectionPathView::drawPath(const QPointF& startPoint, const QPointF& endPoint)
    {
        prepareGeometryChange();

        if (startPoint == endPoint)
            return;
        if (startPoint.isNull() || endPoint.isNull())
            return;

        if (compatible_ || (!m_inputPort.portName.isEmpty() && !m_outputPort.portName.isEmpty()))
            setPen(QPen(Qt::green, 2));
        else if (!compatible_)
            setPen(QPen(Qt::red, 2));
        QPainterPath path(startPoint);
        qreal dx = endPoint.x() - startPoint.x();
        QPointF ctrl1 = startPoint + QPointF(dx * 0.25, 0);
        QPointF ctrl2 = endPoint - QPointF(dx * 0.25, 0);
        path.cubicTo(ctrl1, ctrl2, endPoint);

        m_currentPath = path;
        setPath(path);

        updateAnimationStatus();
    }

    void
    ConnectionPathView::setIsCompatible(bool newIsCompatible)
    {
        if (compatible_ == newIsCompatible)
            return;

        compatible_ = newIsCompatible;
        QPen pen(Qt::red, 2);

        if (compatible_)
            pen = QPen(Qt::green, 2);

        setPen(pen);
        set_compatible(newIsCompatible);
        update();
    }

    bool
    ConnectionPathView::isDestroying() const
    {
        return m_isDestroying;
    }

    bool
    ConnectionPathView::isActivated() const
    {
        return active_;
    }

    void
    ConnectionPathView::setIsActive(bool newIsActive)
    {
        if (active_ == newIsActive)
            return;

        active_ = newIsActive;
        set_active(newIsActive);
        update();
    }

    void
    ConnectionPathView::updatePath()
    {
        QPointF startPoint;
        QPointF endPoint;

        // Start from input
        if (!m_inputPort.scenePos.isNull())
        {
            startPoint = computeOutputPoint(m_inputPort);
        }

        if (!m_outputPort.scenePos.isNull()) // start from output
        {
            endPoint = computeInputPoint(m_outputPort);
        }

        if (!startPoint.isNull() && !endPoint.isNull())
            drawPath(startPoint, endPoint);
        else if (!endPoint.isNull()) // no output yet
            drawPath(endPoint, m_endPoint);
        else if (!startPoint.isNull())
            drawPath(startPoint, m_endPoint);
    }

    QPainterPath
    ConnectionPathView::shape() const
    {
        QPainterPathStroker stroker;
        stroker.setWidth(10); // thickness for selection hit area
        return stroker.createStroke(m_currentPath);
    }

    void ConnectionPathView::set_inputPort(const common::utility::SPort& p)
    {
        if (m_inputPort.portName == p.name && m_inputPort.moduleName == p.nodeName && m_inputPort.isInput == p.input)
            return;
        m_inputPort.portName = QString::fromStdString(p.name);
        m_inputPort.moduleName = QString::fromStdString(p.nodeName);
        m_inputPort.isInput = p.input;
        common::utility::SPort inP{m_inputPort.portName.toStdString(), m_inputPort.moduleName.toStdString(), m_inputPort.isInput};
        inputPort_changed.notify(inP);
        if (onInputPortChanged)
            onInputPortChanged(inP);
        updatePath();
    }

    void ConnectionPathView::set_inputPos(const common::utility::SPos& p)
    {
        m_inputPort.scenePos.setX(p.x);
        m_inputPort.scenePos.setY(p.y);
        updatePath();
    }

    void ConnectionPathView::set_outputPos(const common::utility::SPos& p)
    {
        m_outputPort.scenePos.setX(p.x);
        m_outputPort.scenePos.setY(p.y);
        updatePath();
    }

    void ConnectionPathView::set_outputPort(const common::utility::SPort& p)
    {
        if (m_outputPort.portName == p.name && m_outputPort.moduleName == p.nodeName && m_outputPort.isInput == p.input)
            return;
        m_outputPort.portName = QString::fromStdString(p.name);
        m_outputPort.moduleName = QString::fromStdString(p.nodeName);
        m_outputPort.isInput = p.input;
        common::utility::SPort outP{m_outputPort.portName.toStdString(), m_outputPort.moduleName.toStdString(), m_outputPort.isInput};
        outputPort_changed.notify(outP);
        if (onOutputPortChanged)
            onOutputPortChanged(outP);
        updatePath();
    }

    void ConnectionPathView::set_endPoint(const common::utility::SPoint& p)
    {
        if (m_endPoint.x() == p.x && m_endPoint.y() == p.y)
            return;
        m_endPoint.setX(p.x);
        m_endPoint.setY(p.y);

        common::utility::SPoint endP;
        endP.x = m_endPoint.x();
        endP.y = m_endPoint.y();

        endPoint_changed.notify(endP);
        if (onEndPointChanged)
            onEndPointChanged(endP);
        updatePath();
    }

    void ConnectionPathView::set_compatible(bool e)
    {
        if (compatible_ == e)
            return;
        compatible_ = e;
        setIsCompatible(e);
        compatible_changed.notify(compatible_);
        if (onCompatibleChanged)
            onCompatibleChanged(compatible_);
        update();
    }

    void ConnectionPathView::set_active(bool a)
    {
        if (active_ == a)
            return;
        active_ = a;
        active_changed.notify(active_);
        if (onActiveChanged)
            onActiveChanged(active_);
        updateAnimationStatus();
        update();
    }

    void
    ConnectionPathView::paint(QPainter* painter,
                              const QStyleOptionGraphicsItem* option,
                              QWidget*)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);

        if (option->state & QStyle::State_Selected)
        {
            QPen glowPen(QColor(0, 150, 255, 180)); // bright blue, semi-transparent
            glowPen.setWidth(10);                   // outer glow width
            glowPen.setCapStyle(Qt::RoundCap);
            glowPen.setJoinStyle(Qt::RoundJoin);
            painter->setPen(glowPen);
            painter->drawPath(m_currentPath);

            QPen innerGlow(QColor(0, 180, 255, 255)); // stronger blue
            innerGlow.setWidth(6);
            innerGlow.setCapStyle(Qt::RoundCap);
            innerGlow.setJoinStyle(Qt::RoundJoin);
            painter->setPen(innerGlow);
            painter->drawPath(m_currentPath);
        }
        QPen normalPen = pen();
        normalPen.setCapStyle(Qt::RoundCap);
        normalPen.setJoinStyle(Qt::RoundJoin);
        painter->setPen(normalPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_currentPath);
        if (active_)
        {
            if (compatible_ || (!m_inputPort.portName.isEmpty() && !m_outputPort.portName.isEmpty()))
                painter->setBrush(Qt::green);
            else
                painter->setBrush(Qt::red);
            painter->setPen(Qt::NoPen);

            for (qreal t : std::as_const(m_circlePositions))
            {
                QPointF pt = m_currentPath.pointAtPercent(t);
                painter->drawEllipse(pt, 5, 5);
            }
        }
    }

    // -------------------- Callbacks Setters --------------------
    void ConnectionPathView::setOnActiveChanged(std::function<void(const bool&)> cb) { onActiveChanged = std::move(cb); }

} // namespace nodeeditor::core::view
