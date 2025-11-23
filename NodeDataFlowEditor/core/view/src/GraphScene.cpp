#include "core/view/GraphScene.hpp"

#include "core/model/ConnectionPathModel.hpp"
#include "core/model/NodeItemModel.hpp"
#include "core/model/PortItemModel.hpp"
#include "core/presenter/ConnectionPathPresenter.hpp"
#include "core/presenter/NodeItemPresenter.hpp"
#include "core/presenter/PortItemPresenter.hpp"
#include "core/view/ConnectionPathView.hpp"
#include "core/view/NodeItemView.hpp"
#include "core/view/PortItemView.hpp"
#include <qgraphicssceneevent.h>

using namespace nodeeditor::core;

NodeEditorScene::NodeEditorScene(QObject* parent)
    : QGraphicsScene(parent)
{}

std::shared_ptr<nodeeditor::core::presenter::NodeItemPresenter>
NodeEditorScene::createNode(
    const QString& name,
    const QPointF& pos)
{
    auto model = std::make_shared<model::NodeItemModel>();
    model->set_pos(common::utility::SPos(pos.x(), pos.y()));
    model->set_text(name.toStdString());
    auto view = std::make_shared<view::NodeItemView>(name, name);
    view->setPos(pos);

    auto presenter = std::make_shared<nodeeditor::core::presenter::NodeItemPresenter>(model, view);

    this->addItem(view.get());
    m_nodes[name] = presenter;

    return presenter;
}

bool
NodeEditorScene::removeNode(const QString& nodeId)
{
    auto it = m_nodes.find(nodeId);
    if (it == m_nodes.end())
        return false;

    auto presenter = it->second;
    auto view = dynamic_cast<nodeeditor::core::view::ConnectionPathView*>(presenter->view().get());

    std::vector<std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter>> connsToRemove;
    for (const auto& cp : m_connections)
    {
        auto connModel = dynamic_cast<nodeeditor::core::model::ConnectionPathModel*>(cp->model().get());

        if (connModel->input().nodeName == nodeId ||
            connModel->output().nodeName == nodeId)
        {
            connsToRemove.push_back(cp);
        }
    }
    for (auto& cid : connsToRemove)
        removeConnection(cid);

    if (view)
        this->removeItem(view);

    m_nodes.erase(it);

    return true;
}

std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter>
NodeEditorScene::createConnection(
    const std::shared_ptr<nodeeditor::core::presenter::PortItemPresenter>& from,
    const std::shared_ptr<nodeeditor::core::presenter::PortItemPresenter>& to)
{
    if (!from || !to)
        return nullptr;

    auto viewFrom = dynamic_cast<nodeeditor::core::view::PortItemView*>(from->view().get());
    auto viewTo = dynamic_cast<nodeeditor::core::view::PortItemView*>(to->view().get());

    ConnectionPortData p1{viewFrom->pos(), viewFrom->boundingRect(), viewFrom->name(), viewFrom->displayName(), true};
    ConnectionPortData p2{viewTo->pos(), viewTo->boundingRect(), viewTo->name(), viewTo->displayName(), false};
    auto model = std::make_shared<model::ConnectionPathModel>();

    auto view = std::make_shared<view::ConnectionPathView>(p1, p2);

    auto presenter = std::make_shared<presenter::ConnectionPathPresenter>(model, view);
    auto rawView = view.get();
    this->addItem(rawView);
    m_connections.emplace_back(presenter);
    viewFrom->pos_changed.connect([this, rawView](const common::utility::SPos& pos) {
        if (rawView)
            rawView->set_inputPos(pos);
    });
    viewTo->pos_changed.connect([this, rawView](const common::utility::SPos& pos) {
        if (rawView)
            rawView->set_outputPos(pos);
    });
    return presenter;
}

bool
NodeEditorScene::createConnection(const QString& fromNode,
                                  const QString& fromPort,
                                  const QString& toNode,
                                  const QString& toPort)
{
    auto fromIt = m_nodes.find(fromNode);
    if (fromIt == m_nodes.end())
        return false;

    auto fromPresenter = fromIt->second->getInputPort(fromPort.toStdString());

    auto toIt = m_nodes.find(toNode);
    if (toIt == m_nodes.end())
        return false;
    auto toPresenter = toIt->second->getOutputPort(toPort.toStdString());

    createConnection(fromPresenter, toPresenter);

    return true;
}

bool
NodeEditorScene::removeConnection(
    std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter> connectionPtr)
{
    if (!connectionPtr)
        return false;

    auto it = std::find(m_connections.begin(), m_connections.end(), connectionPtr);

    if (it == m_connections.end())
        return false;

    auto viewBase = (*it)->view().get();
    auto viewConn = dynamic_cast<nodeeditor::core::view::ConnectionPathView*>(viewBase);

    if (viewConn)
        this->removeItem(viewConn);

    m_connections.erase(it);

    return true;
}

