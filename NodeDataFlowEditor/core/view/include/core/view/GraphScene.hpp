#pragma once
#include <QGraphicsScene>
#include <memory>
#include <unordered_map>

namespace nodeeditor::core::presenter
{
    class ConnectionPathPresenter;
    class PortItemPresenter;
    class NodeItemPresenter;
} // namespace nodeeditor::core::presenter
namespace nodeeditor
{
    namespace core
    {

        class NodeEditorScene : public QGraphicsScene
        {
            Q_OBJECT

        public:
            explicit NodeEditorScene(QObject* parent = nullptr);

            std::shared_ptr<presenter::NodeItemPresenter> createNode(
                const QString& name,
                const QPointF& pos);

            std::shared_ptr<presenter::ConnectionPathPresenter> createConnection(
                const std::shared_ptr<presenter::PortItemPresenter>& from,
                const std::shared_ptr<presenter::PortItemPresenter>& to);

            bool createConnection(const QString& fromNode,
                                  const QString& fromPort,
                                  const QString& toNode,
                                  const QString& toPort);

            bool removeNode(const QString& nodeId);
            bool removeConnection(std::shared_ptr<presenter::ConnectionPathPresenter> connectionId);

            const std::unordered_map<QString, std::shared_ptr<nodeeditor::core::presenter::NodeItemPresenter>>& nodes() const;

            const std::vector<std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter>>& connections() const;

            std::shared_ptr<presenter::PortItemPresenter> addInputPort(
                const QString& nodeId,
                const QString& portName,
                const QString& displayName);

            std::shared_ptr<presenter::PortItemPresenter> addOutputPort(
                const QString& nodeId,
                const QString& portName,
                const QString& displayName);

        public:
            bool removePort(
                const QString& nodeId,
                const QString& portName);

        private:
            void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

            std::unordered_map<QString, std::shared_ptr<nodeeditor::core::presenter::NodeItemPresenter>> m_nodes;
            std::vector<std::shared_ptr<nodeeditor::core::presenter::ConnectionPathPresenter>> m_connections;
        };

    } // namespace core
} // namespace nodeeditor
