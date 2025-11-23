#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "core/view/GraphScene.hpp"

int
main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    nodeeditor::core::NodeEditorScene scene;

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setSceneRect(0, 0, 400, 300);
    view.setBackgroundBrush(Qt::gray);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.show();
    scene.createNode("node", {100, 100});
    scene.addInputPort("node", "p1", "input1");
    scene.addOutputPort("node", "p2", "output1");
    scene.createNode("node2", {500, 500});
    scene.addInputPort("node2", "p1", "input1");
    scene.addOutputPort("node2", "p2", "output1");

    scene.createConnection("node", "p1", "node2", "p2");
    
    return app.exec();
}