const std::vector<std::shared_ptr<presenter::ConnectionPathPresenter>>&
NodeEditorScene::connections() const
{
    return m_connections;
}

const std::unordered_map<QString, std::shared_ptr<presenter::NodeItemPresenter>>&
NodeEditorScene::nodes() const
{
    return m_nodes;
}

std::shared_ptr<presenter::PortItemPresenter>
NodeEditorScene::addInputPort(
    const QString& nodeId,
    const QString& portName,
    const QString& displayName)
{
    auto it = m_nodes.find(nodeId);
    if (it == m_nodes.end())
        return nullptr;

    auto nodePresenter = it->second;
    auto nodeView = dynamic_cast<view::NodeItemView*>(nodePresenter->view().get());
    if (!nodeView)
        return nullptr;

    auto portModel = std::make_shared<model::PortItemModel>();
    portModel->set_name(portName.toStdString());
    portModel->set_module_name(nodeId.toStdString());
    portModel->set_display_name(displayName.toStdString());
    portModel->set_orientation(common::utility::SPort::Orientation::Input);

    auto portView = std::make_shared<view::PortItemView>(
        portName,
        displayName,
        nodeView->nodeName(),
        common::utility::SPort::Orientation::Input);

    nodeView->addPortView(portView);

    auto portPresenter = std::make_shared<presenter::PortItemPresenter>(portModel, portView);

    nodePresenter->addPortPresenter(portPresenter);

    this->addItem(portView.get());

    return portPresenter;
}

std::shared_ptr<presenter::PortItemPresenter>
NodeEditorScene::addOutputPort(
    const QString& nodeId,
    const QString& portName,
    const QString& displayName)
{
    auto it = m_nodes.find(nodeId);
    if (it == m_nodes.end())
        return nullptr;

    auto nodePresenter = it->second;
    auto nodeView = dynamic_cast<view::NodeItemView*>(nodePresenter->view().get());
    if (!nodeView)
        return nullptr;

    auto portModel = std::make_shared<model::PortItemModel>();
    portModel->set_name(portName.toStdString());
    portModel->set_module_name(nodeId.toStdString());
    portModel->set_display_name(displayName.toStdString());
    portModel->set_orientation(common::utility::SPort::Orientation::Output);

    auto portView = std::make_shared<view::PortItemView>(
        portName,
        displayName,
        nodeView->nodeName(),
        common::utility::SPort::Orientation::Output);

    nodeView->addPortView(portView);

    auto portPresenter = std::make_shared<presenter::PortItemPresenter>(portModel, portView);

    nodePresenter->addPortPresenter(portPresenter);

    this->addItem(portView.get());

    return portPresenter;
}

bool
NodeEditorScene::removePort(
    const QString& nodeId,
    const QString& portName)
{
    auto itNode = m_nodes.find(nodeId);
    if (itNode == m_nodes.end())
        return false;

    auto nodePresenter = itNode->second;
    auto nodeView = dynamic_cast<view::NodeItemView*>(nodePresenter->view().get());
    if (!nodeView)
        return false;

    std::shared_ptr<presenter::PortItemPresenter> portPresenter = nullptr;
    for (auto& pp : nodePresenter->ports()) // assume NodeItemPresenter has ports() returning vector/shared_ptr
    {
        auto pView = dynamic_cast<view::PortItemView*>(pp->view().get());
        if (pView && pView->name() == portName)
        {
            portPresenter = pp;
            break;
        }
    }

    if (!portPresenter)
        return false;

    std::vector<std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter>> connsToRemove;
    for (const auto& cp : m_connections)
    {
        auto connModel = dynamic_cast<model::ConnectionPathModel*>(cp->model().get());
        if (connModel->input().name == portName && connModel->input().nodeName == nodeId || connModel->output().name == portName && connModel->output().nodeName == nodeId)
        {
            connsToRemove.push_back(cp);
        }
    }
    for (auto& cid : connsToRemove)
        removeConnection(cid);

    auto portView = dynamic_cast<view::PortItemView*>(portPresenter->view().get());
    if (portView)
        this->removeItem(portView);

    nodePresenter->removePortPresenter(portPresenter);

    return true;
}

void
NodeEditorScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* clickedItem = itemAt(event->scenePos(), QTransform());

    if (clickedItem)
    {
        for (auto item : selectedItems())
        {
            if (item != clickedItem)
                item->setSelected(false);
        }

        clickedItem->setSelected(true);
    }
    else
    {
        for (auto item : selectedItems())
            item->setSelected(false);
    }

    QGraphicsScene::mousePressEvent(event);
}
