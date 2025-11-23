#include "core/view/NodeItemView.hpp"
#include "core/view/EditableArrowItemView.hpp"
#include "core/view/PortItemView.hpp"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QLinearGradient>
#include <QPainter>
#include <algorithm>
#include <cmath>
#include <utility>

namespace nodeeditor::core::view
{

    NodeItemView::NodeItemView(QString nodeName,
                               const QString& nodeDisplayedName,
                               QColor titleColor,
                               QGraphicsItem* parent)
        : nodeeditor::common::view::AbstractItemView(parent)
        , m_nodeName(std::move(nodeName))
        , m_displayedNodeName(nodeDisplayedName.isEmpty() ? m_nodeName : nodeDisplayedName)
        , m_nodeNameColor(titleColor)
    {
        m_nodeNameLabel = new EditableArrowItemView(m_displayedNodeName, this);
        m_nodeNameLabel->setShowArrow(false);
        m_titleHeight = m_nodeNameLabel->boundingRect().height();

        m_nodeNameLabel->setOnTextChanged([this](const QString& t) { set_text(t.toStdString()); });
        pos_changed.connect([this](const common::utility::SPos&) { updateLayout(); });
        setFlags(ItemIsMovable | ItemIsSelectable);
        setFlag(ItemSendsGeometryChanges, true);
        setAcceptHoverEvents(true);

        updateLayout();
    }

    NodeItemView::NodeItemView(QString nodeName,
                               const QString& nodeDisplayedName,
                               QGraphicsItem* parent)
        : NodeItemView(std::move(nodeName), nodeDisplayedName, Qt::blue, parent)
    {
    }

    NodeItemView::~NodeItemView()
    {
        try
        {
            disconnectAllPorts();
        }
        catch (...)
        {
            qWarning() << "Exception in NodeItemView destructor";
        }
    }

    // ----------------------
    // ADD PORTS
    // ----------------------
    std::shared_ptr<PortItemView> NodeItemView::addInput(const QString& name)
    {
        auto input = std::make_shared<PortItemView>(name, m_nodeName, common::utility::SPort::Orientation::Input, this);
        input->setColor(Qt::gray);
        input->display_name_changed.connect([this](const std::string&) { updateLayout(); });
        m_inputs.append(input);
        updateLayout();
        return input;
    }

    std::shared_ptr<PortItemView> NodeItemView::addOutput(const QString& name)
    {
        auto output = std::make_shared<PortItemView>(name, m_nodeName, common::utility::SPort::Orientation::Output, this);
        output->setColor(Qt::gray);
        output->display_name_changed.connect([this](const std::string&) { updateLayout(); });
        m_outputs.append(output);
        updateLayout();
        return output;
    }

    std::shared_ptr<PortItemView> NodeItemView::addInput(const QString& name, const QString& displayName)
    {
        auto p = addInput(name);
        if (p)
            p->setDisplayName(displayName);
        return p;
    }

    std::shared_ptr<PortItemView> NodeItemView::addOutput(const QString& name, const QString& displayName)
    {
        auto p = addOutput(name);
        if (p)
            p->setDisplayName(displayName);
        return p;
    }

    std::shared_ptr<PortItemView> NodeItemView::addParamInput(const QString& name)
    {
        auto input = std::make_shared<PortItemView>(name, m_nodeName, common::utility::SPort::Orientation::Parameter, this);
        input->setColor(Qt::gray);
        input->display_name_changed.connect([this](const std::string&) { updateLayout(); });
        updateLayout();
        return input;
    }

    std::shared_ptr<PortItemView> NodeItemView::addParameter(QWidget* widget, const QString& name)
    {
        if (widget == nullptr)
            return nullptr;
        auto* proxy = new QGraphicsProxyWidget(this);
        proxy->setWidget(widget);
        m_parameterWidgets.insert(widget, proxy);
        auto port = addParamInput(name);
        m_parameterPorts.insert(port, proxy);
        updateLayout();
        return port;
    }

    std::shared_ptr<PortItemView> NodeItemView::addParameter(QWidget* widget, const QString& name, const QString& displayName)
    {
        auto port = addParameter(widget, name);
        if (port)
            port->setDisplayName(displayName);
        return port;
    }

    // ----------------------
    // REMOVE PORTS
    // ----------------------
    void NodeItemView::removeInput(const std::shared_ptr<PortItemView>& input)
    {
        if (!m_inputs.contains(input))
            return;
        m_inputs.removeOne(input);
        updateLayout();
    }

    void NodeItemView::removeOutput(const std::shared_ptr<PortItemView>& output)
    {
        if (!m_outputs.contains(output))
            return;
        m_outputs.removeOne(output);
        updateLayout();
    }

    void NodeItemView::removeParamInput(const std::shared_ptr<PortItemView>& input)
    {
        if (!m_parameterPorts.contains(input))
            return;
        auto* proxy = m_parameterPorts.value(input);
        auto* widget = m_parameterWidgets.key(proxy);
        m_parameterPorts.remove(input);
        m_parameterWidgets.remove(widget);
        proxy->deleteLater();
        updateLayout();
    }

    void NodeItemView::removeInput(const QString& name)
    {
        for (auto& port : m_inputs)
        {
            if (port && port->name() == name)
            {
                removeInput(port);
                break;
            }
        }
    }

    void NodeItemView::removeOutput(const QString& name)
    {
        for (auto& port : m_outputs)
        {
            if (port && port->name() == name)
            {
                removeOutput(port);
                break;
            }
        }
    }

    void NodeItemView::removeParamInput(const QString& name)
    {
        for (auto& port : m_parameterPorts.keys())
        {
            if (port && port->name() == name)
            {
                removeParamInput(port);
                break;
            }
        }
    }

    // ----------------------
    // PORT QUERIES
    // ----------------------
    QVector<std::shared_ptr<PortItemView>> NodeItemView::getAllPorts() const
    {
        QVector<std::shared_ptr<PortItemView>> all;
        all += m_inputs;
        all += m_outputs;
        all += m_parameterPorts.keys().toVector();
        return all;
    }

    QVector<std::shared_ptr<PortItemView>> NodeItemView::inputs() const { return m_inputs; }
    QVector<std::shared_ptr<PortItemView>> NodeItemView::outputs() const { return m_outputs; }
    QMap<QWidget*, QGraphicsProxyWidget*> NodeItemView::parameterWidgets() const { return m_parameterWidgets; }
    QMap<std::shared_ptr<PortItemView>, QGraphicsProxyWidget*> NodeItemView::parameterPorts() const { return m_parameterPorts; }
    QList<std::shared_ptr<PortItemView>> NodeItemView::paramsInputs() const { return m_parameterPorts.keys(); }

    // ----------------------
    // TITLE / TEXT
    // ----------------------
    void NodeItemView::set_text(const std::string& t)
    {
        if (m_displayedNodeName.toStdString() == t)
            return;
        setDisplayedNodeName(QString::fromStdString(t));
        text_changed.notify(t);
    }

    std::shared_ptr<PortItemView> NodeItemView::addPortView(const std::shared_ptr<PortItemView>& port, QWidget* widget)
    {
        if (!port)
            return nullptr;

        if (port->isInputPort())
        {
            m_inputs.append(port);
        }
        else if (port->isOutputPort())
        {
            m_outputs.append(port);
        }
        else if (port->isParameterPort())
        {
            m_parameterPorts[port] = nullptr;
            if (widget != nullptr)
            {
                auto* proxy = new QGraphicsProxyWidget(this);
                proxy->setWidget(widget);
                m_parameterWidgets[widget] = proxy;
                m_parameterPorts[port] = proxy;
            }
        }
        else
        {
            return nullptr;
        }

        updateLayout();
        return port;
    }

    void NodeItemView::setDisplayedNodeName(const QString& t)
    {
        m_displayedNodeName = t;
        if (m_nodeNameLabel != nullptr)
            m_nodeNameLabel->set_text(t.toStdString());
        m_titleHeight = m_nodeNameLabel->boundingRect().height();
        updateLayout();
    }

    QString NodeItemView::displayedNodeName() const { return m_displayedNodeName; }
    void NodeItemView::setNodeName(const QString& t) { m_nodeName = t; }
    QString NodeItemView::nodeName() const { return m_nodeName; }
    void NodeItemView::setNodeNameColor(const QColor& c)
    {
        m_nodeNameColor = c;
        update();
    }

    // ----------------------
    // LAYOUT
    // ----------------------
    void NodeItemView::updateLayout()
    {
        updateRect();

        if (m_nodeNameLabel != nullptr)
        {
            auto lbl = m_nodeNameLabel->boundingRect();
            m_nodeNameLabel->set_pos((m_rect.width() - lbl.width()) / 2.0, (m_titleHeight - lbl.height()) / 2.0);
        }

        qreal yInput = m_titleHeight + m_margin;
        for (const auto& port : std::as_const(m_inputs))
        {
            port->set_pos(pos().x() + m_margin, pos().y() + yInput);
            yInput += port->boundingRect().height() + m_spacing;
        }

        qreal yOutput = m_titleHeight + m_margin;
        for (const auto& port : std::as_const(m_outputs))
        {
            port->set_pos(pos().x() + m_rect.width() - m_margin - port->boundingRect().width(), pos().y() + yOutput);
            yOutput += port->boundingRect().height() + m_spacing;
        }

        qreal paramX = m_margin + m_maxInputWidth + m_spacing;
        qreal yParam = m_titleHeight + m_margin;
        for (auto it = m_parameterPorts.constBegin(); it != m_parameterPorts.constEnd(); ++it)
        {
            it.key()->set_pos(pos().x() + paramX, pos().y() + yParam);
            it.value()->setPos(pos().x() + paramX, pos().y() + yParam + it.key()->boundingRect().height());
            yParam += it.key()->boundingRect().height() + it.value()->boundingRect().height() + m_spacing;
        }

        qreal height = std::max({yInput, yOutput, yParam}) + m_margin;
        m_rect = QRectF(0, 0, std::max(m_rect.width(), m_rect.width()), height);
        prepareGeometryChange();
        update();
    }

    void NodeItemView::disconnectAllPorts()
    {
        for (const auto& input : std::as_const(m_inputs))
            if (input)
            {
                // input->deleteLater();
            }
        m_inputs.clear();

        for (const auto& output : std::as_const(m_outputs))
            if (output)
            {
                // output->deleteLater();
            }
        m_outputs.clear();

        for (auto port : m_parameterPorts.keys())
        {
            auto* proxy = m_parameterPorts.value(port);
            port.reset();
            proxy->deleteLater();
        }
        m_parameterPorts.clear();
        m_parameterWidgets.clear();
    }

    void NodeItemView::updateRect()
    {
        qreal maxInputWidth = 0;
        int maxInputHeight = 0;
        int maxOutputHeight = 0;
        int maxParamHeight = 0;

        for (const auto& in : std::as_const(m_inputs))
            if (in)
            {
                maxInputWidth = std::fmax(maxInputWidth, in->boundingRect().width());
                maxInputHeight += (int)in->boundingRect().height() + m_spacing;
            }

        qreal maxOutputWidth = 0;
        for (const auto& out : std::as_const(m_outputs))
            if (out)
            {
                maxOutputWidth = std::fmax(maxOutputWidth, out->boundingRect().width());
                maxOutputHeight += (int)out->boundingRect().height() + m_spacing;
            }

        qreal maxParamWidth = 0;
        for (auto* proxy : m_parameterWidgets.values())
            if (proxy != nullptr)
            {
                maxParamWidth = std::fmax(maxParamWidth, proxy->boundingRect().width());
                maxParamHeight += (int)proxy->boundingRect().height() + m_spacing;
            }

        qreal width = maxInputWidth + maxOutputWidth + 40 + maxParamWidth + 2 * m_margin + 2 * m_margin;

        qreal contentHeight = m_titleHeight + m_margin;

        contentHeight += std::max({(qreal)maxInputHeight, (qreal)maxOutputHeight, (qreal)maxParamHeight});

        m_rect = QRectF(0, 0, width, contentHeight + m_margin);
    }

    QRectF NodeItemView::boundingRect() const
    {
        return m_rect;
    }

    void NodeItemView::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
    {
        drawBackground(*painter);
        drawTitle(*painter);
        drawGlowingBounding(*painter);
    }
    void NodeItemView::drawBackground(QPainter& painter) const
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(m_borderColor, 1));
        painter.setBrush(m_bgColor);
        painter.drawRoundedRect(m_rect, 10, 10);
    }

    void NodeItemView::drawTitle(QPainter& painter) const
    {
        QLinearGradient gradient(0, 0, m_rect.width(), 10);
        gradient.setColorAt(0, m_nodeNameColor.lighter(150));
        gradient.setColorAt(1, m_nodeNameColor.darker(120));
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRoundedRect(QRectF(0, 0, m_rect.width(), m_titleHeight), 10, 10);

        QRectF overlap(0, m_titleHeight - 5, m_rect.width(), 10);
        painter.setBrush(m_bgColor);
        painter.drawRect(overlap);
    }

    void NodeItemView::drawGlowingBounding(QPainter& painter)
    {
        if (!hovered_ && !select_)
            return;

        QRectF glowRect = m_rect.adjusted(-2, -2, 2, 2);
        QColor glowColor = select_ ? QColor(0, 255, 100, 100) : QColor(0, 255, 255, 100);

        QPen glowPen(glowColor);
        glowPen.setWidth(12);
        glowPen.setJoinStyle(Qt::RoundJoin);

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(glowPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(glowRect, 10, 10);
    }

} // namespace nodeeditor::core::view
